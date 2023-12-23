#include "array.hpp"
#include <phi/variant.hpp>

#include "reflect_impl"

namespace phi
{
    namespace builtin
    {
        Variant ArrayLib::size(array& args)
        {
            CHECK(1);
            return integer(args[0]->size());
        }
        Variant ArrayLib::push_back(array& args)
        {
            CHECK(2);
            args[0]->pushBack(args[1]);
            return *Variant::Null;
        }
    } // namespace builtin
} // namespace phi
