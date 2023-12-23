#pragma once
#include "test.hpp"

namespace test
{
    class TestTest : public Test
    {
    private:
    public:
        virtual void _run()
        {
            GroupBegin("Group1");
            Assert(1, 1);
            Assert(1, 1, "detail test");
            Assert(1, 2, "fail test");
            GroupEnd();
            GroupBegin("Group2");
            GroupEnd();
            GroupBegin("Nested Group: Outer");
            GroupBegin("Nested Group: Inner");
            GroupEnd();
        }
    };

} // namespace test
