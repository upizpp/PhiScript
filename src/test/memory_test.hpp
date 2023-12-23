#pragma once
#include <phi/reference.hpp>
#include "test.hpp"

namespace test
{
    class MemoryTest : public Test
    {
    private:
    public:
        virtual void _run()
        {
            using namespace phi;
            auto &&pool = Variant::Memory::instance();

            LOGGER_INSTANCE->setLevel(Logger::Level::TRACE);
            {
                GroupBegin("Memory Extract and Free");

                Variant *value = pool->extract(5);
                pool->reference(value);
                pool->unreference(value);
                pool->unreference(value);

                {
                    Ref<Variant> i(999);
                    Ref<Variant> s("HELLO WORLD HELLO WORLD HELLO WORLD HELLO WORLD HELLO WORLD HELLO WORLD HELLO WORLD HELLO WORLD");
                }

                GroupEnd();
            }

            {
                GroupBegin("Container Reference");

                {
                    Ref<Variant> array(Variant::Type::ARRAY);
                    array->pushBack(111);
                    array->pushBack(222);
                    array->pushBack(333);
                    array->pushBack("Hello");
                    array->pushBack(true);
                    array->pushBack(false);
                    array->pushBack(makeArray({1, 2, 3}));
                    array->pushBack(makeList({4, 5, 6}));
                    Assert(array->toString(), "[111, 222, 333, \"Hello\", true, false, [1, 2, 3], [4, 5, 6]]", "Vector Based.");
                }

                {
                    Ref<Variant> array(Variant::Type::LIST);
                    array->pushBack(111);
                    array->pushBack(222);
                    array->pushBack(333);
                    array->pushBack("Hello");
                    array->pushBack(true);
                    array->pushBack(false);
                    array->pushBack(makeArray({1, 2, 3}));
                    array->pushBack(makeList({4, 5, 6}));
                    Assert(array->toString(), "[111, 222, 333, \"Hello\", true, false, [1, 2, 3], [4, 5, 6]]", "List Based.");
                }

                {
                    Ref<Variant> dict(makeDict({
                        {"a", 5},
                        {"b", 10},
                        {"c", 15},
                        {123, false},
                        {true, 3.1415},
                        {makeArray({1, 2, 3, 4}), "It's working!"},
                    }));
                    Assert(dict->toString(), "{\"a\": 5, \"b\": 10, \"c\": 15, 123: false, true: 3.1415, [1, 2, 3, 4]: \"It's working!\"}", "Dictionary Based.");
                    dict->setProperty("b", Variant::Memory::instance()->create("Hello world!"));
                    Assert(dict->toString(), "{\"a\": 5, \"b\": \"Hello world!\", \"c\": 15, 123: false, true: 3.1415, [1, 2, 3, 4]: \"It's working!\"}", "Dictionary Based.");
                }

                GroupEnd();
            }
        }
    };

} // namespace test
