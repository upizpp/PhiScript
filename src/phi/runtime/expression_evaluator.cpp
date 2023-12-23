#include "expression_evaluator.hpp"
#include "block.hpp"
#include "expression.hpp"
#include <phi/builtin/method.hpp>
#include <phi/exception.hpp>
#include <phi/function.hpp>
#include <phi/phi_tool.hpp>

#define UNARY_COMMAND(cmd, fname)                                 \
    {                                                             \
        OPCode::Command::cmd,                                     \
            ExpressionEvaluator::CommandHandlerInfo{              \
                1,                                                \
                [](ExpressionEvaluator *evaluator, array &args) { \
                    Ref<Variant> v = args[0];                     \
                    evaluator->push(Variant{v->fname()});         \
                },                                                \
            },                                                    \
    }
#define BINARY_COMMAND(cmd, fname)                                \
    {                                                             \
        OPCode::Command::cmd,                                     \
            ExpressionEvaluator::CommandHandlerInfo{              \
                2,                                                \
                [](ExpressionEvaluator *evaluator, array &args) { \
                    Ref<Variant> v1 = args[0];                    \
                    Ref<Variant> v2 = args[1];                    \
                    evaluator->push(Variant{v1->fname(*v2)});     \
                },                                                \
            },                                                    \
    }
#define BINARY_COMMAND_OP(cmd, op)                                \
    {                                                             \
        OPCode::Command::cmd,                                     \
            ExpressionEvaluator::CommandHandlerInfo{              \
                2,                                                \
                [](ExpressionEvaluator *evaluator, array &args) { \
                    Ref<Variant> v1 = args[0];                    \
                    Ref<Variant> v2 = args[1];                    \
                    evaluator->push(Variant{*v1 op * v2});        \
                },                                                \
            },                                                    \
    }
#define ASSIGN_COMMAND(cmd, fname)                              \
    {                                                           \
        OPCode::Command::ADDASN,                                \
            ExpressionEvaluator::CommandHandlerInfo{            \
                2,                                              \
                [](ExpressionEvaluator *evaluator, array &args) \
                {                                               \
                    *args[0] = args[0]->fname(*args[1]);        \
                    evaluator->push(args[0]);                   \
                },                                              \
            },                                                  \
    }

