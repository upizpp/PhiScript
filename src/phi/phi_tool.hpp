#pragma once
#include "data_type.hpp"
#include <concepts>
#include <stack>
#include <string>

namespace phi
{
    template <typename T>
    inline std::string toString(T value)
    {
        return std::to_string(value);
    }

    template <>
    inline std::string toString(char value)
    {
        return std::string(1, value);
    }

    template <typename T>
    inline void clear(std::stack<T> &stack)
    {
        while (!stack.empty())
            stack.pop();
    }
} // namespace phi
