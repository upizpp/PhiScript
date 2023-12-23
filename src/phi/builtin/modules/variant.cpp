#include "variant.hpp"
#include <iostream>
#include <phi/exception.hpp>
#include <phi/function.hpp>
#include <phi/variant.hpp>

#include "reflect_impl"

namespace phi
{
    namespace builtin
    {
        Variant VariantLib::convert(array &args)
        {
            CHECK(2);
            Ref<Variant> value = args[0];
            Variant::Type type = Variant::Type(integer(*args[1]));
            switch (type)
            {
            case Variant::Type::NIL:
                return *Variant::Null;
            case Variant::Type::INT:
                return integer(*value);
            case Variant::Type::REAL:
                return real(*value);
            case Variant::Type::BOOL:
                return bool(*value);
            case Variant::Type::STRING:
                return string(*value);
            case Variant::Type::ARRAY:
                return array(*value);
            case Variant::Type::LIST:
                return list(*value);
            case Variant::Type::DICTIONARY:
                return dictionary(*value);
            case Variant::Type::FUNCTION:
                return Function(*value);
            case Variant::Type::OBJECT:
                return Object(*value);
            default:
                return Variant::NIL;
            }
        }

        Variant VariantLib::type(array &args)
        {
            CHECK(1);
            return args[0]->type();
        }

        Variant VariantLib::type_s(array &args)
        {
            CHECK(1);
            return Variant::stringifyType(args[0]->type());
        }

        Variant VariantLib::stringify_type(array &args)
        {
            CHECK(1);
            return Variant::stringifyType(Variant::Type(integer(*args[0])));
        }
    } // namespace builtin
} // namespace phi