namespace phi
{
    namespace runtime
    {
        ExpressionEvaluator::HandlerContainer ExpressionEvaluator::commandHandlers = {
            BINARY_COMMAND(ADD, add),
            BINARY_COMMAND(SUB, sub),
            UNARY_COMMAND(NEG, negative),
            BINARY_COMMAND(MUL, mul),
            BINARY_COMMAND(DIV, div),
            BINARY_COMMAND(MOD, mod),
            BINARY_COMMAND(POWER, power),
            BINARY_COMMAND(BAND, band),
            BINARY_COMMAND(BOR, bor),
            BINARY_COMMAND(BXOR, bxor),
            UNARY_COMMAND(BNOT, reverse),
            BINARY_COMMAND(LAND, land),
            BINARY_COMMAND(LOR, lor),
            BINARY_COMMAND(LXOR, lxor),
            UNARY_COMMAND(LNOT, lnot),
            UNARY_COMMAND(COPY, copy),
            UNARY_COMMAND(COPYDEEP, copyDeep),
            BINARY_COMMAND(LSHIFT, left_shift),
            BINARY_COMMAND(RSHIFT, right_shift),
            BINARY_COMMAND_OP(EQ, ==),
            BINARY_COMMAND_OP(NE, !=),
            BINARY_COMMAND_OP(LT, <),
            BINARY_COMMAND_OP(LE, <=),
            BINARY_COMMAND_OP(GT, >),
            BINARY_COMMAND_OP(GE, >=),
            {
                OPCode::Command::LOAD,
                ExpressionEvaluator::CommandHandlerInfo{
                    1,
                    [](ExpressionEvaluator *evaluator, array &args)
                    {
                        string name = *args[0];
                        evaluator->push(evaluator->block()->loadVariable(name));
                    },
                },
            },
            {
                OPCode::Command::ALLOCATE,
                ExpressionEvaluator::CommandHandlerInfo{
                    1,
                    [](ExpressionEvaluator *evaluator, array &args)
                    {
                        string name = *args[0];
                        evaluator->push(evaluator->block()->allocateVariable(name));
                    },
                },
            },
            {
                OPCode::Command::DELETE,
                ExpressionEvaluator::CommandHandlerInfo{
                    1,
                    [](ExpressionEvaluator *evaluator, array &args)
                    {
                        string name = *args[0];
                        evaluator->block()->deleteVariable(name);
                    },
                },
            },
            {
                OPCode::Command::ASSIGN,
                ExpressionEvaluator::CommandHandlerInfo{
                    2,
                    [](ExpressionEvaluator *evaluator, array &args)
                    {
                        *args[0] = *args[1];
                        evaluator->push(args[0]);
                    },
                },
            },
            {
                OPCode::Command::ACCESS,
                ExpressionEvaluator::CommandHandlerInfo{
                    0,
                    [](ExpressionEvaluator *evaluator, array &args)
                    {
                        array access_args;
                        Ref<Variant> temp;
                        while ((temp = evaluator->pop())->flag() != FLAG_OPCODE_ARGS)
                            access_args.push_back(temp);
                        std::reverse(access_args.begin(), access_args.end());

                        Ref<Variant> value = evaluator->pop();
                        evaluator->push(value->access(access_args, value));
                    },
                },
            },
            {
                OPCode::Command::CALL,
                ExpressionEvaluator::CommandHandlerInfo{
                    0,
                    [](ExpressionEvaluator *evaluator, array &args)
                    {
                        // TODO CALLING STATIC FOLLOW
                        array parameters;
                        Ref<Variant> temp;
                        while ((temp = evaluator->pop())->flag() != FLAG_OPCODE_ARGS)
                            parameters.push_back(temp);
                        std::reverse(parameters.begin(), parameters.end());

                        evaluator->push(evaluator->pop()->call(parameters));
                    },
                },
            },
            {
                OPCode::Command::MAKE_ARRAY,
                ExpressionEvaluator::CommandHandlerInfo{
                    0,
                    [](ExpressionEvaluator *evaluator, array &args)
                    {
                        Ref<Variant> arr{new Variant{Variant::Type::ARRAY}};
                        Ref<Variant> temp;
                        while ((temp = evaluator->pop())->flag() != FLAG_OPCODE_ARGS)
                            arr->getArray()->push_back(temp);
                        std::reverse(arr->getArray()->begin(), arr->getArray()->end());

                        evaluator->push(arr);
                    },
                },
            },
            {
                OPCode::Command::MAKE_DICTIONARY,
                ExpressionEvaluator::CommandHandlerInfo{
                    0,
                    [](ExpressionEvaluator *evaluator, array &args)
                    {
                        Ref<Variant> dict{new Variant{Variant::Type::DICTIONARY}};
                        Ref<Variant> key;
                        Ref<Variant> value;
                        while ((value = evaluator->pop())->flag() != FLAG_OPCODE_ARGS)
                        {
                            key = evaluator->pop();
                            dict->getDictionary()->insert(std::make_pair(*key, value));
                        }
                        evaluator->push(dict);
                    },
                },
            },
            {
                OPCode::Command::FN_LOCATE,
                ExpressionEvaluator::CommandHandlerInfo{
                    1,
                    [](ExpressionEvaluator *evaluator, array &args)
                    {
                        Ref<Variant> fn{args[0]};
                        if (fn->type() != Variant::Type::FUNCTION)
                            throw RuntimeException("Invalid OPCode FN_LOCATE.");
                        fn->getFunction()->locate(evaluator->block());
                        evaluator->push(fn);
                    },
                },
            },

            ASSIGN_COMMAND(ADD, add),
            ASSIGN_COMMAND(SUB, sub),
            ASSIGN_COMMAND(MUL, mul),
            ASSIGN_COMMAND(DIV, div),
            ASSIGN_COMMAND(AND, band),
            ASSIGN_COMMAND(OR, bor),
            ASSIGN_COMMAND(XOR, bxor),
            ASSIGN_COMMAND(LS, left_shift),
            ASSIGN_COMMAND(RS, right_shift),
        };

        void ExpressionEvaluator::clear()
        {
            phi::clear(m_data);
        }

        void ExpressionEvaluator::push(const OPCode &code)
        {
            if (code.command() == OPCode::Command::ARGS)
            {
                Ref<Variant> temp{new Variant};
                temp->setFlag(FLAG_OPCODE_ARGS);
                push(temp);
                return;
            }
            if (code.command() == OPCode::Command::DATA)
            {
                push(code.data());
                return;
            }
            CommandHandlerInfo &info = commandHandlers[code.command()];
            integer argument_count = info.first;
            CommandHandler &handler = info.second;
            array args;
            for (integer i = 0; i < argument_count; i++)
            {
                args.push_back(pop());
            }
            std::reverse(args.begin(), args.end());
            handler(this, args);
        }

        void ExpressionEvaluator::push(const Ref<Variant> &data)
        {
            m_data.push(data);
        }

        Ref<Variant> ExpressionEvaluator::pop()
        {
            if (m_data.empty())
                return Variant::Null;
            Ref<Variant> temp = std::move(m_data.top());
            m_data.pop();
            return temp;
        }

        Ref<Variant> ExpressionEvaluator::top()
        {
            return m_data.top();
        }

        Ref<Variant> ExpressionEvaluator::evalExpr(Expression &expr)
        {
            expr.toBegin();
            clear();
            while (!expr.eof())
                push(expr.next());
            return pop();
        }
    } // namespace runtime
} // namespace phi