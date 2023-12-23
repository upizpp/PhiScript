#include "test.hpp"
#include <phi/runtime/expression.hpp>
#include <phi/runtime/expression_evaluator.hpp>
#include <phi/runtime/block.hpp>

namespace test
{
    class EvalTest : public Test
    {
    public:
        virtual void _run()
        {
            using namespace phi;
            using namespace runtime;
            {
                GroupBegin("Eval");

                Expression expr;
                expr.push(Ref<Variant>{4});
                expr.push(Ref<Variant>{2});
                expr.push(OPCode::Command::DIV);

                ExpressionEvaluator evaluator;
                Assert(*evaluator.eval(expr), 2);
                
                GroupEnd();
            }

            {
                GroupBegin("Number Calculate");

                Expression expr;
                expr.push(Ref<Variant>{1});
                expr.push(Ref<Variant>{1});
                expr.push(OPCode::Command::ADD);;
                expr.push(Ref<Variant>{4.0});
                expr.push(OPCode::Command::DIV);;

                ExpressionEvaluator evaluator;
                Assert(*evaluator.eval(expr), 0.5);

                expr.push(Ref<Variant>{"mv_2=E_k"});
                expr.push(OPCode::Command::ADD);;
                Assert(*evaluator.eval(expr), "0.5mv_2=E_k");

                GroupEnd();
            }

            {
                GroupBegin("String Add");
                Expression expr;
                ExpressionEvaluator evaluator;
                
                expr.push(Ref<Variant>{"Hello, "});
                expr.push(Ref<Variant>{"World!"});
                expr.push(OPCode::Command::ADD);;

                Assert(*evaluator.eval(expr), "Hello, World!");
                
                GroupEnd();
            }

            {
                GroupBegin("Complex Number Calculate");
                Expression expr;
                ExpressionEvaluator evaluator;
                // 5 2 3 * + 18 - 2 / = -3
                expr.push(Ref<Variant>{5});
                expr.push(Ref<Variant>{2});
                expr.push(Ref<Variant>{3});
                expr.push(OPCode::Command::MUL);;
                expr.push(OPCode::Command::ADD);;
                expr.push(Ref<Variant>{18});
                expr.push(OPCode::Command::SUB);;
                expr.push(Ref<Variant>{2});
                expr.push(OPCode::Command::DIV);;
                Assert(*evaluator.eval(expr), -3);

                // 15 4 - 8 2 + * 6 / = 18
                expr.clear();
                expr.push(Ref<Variant>{15});
                expr.push(Ref<Variant>{4});
                expr.push(OPCode::Command::SUB);;
                expr.push(Ref<Variant>{8});
                expr.push(Ref<Variant>{2});
                expr.push(OPCode::Command::ADD);;
                expr.push(OPCode::Command::MUL);;
                expr.push(Ref<Variant>{6});
                expr.push(OPCode::Command::DIV);;
                Assert(*evaluator.eval(expr), 18);

                // 10 5 * 20 4 / + 30 2 1 + * - * = 1485
                expr.clear();
                expr.push(Ref<Variant>{10});
                expr.push(Ref<Variant>{5});
                expr.push(OPCode::Command::MUL);;
                expr.push(Ref<Variant>{20});
                expr.push(Ref<Variant>{4});
                expr.push(OPCode::Command::DIV);;
                expr.push(OPCode::Command::ADD);;
                expr.push(Ref<Variant>{30});
                expr.push(Ref<Variant>{2});
                expr.push(Ref<Variant>{1});
                expr.push(OPCode::Command::ADD);;
                expr.push(OPCode::Command::SUB);;
                expr.push(OPCode::Command::MUL);;
                Assert(*evaluator.eval(expr), 1485);

                // 5 2 + 3 * 18 - 2 / 7 8 * 9 + 10 - 11 12 * 13 / 14 + 15 16 + 17 * 18 - 19 / 20 21 * 22 + 23 - 24 25 * 26 / 27 + = 27
                expr.clear();
                expr.push(Ref<Variant>{5});
                expr.push(Ref<Variant>{2});
                expr.push(OPCode::Command::ADD);;
                expr.push(Ref<Variant>{3});
                expr.push(OPCode::Command::MUL);;
                expr.push(Ref<Variant>{18});
                expr.push(OPCode::Command::SUB);;
                expr.push(Ref<Variant>{2});
                expr.push(OPCode::Command::DIV);;
                expr.push(Ref<Variant>{7});
                expr.push(Ref<Variant>{8});
                expr.push(OPCode::Command::MUL);;
                expr.push(Ref<Variant>{9});
                expr.push(OPCode::Command::ADD);;
                expr.push(Ref<Variant>{10});
                expr.push(Ref<Variant>{11});
                expr.push(OPCode::Command::MUL);;
                expr.push(Ref<Variant>{12});
                expr.push(OPCode::Command::DIV);;
                expr.push(Ref<Variant>{13});
                expr.push(OPCode::Command::ADD);;
                expr.push(Ref<Variant>{14});
                expr.push(Ref<Variant>{15});
                expr.push(OPCode::Command::ADD);;
                expr.push(Ref<Variant>{16});
                expr.push(OPCode::Command::MUL);;
                expr.push(Ref<Variant>{17});
                expr.push(OPCode::Command::SUB);;
                expr.push(Ref<Variant>{18});
                expr.push(OPCode::Command::DIV);;
                expr.push(Ref<Variant>{19});
                expr.push(Ref<Variant>{20});
                expr.push(OPCode::Command::MUL);;
                expr.push(Ref<Variant>{21});
                expr.push(OPCode::Command::ADD);;
                expr.push(Ref<Variant>{22});
                expr.push(OPCode::Command::MUL);;
                expr.push(Ref<Variant>{23});
                expr.push(OPCode::Command::SUB);;
                expr.push(Ref<Variant>{24});
                expr.push(OPCode::Command::DIV);;
                expr.push(Ref<Variant>{25});
                expr.push(OPCode::Command::ADD);;
                expr.push(Ref<Variant>{26});
                expr.push(OPCode::Command::DIV);;
                expr.push(Ref<Variant>{27});
                Assert(*evaluator.eval(expr), 27);

                // 3 6 2 + * 5 / 2 7 - 8 * 4 / + 9 + 5 3 - * 7 2 4 * - 3 2 + / + = 7.6
                expr.clear();
                expr.push(Ref<Variant>{3.0}); // Avoid excessive loss of precision and use real types
                expr.push(Ref<Variant>{6});
                expr.push(Ref<Variant>{2});
                expr.push(OPCode::Command::ADD);;
                expr.push(OPCode::Command::MUL);;
                expr.push(Ref<Variant>{5});
                expr.push(OPCode::Command::DIV);;
                expr.push(Ref<Variant>{2});
                expr.push(Ref<Variant>{7});
                expr.push(OPCode::Command::SUB);;
                expr.push(Ref<Variant>{8});
                expr.push(OPCode::Command::MUL);;
                expr.push(Ref<Variant>{4});
                expr.push(OPCode::Command::DIV);;
                expr.push(OPCode::Command::ADD);;
                expr.push(Ref<Variant>{9});
                expr.push(OPCode::Command::ADD);;
                expr.push(Ref<Variant>{5});
                expr.push(Ref<Variant>{3});
                expr.push(OPCode::Command::SUB);;
                expr.push(OPCode::Command::MUL);;
                expr.push(Ref<Variant>{7});
                expr.push(Ref<Variant>{2});
                expr.push(Ref<Variant>{4});
                expr.push(OPCode::Command::MUL);;
                expr.push(OPCode::Command::SUB);;
                expr.push(Ref<Variant>{3});
                expr.push(Ref<Variant>{2});
                expr.push(OPCode::Command::ADD);;
                expr.push(OPCode::Command::DIV);;
                expr.push(OPCode::Command::ADD);;
                Assert(integer(*evaluator.eval(expr)), 7); // Approximately equal to 7.6

                GroupEnd();
            }   

            {
                GroupBegin("Variable loading");
                Expression expr;
                ExpressionEvaluator evaluator;
                expr.push(Ref<Variant>{"my_variant1"});
                expr.push(OPCode::Command::LOAD);;
                expr.push(Ref<Variant>{123});
                expr.push(OPCode::Command::ASSIGN);;
                expr.push(Ref<Variant>{"my_variant1"});
                expr.push(OPCode::Command::LOAD);;
                Assert(*evaluator.eval(expr), 123);
                expr.push(Ref<Variant>{"my_variant2"});
                expr.push(OPCode::Command::LOAD);;
                expr.push(Ref<Variant>{456});
                expr.push(OPCode::Command::ASSIGN);;
                expr.push(Ref<Variant>{"my_variant2"});
                expr.push(OPCode::Command::LOAD);;
                Assert(*evaluator.eval(expr), 456);
                expr.push(OPCode::Command::ADD);;
                Assert(*evaluator.eval(expr), 579);

                GroupEnd();
            }
        }
    };

} // namespace test
