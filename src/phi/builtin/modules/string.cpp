#include "string.hpp"
#include <phi/variant.hpp>

#include "reflect_impl"
#include <phi/builtin/algorithms/string.hpp>

namespace phi
{
    namespace builtin
    {
        Variant StringLib::is_integer(array& args)
        {
            CHECK(1);
            return algorithm::isNumber(*args[0]);
        }

        Variant StringLib::is_number(array& args)
        {
            CHECK(1);
            return algorithm::isNumber(*args[0]);
        }
    } // namespace builtin
} // namespace phi
