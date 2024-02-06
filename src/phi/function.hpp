#pragma once
#include <phi/runtime/state.hpp>
#include <phi/typedef.hpp>

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

        // TODO Function::Call
        Ref<Variant> call(array &args) { return Variant::Null; }

        bool operator==(const Function &func) { return this == &func; }

        // TODO: Function::toString()
        string toString()
        {
            return string();
        }
        operator string() { return string(); }
    };
} // namespace phi
