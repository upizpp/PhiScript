#pragma once
#include <cstdint>
#include <iostream>
#include <list>
#include <map>
#include <phi/auto_ptr.hpp>
#include <string>
#include <unordered_map>
#include <vector>

namespace phi {
    using std::list;
    using std::map;
    using std::string;
    using std::unordered_map;
    using std::vector;

    using std::cin;
    using std::cout;
    using std::endl;

    using integer = int64_t;
    using uinteger = uint64_t;
    using real = long double;
    using byte = unsigned char;
    using char_t = unsigned char;

    using nullptr_t = decltype(nullptr);

    template <typename T> using Ref = Reference<T>;
} // namespace phi
