#pragma once
#include <phi/typedef.hpp>
#include <phi/runtime/state.hpp>

namespace phi
{
    class Function
    {
    private:
        Ref<State> _M_state;
    public:
        Function() {}
        Function(const Function &) = default;
        ~Function() {}

        bool operator==(const Function &func) { return this == &func; }

        // TODO: Function::toString()
        string toString()
        {
            return string();
        }
        operator string() { return string(); }
    };
} // namespace phi
