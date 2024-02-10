#include "evaluator.hpp"
#include <algorithm>
#include <phi/exception.hpp>
#include <phi/runtime/builtin/global.hpp>
#include <phi/runtime/builtin/import.hpp>
#include <phi/runtime/follower.hpp>
#include <phi/singleton.hpp>

namespace phi
{
    void Evaluator::setup(const State &state)
    {
        _M_state.reset(&state);
        try
        {
            eval();
        }
        catch (...)
        {
            _M_state.release();
            throw;
        }
        _M_state.release();
    }
    Ref<Variant> Evaluator::eval()
    {
        const vector<OPCode> &codes = _M_state->getCodes();
        for (_M_stream = 0; _M_stream < codes.size();
             ++_M_stream, Singleton<ProgramFollower>::instance()->line(_M_state->line(_M_stream)))
        {
            Ref<Variant> res = handle(codes[_M_stream]);
            if (res)
            {
                clear();
                return res;
            }
        }
        return Variant::Null;
    }

#define BINARY_IMPL(opcode, op)                                       \
    case OPCode::Command::opcode:                                     \
    {                                                                 \
        VariantPacker right = pop();                                  \
        VariantPacker left = pop();                                   \
        push(Ref<Variant>{new Variant{left.data() op right.data()}}); \
        break;                                                        \
    }
#define BINARY_M_IMPL(opcode, method)                                      \
    case OPCode::Command::opcode:                                          \
    {                                                                      \
        VariantPacker right = pop();                                       \
        VariantPacker left = pop();                                        \
        push(Ref<Variant>{new Variant{left.data().method(right.data())}}); \
        break;                                                             \
    }
#define UNARY_IMPL(opcode, op)                              \
    case OPCode::Command::opcode:                           \
    {                                                       \
        VariantPacker operand = pop();                      \
        push(Ref<Variant>{new Variant{op operand.data()}}); \
        break;                                              \
    }
#define UNARY_M_IMPL(opcode, method)                              \
    case OPCode::Command::opcode:                                 \
    {                                                             \
        VariantPacker operand = pop();                            \
        push(Ref<Variant>{new Variant{operand.data().method()}}); \
        break;                                                    \
    }

