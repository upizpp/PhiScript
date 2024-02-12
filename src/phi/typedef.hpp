#pragma once
#include <cstdint>
#include <string>
#include <vector>
#include <map>
#include <list>
#include <iostream>
#include <unordered_map>
#include <phi/auto_ptr.hpp>

namespace phi
{
    using std::vector;
    using std::map;
    using std::unordered_map;
    using std::string;
    using std::list;

    using std::cout;
    using std::cin;
    using std::endl;

    using integer = int64_t;
    using uinteger = uint64_t;
    using real = long double;
    using byte = unsigned char;
    using char_t = unsigned char;

    using nullptr_t = decltype(nullptr);

    template<typename T>
    using Ref = Reference<T>;
} // namespace phi
