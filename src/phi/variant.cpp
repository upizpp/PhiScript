#include "variant.hpp"
#include <phi/function.hpp>
#include <phi/object.hpp>

namespace phi
{
    Variant::Variant() : _M_type(Type::NIL)
    {
    }

    Variant::Variant(const int &value) : Variant((integer)value)
    {
    }

    Variant::Variant(const double &value) : Variant((real)value)
    {
    }

    Variant::Variant(const char *value) : Variant(string{value})
    {
    }

    Variant::Variant(const integer &value) : _M_type(Type::INT)
    {
        _M_int = value;
    }

    Variant::Variant(const real &value) : _M_type(Type::REAL)
    {
        _M_real = value;
    }

    Variant::Variant(const bool &value) : _M_type(Type::BOOL)
    {
        _M_bool = value;
    }

    Variant::Variant(const string &value) : _M_type(Type::STRING)
    {
        _M_string_P = new string{value};
    }

    Variant::Variant(const array &value) : _M_type(Type::ARRAY)
    {
        _M_array_P = new array{value};
    }

    Variant::Variant(const dict &value) : _M_type(Type::DICTIONARY)
    {
        _M_dict_P = new dict{value};
    }

    Variant::Variant(const Object &value) : _M_type(Type::OBJECT)
    {
        _M_obj_P = new Object{value};
    }

    Variant::Variant(const Function &value) : _M_type(Type::FUNCTION)
    {
        _M_func_P = new Function{value};
    }

    Variant &Variant::operator=(const int &value)
    {
        free();
        _M_int = value;
        return *this;
    }

    Variant &Variant::operator=(const double &value)
    {
        free();
        _M_real = value;
        return *this;
    }

    Variant &Variant::operator=(const integer &value)
    {
        free();
        _M_int = value;
        return *this;
    }

    Variant &Variant::operator=(const real &value)
    {
        free();
        _M_real = value;
        return *this;
    }

    Variant &Variant::operator=(const bool &value)
    {
        free();
        _M_bool = value;
        return *this;
    }

    Variant &Variant::operator=(const char *value)
    {
        free();
        _M_string_P = new string{value};
        return *this;
    }

    Variant &Variant::operator=(const string &value)
    {
        free();
        _M_string_P = new string{value};
        return *this;
    }

    Variant &Variant::operator=(const array &value)
    {
        free();
        _M_array_P = new array{value};
        return *this;
    }

    Variant &Variant::operator=(const dict &value)
    {
        free();
        _M_dict_P = new dict{value};
        return *this;
    }

    Variant &Variant::operator=(const Object &value)
    {
        free();
        _M_obj_P = new Object{value};
        return *this;
    }

    Variant &Variant::operator=(const Function &value)
    {
        free();
        _M_func_P = new Function{value};
        return *this;
    }

    Variant::operator Object()
    {
        return *_M_obj_P;
    }

    Variant::operator Function()
    {
        return *_M_func_P;
    }

    void Variant::free()
    {
        switch (_M_type)
        {
        case Type::STRING:
            delete _M_string_P;
        case Type::ARRAY:
            delete _M_array_P;
        case Type::DICTIONARY:
            delete _M_dict_P;
        case Type::OBJECT:
            delete _M_obj_P;
        case Type::FUNCTION:
            delete _M_func_P;
        }
    }

} // namespace phi
