#pragma once
#include <phi/typedef.hpp>

namespace phi
{
    class Function
    {
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
