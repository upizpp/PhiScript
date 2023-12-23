#include "os.hpp"
#include <phi/variant.hpp>
#include <cstdlib>

#include "reflect_impl"

namespace phi
{
    namespace builtin
    {
        Variant OSLib::system(array& args)
        {
            CHECK(1);
            return integer(std::system(args[0]->getString()->c_str()));
        }
    } // namespace builtin
} // namespace phi
