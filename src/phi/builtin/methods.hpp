#pragma once
#include "method.hpp"
#include <phi/runtime/block.hpp>

namespace phi
{
    namespace builtin
    {
        void import(Ref<runtime::Block> , const string &module_name, const string &import_name);
    } // namespace builtin
} // namespace phi
