#pragma once
#include <phi/variant.hpp>
#include "test.hpp"

namespace test
{
    class VariantTest : public Test
    {
    private:
    public:
        virtual void _run()
        {
            using namespace phi;
            LOGGER_INSTANCE->setLevel(Logger::Level::TRACE);
            {
                GroupBegin("Initialization and toString() Test");

                Variant v1(5);
                Assert(v1.toString(), "5");
                Variant v2(3.14);
                Assert(v2.toString(), "3.14");
                Variant v3("test");
                Assert(v3.toString(), "\"test\"");
                // Variant v4(array{1, 2, 3, true, false, 3.14, "Hello"});
                // Assert(v4.toString(), "[1, 2, 3, true, false, 3.14, \"Hello\"]");
                // Variant v5(dictionary{
                //     {"abc", 1},
                //     {"def", 2},
                //     {111, 3},
                //     {false, "aaa"}});
                // Assert(v5.toString(), "{111: 3, \"def\": 2, false: \"aaa\", \"abc\": 1}");

                GroupEnd();
            }

            {
                GroupBegin("Hash Test");

                auto &&printHash = [](const Variant &value)
                {
                    cout << value << ": " << value.hash();
                    struct Proxy
                    {
                        void operator()() { cout << endl; }
                    };
                    return Proxy{};
                };

                cout << "================================================================" << endl;
                Variant v1(5);
                printHash(v1)();
                Variant v2(3.14);
                printHash(v2)();
                Variant v3("test");
                printHash(v3)();
                // Variant v4(array{1, 2, 3, true, false, 3.14, "Hello"});
                // printHash(v4)();
                // Variant v5(dictionary{
                //     {"abc", 1},
                //     {"def", 2},
                //     {111, 3},
                //     {false, "aaa"}});
                // printHash(v5)();

                for (integer i = 0; i < 16; i++)
                {
                    integer num = i * 0xffff;
                    cout << num << ": ";
                    cout << COLOR(C_BLUE, "INT: ");
                    cout << Variant(num).hash();
                    cout << "\t";
                    cout << COLOR(C_BLUE, "REAL: ");
                    cout << Variant(real(num)).hash();
                    cout << endl;
                }

                GroupEnd();
            }

            {
                GroupBegin("Calculate");

                Variant v1 = 1;
                Assert(v1 + 1, 1 + 1);
                Variant v2 = 2;
                Assert(v1 + v2, 1 + 2);
                Assert(v1 - 2, 1 - 2);
                Assert(v1 - v2, 1 - 2);
                Assert(v1 * 2, 1 * 2);
                Assert(v1 * v2, 1 * 2);
                Assert(v1 / 2, 1 / 2);
                Assert(v1 / 2.0, 1 / 2.0);
                Assert(v1 & 4, 1 & 4);
                Assert(v1 | 3, 1 | 3);
                Assert(v1 ^ 0xffff, 1 ^ 0xffff);
                Variant v3 = v1 + v2;
                Assert(v3, 3);
                
                GroupEnd();
            }
        }
    };

} // namespace test
