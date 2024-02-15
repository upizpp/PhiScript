#pragma once
#include <phi/typedef.hpp>
#include <phi/variant.hpp>

namespace phi {
    Ref<Variant> import(const string &module_name, const string &chunk_name);
} // namespace phi
