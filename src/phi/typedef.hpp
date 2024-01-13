#pragma once
#include <cstdint>
#include <string>
#include <vector>
#include <phi/auto_ptr.hpp>

namespace phi
{
    using std::vector;
    using std::string;

    using integer = int64_t;
    using uinteger = int64_t;
    using real = long double;

    template<typename T>
    using Ref = Reference<T>;
} // namespace phi
