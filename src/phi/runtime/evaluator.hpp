#pragma once
#include <phi/runtime/state.hpp>
#include <stack>

namespace phi
{
    class Evaluator
    {
    public:
        using value_stack = std::stack<Ref<Variant>>;

    private:
        Borrower<const State> _M_state;
        list<value_stack> _M_envs;

        uinteger _M_stream;

    public:
        void setup(const State &);

    private:
        void eval();
        void handle(const OPCode &);

        inline void stream_goto(uinteger where) { _M_stream = where - 1; }
        inline value_stack &get_stack() { return _M_envs.back(); }
        inline Ref<Variant> top() { return get_stack().top(); }
        inline Ref<Variant> pop()
        {
            Ref<Variant> tmp = top();
            get_stack().pop();
            return tmp;
        }
    };
} // namespace phi
