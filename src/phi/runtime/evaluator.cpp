#include "evaluator.hpp"

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
        for (_M_stream = 0; _M_stream < codes.size(); ++_M_stream)
        {
            Ref<Variant> res = handle(codes[_M_stream]);
            if (res)
            {
                clear();
                return res;
            }
        }
        return nullptr;
    }

#define BINARY_IMPL(opcode, op)                     \
    case OPCode::Command::opcode:                   \
    {                                               \
        VariantPacker right = pop();                \
        VariantPacker left = pop();                 \
        push(Variant{left.data() op right.data()}); \
        break;                                      \
    }
#define BINARY_M_IMPL(opcode, method)                    \
    case OPCode::Command::opcode:                        \
    {                                                    \
        VariantPacker right = pop();                     \
        VariantPacker left = pop();                      \
        push(Variant{left.data().method(right.data())}); \
        break;                                           \
    }
#define UNARY_IMPL(opcode, op)            \
    case OPCode::Command::opcode:         \
    {                                     \
        VariantPacker operand = pop();    \
        push(Variant{op operand.data()}); \
        break;                            \
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
            push(*_M_state->lookup(code.value()));
            break;
        }
        case OPCode::Command::PUSH_VAL:
        {
            push(_M_state->lookup(code.value()));
            break;
        }
        case OPCode::Command::POP_VAL:
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
            stream_goto(code.value());
            break;
        }
        case OPCode::Command::IFTRUE:
        {
            if (pop().data())
                stream_goto(code.value());
            break;
        }
        case OPCode::Command::IFFALSE:
        {
            if (!pop().data())
                stream_goto(code.value());
            break;
        }
        case OPCode::Command::RETURN:
        {
            return pop().pointer();
        }
        }
        return nullptr;
    }
    VariantPacker Evaluator::load(const string &name)
    {
        auto it = _M_envs.end();
        while (--it != _M_envs.end())
            if (it->has(name))
                return it->load(name);
        return Variant::Null;
    }
    VariantPacker Environment::allocate(const string &name)
    {
        _M_locals[name] = new Variant;
        return VariantPacker::source_t{&(_M_locals[name])};
    }
    VariantPacker Environment::load(const string &name)
    {
        return _M_locals[name];
    }
    bool Environment::has(const string &name) const
    {
        return _M_locals.find(name) != _M_locals.end();
    }
} // namespace phi