    Ref<Variant> Evaluator::handle(const OPCode &code)
    {
        switch (code.opt())
        {
            BINARY_IMPL(ADD, +)
            BINARY_IMPL(SUB, -)
            BINARY_IMPL(MUL, *)
            BINARY_IMPL(DIV, /)
            BINARY_IMPL(MOD, %)
            BINARY_M_IMPL(POW, power)
            BINARY_IMPL(BAND, &)
            BINARY_IMPL(BOR, |)
            BINARY_IMPL(BXOR, ^)
            BINARY_IMPL(LAND, &&)
            BINARY_IMPL(LOR, ||)
            BINARY_IMPL(LSHIFT, <<)
            BINARY_IMPL(RSHIFT, >>)
            BINARY_IMPL(EQ, ==)
            BINARY_IMPL(NE, !=)
            BINARY_IMPL(LT, <)
            BINARY_IMPL(LE, <=)
            BINARY_IMPL(GT, >)
            BINARY_IMPL(GE, >=)
            UNARY_IMPL(NOT, !)
            UNARY_IMPL(REV, ~)
            UNARY_IMPL(NEG, -)
            UNARY_IMPL(INC, ++)
            UNARY_IMPL(RED, --)
            UNARY_M_IMPL(COPY, copy)
            UNARY_M_IMPL(DCPY, deepCopy)
        case OPCode::Command::IMPORT:
        {
            Ref<Variant> module = import(*_M_state->lookup(code.value()));
            /*
            Don't worry about taking the address of a temporary variable here,
            it's just to tell VariantPacker that it's a Variable rather than a Constant,
            and its value already exists in the -M_data variable
            */
            push(VariantPacker::variable_t{&module});
            break;
        }
        case OPCode::Command::CLOSURE_BIND:
        {
            VariantPacker value = pop();
            VariantPacker func = pop();
            if (func->type() != Variant::Type::FUNCTION)
                throw CompilerException{"Bad OPCode::Command::CLOSURE_BIND. (Not a function)"};
            Ref<Variant> rename = _M_state->lookup(code.value());
            func->access({rename}) = value.pointer();
            push(func);
            break;
        }
        case OPCode::Command::ARGS:
        {
            push(VariantPacker{true});
            break;
        }
        case OPCode::Command::CALL:
        {
            VariantPacker operand = pop();
            if (operand->isNull())
                throw OperateNullException{operand, "call"};
            array args;
            while (!top().isArgs())
                args.push_back(pop().pointer());
            pop(); // pop the args flag.
            std::reverse(args.begin(), args.end());
            Singleton<ProgramFollower>::instance()->callBegin({operand.nameSafely(), _M_state->line(_M_stream)});
            push({operand.data().call(args)});
            Singleton<ProgramFollower>::instance()->callEnd();
            break;
        }
        case OPCode::Command::ACCESS:
        {
            VariantPacker operand = pop();
            array args;
            while (!top().isArgs())
                args.push_back(pop().pointer());
            pop(); // pop the args flag.
            std::reverse(args.begin(), args.end());
            push(operand->access(args));
            break;
        }
        case OPCode::Command::MAKE_ARRAY:
        {
            Owner<array> temp = new array;
            while (!top().isArgs())
                temp->push_back(pop().pointer());
            pop(); // pop the args flag.
            std::reverse(temp->begin(), temp->end());
            push(Ref<Variant>{new Variant{std::move(temp)}});
            break;
        }
        case OPCode::Command::MAKE_DICT:
        {
            Owner<dict> temp = new dict;
            Ref<Variant> value;
            while (!top().isArgs())
            {
                if (!value)
                {
                    value = pop().pointer();
                    continue;
                }
                temp->insert({pop().pointer(), value});
                value = nullptr;
            }
            pop(); // pop the args flag.
            push(Ref<Variant>{new Variant{std::move(temp)}});
            break;
        }
        case OPCode::Command::DEL:
        {
            Env &env = get_env();
            pop().free(env);
            break;
        }
        case OPCode::Command::ASSIGN:
        {
            VariantPacker value = pop();
            VariantPacker operand = pop();
            push(operand.assign(value));
            break;
        }
        case OPCode::Command::ALLOCATE:
        {
            push(allocate(*_M_state->lookup(code.value())));
            break;
        }
        case OPCode::Command::LOAD:
        {
            push(load(*_M_state->lookup(code.value())));
            break;
        }
        case OPCode::Command::LOAD_CONST:
        {
            push(Ref<Variant>{new Variant{_M_state->lookup(code.value())->deepCopy()}});
            break;
        }
        case OPCode::Command::PUSH_VAL:
        {
            push(_M_state->lookup(code.value()));
            break;
        }
        case OPCode::Command::POP_TOP:
        {
            pop();
            break;
        }
        case OPCode::Command::PUSH_ENV:
        {
            _M_envs.push_back(Environment());
            break;
        }
        case OPCode::Command::POP_ENV:
        {
            _M_envs.pop_back();
            break;
        }
        case OPCode::Command::CLEAR:
        {
            clear_stack();
            break;
        }
        case OPCode::Command::GOTO:
        {
            stream_goto(_M_state->label(code.value()));
            break;
        }
        case OPCode::Command::IFTRUE:
        {
            if (pop().data())
                stream_goto(_M_state->label(code.value()));
            break;
        }
        case OPCode::Command::IFFALSE:
        {
            if (!pop().data())
                stream_goto(_M_state->label(code.value()));
            break;
        }
        case OPCode::Command::RETURN:
        {
            return pop().pointer();
        }
        }
        return nullptr;
    }
    VariantPacker Evaluator::load(const string &name, bool throws)
    {
        auto it = _M_envs.end();
        while (--it != _M_envs.end())
            if (it->has(name))
                return it->load(name);
        if (name != "this")
        {
            VariantPacker this_ = load("this", false);
            if (!this_.isNull() && this_->hasProperty(name))
                return this_->access({new Variant{name}});
        }
        if (hasGlobal(name))
            return getGlobal(name);
        if (throws)
            throw LoadException{"Attempt to load a unallocated variable. (\"%s\")", name.c_str()};
        else
            return {Variant::Null, name};
    }
    VariantPacker Environment::allocate(const string &name)
    {
        return {VariantPacker::variable_t{&_M_locals.insert({name, new Variant}).first->second}, name};
    }
    void Environment::setLocal(const string &name, Ref<Variant> value)
    {
        _M_locals[name] = value;
    }
    VariantPacker Environment::load(const string &name)
    {
        return {VariantPacker::variable_t{&_M_locals[name]}, name};
    }
    bool Environment::has(const string &name) const
    {
        return _M_locals.find(name) != _M_locals.end();
    }
    void Environment::free(const string &name)
    {
        _M_locals.erase(name);
    }
    void VariantPacker::free(Env &env)
    {
        if (!isVariable())
            return;
        env.free(name());
    }
    VariantPacker &VariantPacker::assign(const VariantPacker &other)
    {
        if (hasName() && _M_data->type() == Variant::Type::OBJECT)
            _M_data->seeAs<Object>().set(*_M_name, other.pointer());
        else if (isVariable() && other.isVariable())
            redirectTo(other.pointer());
        else
            *_M_data = other.data();
        return *this;
    }
    VariantPacker &VariantPacker::assign(const Ref<Variant> &other)
    {
        if (hasName() && _M_data->type() == Variant::Type::OBJECT)
            _M_data->seeAs<Object>().set(*_M_name, other);
        else if (isVariable())
            redirectTo(other);
        else
            *_M_data = *other;
        return *this;
    }
} // namespace phi