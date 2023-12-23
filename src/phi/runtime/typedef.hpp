#pragma once
#include "../reference.hpp"
#include "../variant.hpp"
#include "op_code.hpp"
#include <list>
#include <map>
#include <string>

namespace phi
{
    namespace runtime
    {
        class Block;
        class ExpressionEvaluator;
        class Expression;

        typedef std::map<string, Ref<Variant>> VariantPool;
        typedef std::list<OPCode> OPCodeContainer;
    } // namespace runtime
} // namespace phi
