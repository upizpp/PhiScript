#include "variant.hpp"
#include <cstdlib>
#include <ctime>
#include <phi/exception.hpp>
#include <phi/function.hpp>
#include <phi/object.hpp>
#include <sstream>

namespace phi
{
    static std::ostringstream string_os;
    uinteger Variant::hashSeed = (srand(time(0)), rand());

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

    Variant::Variant(const Variant &value) : _M_type(value.type())
    {
        switch (_M_type)
        {
        case Type::INT:
            _M_int = value._M_int;
            break;
        case Type::REAL:
            _M_real = value._M_real;
            break;
        case Type::BOOL:
            _M_bool = value._M_bool;
            break;
        case Type::STRING:
            _M_string_P = new string{*value._M_string_P};
            break;
        case Type::ARRAY:
            _M_array_P = new array{*value._M_array_P};
            break;
        case Type::DICTIONARY:
            _M_dict_P = new dict{*value._M_dict_P};
            break;
        case Type::OBJECT:
            _M_obj_P = new Object{*value._M_obj_P};
            break;
        case Type::FUNCTION:
            _M_func_P = new Function{*value._M_func_P};
            break;
        }
    }

    Variant::Variant(Variant &&value) : _M_type(value.type())
    {
        value._M_type = Type::NIL;
        switch (_M_type)
        {
        case Type::INT:
            _M_int = value._M_int;
            break;
        case Type::REAL:
            _M_real = value._M_real;
            break;
        case Type::BOOL:
            _M_bool = value._M_bool;
            break;
        case Type::STRING:
            _M_string_P = value._M_string_P;
            value._M_string_P = nullptr;
            break;
        case Type::ARRAY:
            _M_array_P = value._M_array_P;
            value._M_array_P = nullptr;
            break;
        case Type::DICTIONARY:
            _M_dict_P = value._M_dict_P;
            value._M_dict_P = nullptr;
            break;
        case Type::OBJECT:
            _M_obj_P = value._M_obj_P;
            value._M_obj_P = nullptr;
            break;
        case Type::FUNCTION:
            _M_func_P = value._M_func_P;
            value._M_func_P = nullptr;
            break;
        }
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

    Variant &Variant::operator=(const Variant &value)
    {
        _M_type = value.type();
        free();
        switch (value.type())
        {
        case Type::INT:
            _M_int = value._M_int;
            break;
        case Type::REAL:
            _M_real = value._M_real;
            break;
        case Type::BOOL:
            _M_bool = value._M_bool;
            break;
        case Type::STRING:
            _M_string_P = new string{*value._M_string_P};
            break;
        case Type::ARRAY:
            _M_array_P = new array{*value._M_array_P};
            break;
        case Type::DICTIONARY:
            _M_dict_P = new dict{*value._M_dict_P};
            break;
        case Type::OBJECT:
            _M_obj_P = new Object{*value._M_obj_P};
            break;
        case Type::FUNCTION:
            _M_func_P = new Function{*value._M_func_P};
            break;
        }
        return *this;
    }

    Variant &Variant::operator=(Variant &&value)
    {
        _M_type = value.type();
        free();
        switch (value.type())
        {
        case Type::INT:
            _M_int = value._M_int;
            break;
        case Type::REAL:
            _M_real = value._M_real;
            break;
        case Type::BOOL:
            _M_bool = value._M_bool;
            break;
        case Type::STRING:
            _M_string_P = value._M_string_P;
            value._M_string_P = nullptr;
            break;
        case Type::ARRAY:
            _M_array_P = value._M_array_P;
            value._M_array_P = nullptr;
            break;
        case Type::DICTIONARY:
            _M_dict_P = value._M_dict_P;
            value._M_dict_P = nullptr;
            break;
        case Type::OBJECT:
            _M_obj_P = value._M_obj_P;
            value._M_obj_P = nullptr;
            break;
        case Type::FUNCTION:
            _M_func_P = value._M_func_P;
            value._M_func_P = nullptr;
            break;
        }
        return *this;
    }

    Variant::operator integer() const
    {
        switch (_M_type)
        {
        case Type::NIL:
            return 0;
        case Type::INT:
            return _M_int;
        case Type::REAL:
            return _M_real;
        case Type::BOOL:
            return _M_bool;
        case Type::STRING:
            return std::stoll(*_M_string_P);
        case Type::OBJECT:
            return *_M_obj_P;
        default:
            throw ConversionException(type(), Type::INT);
        }
    }

    Variant::operator real() const
    {
        switch (_M_type)
        {
        case Type::NIL:
            return 0.0;
        case Type::INT:
            return (real)_M_int;
        case Type::REAL:
            return _M_real;
        case Type::BOOL:
            return _M_bool ? 1.0 : 0.0;
        case Type::STRING:
            return std::stod(*_M_string_P);
        case Type::OBJECT:
            return *_M_obj_P;
        default:
            throw ConversionException(type(), Type::REAL);
        }
    }

    Variant::operator bool() const
    {
        switch (_M_type)
        {
        case Type::NIL:
            return false;
        case Type::INT:
            return _M_int != 0;
        case Type::REAL:
            return _M_real != 0.0;
        case Type::BOOL:
            return _M_bool;
        case Type::STRING:
            return *_M_string_P != "";
        case Type::ARRAY:
            return !_M_array_P->empty();
        case Type::DICTIONARY:
            return !_M_dict_P->empty();
        case Type::OBJECT:
            return *_M_obj_P;
        case Type::FUNCTION:
            return _M_func_P != nullptr;
        default:
            throw ConversionException(type(), Type::BOOL);
        }
    }

    Variant::operator string() const
    {
        string_os.str("");
        switch (_M_type)
        {
        case Type::NIL:
            return "null";
        case Type::INT:
            return std::to_string(_M_int);
        case Type::REAL:
            return std::to_string(_M_real);
        case Type::BOOL:
            return _M_bool ? "true" : "false";
        case Type::STRING:
            return '\"' + *_M_string_P + '\"';
        case Type::ARRAY:
        {
            bool first = true;
            for (auto &&item : *_M_array_P)
            {
                if (first)
                    first = false;
                else
                    string_os << ", ";
                string_os << (string)(*item);
            }
            return string_os.str();
        }
        case Type::DICTIONARY:
        {
            bool first = true;
            for (auto &&item : *_M_dict_P)
            {
                if (first)
                    first = false;
                else
                    string_os << ", ";
                string_os << (string)(*item.first) << ": " << (string)(*item.second);
            }
            return string_os.str();
        }
        case Type::OBJECT:
            return (string)(*_M_obj_P);
        case Type::FUNCTION:
            return (string)(*_M_func_P);
        }
    }

    Variant::operator array() const
    {
        if (type() != Type::ARRAY)
            throw ConversionException(type(), Type::ARRAY);
        return *_M_array_P;
    }

    Variant::operator dict() const
    {
        if (type() != Type::DICTIONARY)
            throw ConversionException(type(), Type::DICTIONARY);
        return *_M_dict_P;
    }

    Variant::operator Object() const
    {
        if (type() != Type::OBJECT)
            throw ConversionException(type(), Type::OBJECT);
        return *_M_obj_P;
    }

    Variant::operator Function() const
    {
        if (type() != Type::FUNCTION)
            throw ConversionException(type(), Type::FUNCTION);
        return *_M_func_P;
    }

    void Variant::free()
    {
        switch (_M_type)
        {
        case Type::STRING:
            delete _M_string_P;
            break;
        case Type::ARRAY:
            delete _M_array_P;
            break;
        case Type::DICTIONARY:
            delete _M_dict_P;
            break;
        case Type::OBJECT:
            delete _M_obj_P;
            break;
        case Type::FUNCTION:
            delete _M_func_P;
            break;
        default:
            break;
        }
    }

    string Variant::stringifyType(Type type)
    {
        switch (type)
        {
        case Type::NIL:
            return "NULL";
        case Type::INT:
            return "INT";
        case Type::REAL:
            return "REAL";
        case Type::BOOL:
            return "BOOL";
        case Type::STRING:
            return "STRING";
        case Type::ARRAY:
            return "ARRAY";
        case Type::DICTIONARY:
            return "DICTIONARY";
        case Type::OBJECT:
            return "OBJECT";
        case Type::FUNCTION:
            return "FUNCTION";
        }
    }

    string Variant::toString() const
    {
        string_os.str("");
        switch (_M_type)
        {
        case Type::NIL:
            return "null";
        case Type::INT:
            return std::to_string(_M_int);
        case Type::REAL:
            return std::to_string(_M_real);
        case Type::BOOL:
            return _M_bool ? "true" : "false";
        case Type::STRING:
            return *_M_string_P;
        case Type::ARRAY:
        {
            bool first = true;
            for (auto &&item : *_M_array_P)
            {
                if (first)
                    first = false;
                else
                    string_os << ", ";
                string_os << item->toString();
            }
            return string_os.str();
        }
        case Type::DICTIONARY:
        {
            bool first = true;
            for (auto &&item : *_M_dict_P)
            {
                if (first)
                    first = false;
                else
                    string_os << ", ";
                string_os << item.first->toString() << ": " << item.second->toString();
            }
            return string_os.str();
        }
        case Type::OBJECT:
            return _M_obj_P->toString();
        case Type::FUNCTION:
            return _M_func_P->toString();
        }
    }

    uinteger Variant::hash() const
    {
        uinteger value = 0;
        switch (_M_type)
        {
        case Type::NIL:
            value = (uinteger)(-1);
            break;
        case Type::INT:
            value = _M_int;
            break;
        case Type::REAL:
            value = (uinteger)_M_real;
            break;
        case Type::BOOL:
            value = _M_bool;
            break;
        case Type::STRING:
            value = std::hash<std::string>()(*_M_string_P);
            break;
        case Type::ARRAY:
            for (uinteger i = 0; i < _M_array_P->size(); ++i)
                value ^= (*_M_array_P)[i]->hash() << i;
            break;
        case Type::DICTIONARY:
        {
            uinteger i = 0;
            for (auto &&item : *_M_dict_P)
            {
                value ^= (item.first->hash() ^ item.second->hash()) << i;
                ++i;
            }
        }
        case Type::OBJECT:
            value = _M_obj_P->hash();
        case Type::FUNCTION:
            value = (uinteger)_M_func_P;
        }
        return value ^ hashSeed;
    }

    bool Variant::operator==(const int &value) const
    {
        switch (_M_type)
        {
        case Type::INT:
            return _M_int == value;
        case Type::REAL:
            return _M_real == value;
        case Type::BOOL:
            return _M_bool == value;
        default:
            return false;
        }
    }

    bool Variant::operator==(const double &value) const
    {
        switch (_M_type)
        {
        case Type::INT:
            return _M_int == value;
        case Type::REAL:
            return _M_real == value;
        case Type::BOOL:
            return _M_bool == value;
        default:
            return false;
        }
    }

    bool Variant::operator==(const integer &value) const
    {
        switch (_M_type)
        {
        case Type::INT:
            return _M_int == value;
        case Type::REAL:
            return _M_real == value;
        case Type::BOOL:
            return _M_bool == value;
        default:
            return false;
        }
    }

    bool Variant::operator==(const real &value) const
    {
        switch (_M_type)
        {
        case Type::INT:
            return _M_int == value;
        case Type::REAL:
            return _M_real == value;
        case Type::BOOL:
            return _M_bool == value;
        default:
            return false;
        }
    }

    bool Variant::operator==(const bool &value) const
    {
        switch (_M_type)
        {
        case Type::INT:
            return _M_int == value;
        case Type::REAL:
            return _M_real == value;
        case Type::BOOL:
            return _M_bool == value;
        default:
            return false;
        }
    }

    bool Variant::operator==(const char *value) const
    {
        switch (_M_type)
        {
        case Type::STRING:
            return *_M_string_P == value;
        default:
            return false;
        }
    }

    bool Variant::operator==(const string &value) const
    {
        switch (_M_type)
        {
        case Type::STRING:
            return *_M_string_P == value;
        default:
            return false;
        }
    }

    bool Variant::operator==(const array &value) const
    {
        switch (_M_type)
        {
        case Type::ARRAY:
            return _M_array_P == &value;
        default:
            return false;
        }
    }

    bool Variant::operator==(const dict &value) const
    {
        switch (_M_type)
        {
        case Type::DICTIONARY:
            return _M_dict_P == &value;
        default:
            return false;
        }
    }

    bool Variant::operator==(const Object &value) const
    {
        switch (_M_type)
        {
        case Type::OBJECT:
            return *_M_obj_P == value;
        default:
            return false;
        }
    }

    bool Variant::operator==(const Function &value) const
    {
        switch (_M_type)
        {
        case Type::FUNCTION:
            return _M_func_P == &value;
        default:
            return false;
        }
    }

    bool Variant::operator==(const Variant &value) const
    {
        switch (_M_type)
        {
        case Type::NIL:
            return value._M_type == Type::NIL;
        case Type::INT:
            return _M_int == integer(value);
        case Type::REAL:
            return _M_real == real(value);
        case Type::BOOL:
            return _M_bool == bool(value);
        case Type::STRING:
            return *_M_string_P == string(value);
        case Type::ARRAY:
            return *_M_array_P == array(value);
        case Type::DICTIONARY:
            return *_M_dict_P == dict(value);
        case Type::OBJECT:
            return *_M_obj_P == Object(value);
        case Type::FUNCTION:
            return *_M_func_P == Function(value);
        default:
            return false;
        }
    }

} // namespace phi
