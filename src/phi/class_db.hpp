#pragma once

#include <functional>
#include <phi/exception.hpp>
#include <phi/typedef.hpp>
#include <phi/variant.hpp>
#include <phi/traits.hpp>

#define PhiClass(class_name, parent_class)

namespace phi
{
    struct ClassInfo
    {
        using type = Variant;
        using arg_list = vector<type>;
        using method = std::function<type(arg_list &)>;

        map<string, method> methods;
        map<string, size_t> properties;
    };

    class ClassDB
    {
    private:
    };
} // namespace phi
