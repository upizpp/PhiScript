#include "type.hpp"

namespace phi
{
    bool numeric(Variant::Type type)
    {
        return type == Variant::Type::INT ||
               type == Variant::Type::REAL ||
               type == Variant::Type::BOOL;
    }

    Variant::Type improve(Variant::Type t1, Variant::Type t2)
    {
        if (!(numeric(t1) && numeric(t2)))
            return Variant::Type::NIL;
        if (t1 == Variant::Type::REAL || t2 == Variant::Type::REAL)
            return Variant::Type::REAL;
        if (t1 == Variant::Type::INT || t2 == Variant::Type::INT)
            return Variant::Type::INT;
        return Variant::Type::BOOL;
    }
} // namespace phi