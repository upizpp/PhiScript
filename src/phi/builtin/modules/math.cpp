#include "math.hpp"
#include <cmath>
#include <phi/variant.hpp>

#include "reflect_impl"

namespace phi
{
    namespace builtin
    {
        Variant MathLib::sqrt(array& args)
        {
            CHECK(1);
            return std::sqrt(real(*args[0]));
        }
    } // namespace builtin
} // namespace phi
