#include "object.hpp"
#include <phi/exception.hpp>
#include <phi/class_db.hpp>
#include <phi/runtime/evaluator.hpp>

namespace phi
{
#undef CLASS
#define CLASS Test
    void Object::static_register()
    {
    }
    void Object::set(const string &property, Ref<Variant> value)
    {
        if (ClassDB::hasProperty(this, property))
            ClassDB::set(this, property, *value);
        else
            _M_properties[property] = value;
    }
    Ref<Variant> Object::get(const string &property)
    {
        return new Variant{ClassDB::get(this, property)};
    }
    VariantPacker Object::access(const array &args)
    {
        if (args.size() == 1 && args[0]->type() == Variant::Type::STRING)
        {
            string str = *args[0];
            if (ClassDB::hasProperty(this, str))
                return {new Variant{get(str)}, str};
            if (_M_properties.find(str) == _M_properties.end())
                _M_properties[str] = new Variant;
            return _M_properties[str];
        }
        else
        {
            throw ArgumentException(1, args.size(), __FUNCTION__);
        }
    }
} // namespace phi
