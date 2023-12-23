#include "object.hpp"
#include <cstdlib>
#include <phi/builtin/method.hpp>
#include <phi/builtin/modules/module.hpp>

namespace phi
{
    Variant Object::call(const array &args)
    {
        // TODO Object::CALL
        if (Method *method = dynamic_cast<Method *>(this))
        {
            return method->call(args);
        }
        return Variant::NIL;
    }

    integer Object::hash() const
    {
        if (m_hasher != nullptr)
        {
            integer result = m_hasher(this);
            return result != -1 ? result : -2;
        }
        else
        {
            return -1;
        }
    }

    string Object::toString() const
    {
        if (m_to_string != nullptr)
        {
            return m_to_string(this);
        }
        else
        {
            char *address = new char[64];
            sprintf(address, "%p", this);
            string result = string("Object(") + address + ")";
            delete[] address;
            return result;
        }
    }

    bool Object::toBool() const
    {
        if (m_to_bool != nullptr)
        {
            return m_to_bool(this);
        }
        else
        {
            return true;
        }
    }

    Ref<Variant> Object::access(const array &args, Flag flag)
    {
        if (args.size() == 1)
        {
            string key = *args[0];
            if (hasProperty(key))
                return new Variant{m_properties[key]};
            if (flag == FLAG_MODULE)
            {
                Module* module = reinterpret_cast<Module*>(this);
                return new Variant{module->access(key)};
            }
        }
        return Variant::Null;
    }

    Object &Object::operator=(const Object &obj)
    {
        m_hasher = obj.m_hasher;
        m_to_string = obj.m_to_string;
        m_to_bool = obj.m_to_bool;
        m_properties = obj.m_properties;
        return *this;
    }
} // namespace phi