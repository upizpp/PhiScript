#include "object.hpp"

namespace phi
{
    void Object::static_register()
    {
#undef CLASS
#define CLASS Test
    }
} // namespace phi
