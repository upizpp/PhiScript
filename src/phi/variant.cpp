#include "variant.hpp"
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <phi/exception.hpp>
#include <phi/function.hpp>
#include <phi/object.hpp>
#include <phi/type.hpp>
#include <sstream>

namespace phi
{
    Ref<Variant> Variant::Null = new Variant;
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

    Variant::Variant(Owner<array> &&value) : _M_type(Type::ARRAY)
    {
        _M_array_P = value.release();
    }

    Variant::Variant(const dict &value) : _M_type(Type::DICTIONARY)
    {
        _M_dict_P = new dict{value};
    }

    Variant::Variant(Owner<dict> &&value) : _M_type(Type::DICTIONARY)
    {
        _M_dict_P = value.release();
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
        free();
        _M_type = value.type();
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

    Variant::operator int() const
    {
        return integer(*this);
    }

    Variant::operator double() const
    {
        return real(*this);
    }

    void Variant::convert(Type target)
    {
        if (type() == target)
            return;
        if (target == Type::NIL)
        {
            _M_type = target;
            free();
            return;
        }
        switch (type())
        {
        case Type::NIL:
            switch (target)
            {
            case Type::INT:
                _M_int = 0;
                break;
            case Type::REAL:
                _M_real = 0.0;
                break;
            case Type::BOOL:
                _M_bool = false;
                break;
            case Type::STRING:
                *_M_string_P = "";
                break;
            case Type::ARRAY:
                _M_array_P = new array;
                break;
            case Type::DICTIONARY:
                _M_dict_P = new dict;
                break;
            default:
                throw ConversionException{type(), target};
            }
            break;
        case Type::INT:
            switch (target)
            {
            case Type::REAL:
                _M_real = _M_int;
                break;
            case Type::BOOL:
                _M_bool = _M_int;
                break;
            case Type::STRING:
                *_M_string_P = std::to_string(_M_int);
                break;
            default:
                throw ConversionException{type(), target};
            }
            break;
        case Type::REAL:
            switch (target)
            {
            case Type::INT:
                _M_int = _M_real;
                break;
            case Type::BOOL:
                _M_bool = _M_real;
                break;
            case Type::STRING:
                *_M_string_P = std::to_string(_M_real);
                break;
            default:
                throw ConversionException{type(), target};
            }
            break;
        case Type::BOOL:
            switch (target)
            {
            case Type::INT:
                _M_int = _M_bool;
                break;
            case Type::REAL:
                _M_real = _M_bool;
                break;
            case Type::STRING:
                *_M_string_P = _M_bool ? "true" : "false";
                break;
            default:
                throw ConversionException{type(), target};
            }
            break;
        case Type::STRING:
            switch (target)
            {
            case Type::INT:
                try
                {
                    _M_int = std::stoull(*_M_string_P);
                }
                catch (const std::invalid_argument &)
                {
                    throw ConversionException("Cannot convert \"%s\" to INT.", _M_string_P->c_str()).setTypes(type(), target);
                }
                break;
            case Type::REAL:
                try
                {
                    _M_real = std::stoull(*_M_string_P);
                }
                catch (const std::invalid_argument &)
                {
                    throw ConversionException("Cannot convert \"%s\" to REAL.", _M_string_P->c_str()).setTypes(type(), target);
                }
                break;
            case Type::BOOL:
                _M_bool = _M_string_P->empty();
                break;
            default:
                throw ConversionException{type(), target};
            }
        case Type::ARRAY:
        {
            switch (target)
            {
            case Type::BOOL:
                _M_bool = _M_array_P->empty();
                break;
            default:
                throw ConversionException{type(), target};
            }
        }
        case Type::DICTIONARY:
        {
            switch (target)
            {
            case Type::BOOL:
                _M_bool = _M_array_P->empty();
                break;
            default:
                throw ConversionException{type(), target};
            }
        }
        case Type::OBJECT:
        {
            *this = _M_obj_P->convert(target);
            break;
        }

        default:
            break;
        }
        _M_type = target;
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
            try
            {
                return std::stoll(*_M_string_P);
            }
            catch (std::invalid_argument)
            {
                return 0;
            }
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
            try
            {
                return std::stod(*_M_string_P);
            }
            catch (std::invalid_argument)
            {
                return 0.0;
            }
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
        std::ostringstream string_os;
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
            string_os << '[';
            bool first = true;
            for (auto &&item : *_M_array_P)
            {
                if (first)
                    first = false;
                else
                    string_os << ", ";
                string_os << (string)(*item);
            }
            string_os << ']';
            return string_os.str();
        }
        case Type::DICTIONARY:
        {
            string_os << '{';
            bool first = true;
            for (auto &&item : *_M_dict_P)
            {
                if (first)
                    first = false;
                else
                    string_os << ", ";
                string_os << (string)(*item.first) << ": " << (string)(*item.second);
            }
            string_os << '}';
            return string_os.str();
        }
        case Type::OBJECT:
            return (string)(*_M_obj_P);
        case Type::FUNCTION:
            return (string)(*_M_func_P);
        }
        return string();
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
        case Type::MAX:
            return "MAX";
        }
        return string();
    }

    string Variant::toString() const
    {
        std::ostringstream string_os;
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
            return '"' + *_M_string_P + '"';
        case Type::ARRAY:
        {
            string_os << '[';
            bool first = true;
            for (auto &&item : *_M_array_P)
            {
                if (first)
                    first = false;
                else
                    string_os << ", ";
                string_os << item->toString();
            }
            string_os << ']';
            return string_os.str();
        }
        case Type::DICTIONARY:
        {
            string_os << '{';
            bool first = true;
            for (auto &&item : *_M_dict_P)
            {
                if (first)
                    first = false;
                else
                    string_os << ", ";
                string_os << item.first->toString() << ": " << item.second->toString();
            }
            string_os << '}';
            return string_os.str();
        }
        case Type::OBJECT:
            return _M_obj_P->toString();
        case Type::FUNCTION:
            return _M_func_P->toString();
        }
        return string();
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

#define COMPARE_IMPL(op)                                      \
    bool Variant::operator op(const int &value) const         \
    {                                                         \
        switch (_M_type)                                      \
        {                                                     \
        case Type::INT:                                       \
            return _M_int op value;                           \
        case Type::REAL:                                      \
            return _M_real op value;                          \
        case Type::BOOL:                                      \
            return _M_bool op value;                          \
        default:                                              \
            throw CompareException(type(), Type::INT);        \
        }                                                     \
    }                                                         \
                                                              \
    bool Variant::operator op(const double &value) const      \
    {                                                         \
        switch (_M_type)                                      \
        {                                                     \
        case Type::INT:                                       \
            return _M_int op value;                           \
        case Type::REAL:                                      \
            return _M_real op value;                          \
        case Type::BOOL:                                      \
            return _M_bool op value;                          \
        default:                                              \
            throw CompareException(type(), Type::REAL);       \
        }                                                     \
    }                                                         \
                                                              \
    bool Variant::operator op(const integer & value) const    \
    {                                                         \
        switch (_M_type)                                      \
        {                                                     \
        case Type::INT:                                       \
            return _M_int op value;                           \
        case Type::REAL:                                      \
            return _M_real op value;                          \
        case Type::BOOL:                                      \
            return _M_bool op value;                          \
        default:                                              \
            throw CompareException(type(), Type::INT);        \
        }                                                     \
    }                                                         \
                                                              \
    bool Variant::operator op(const real & value) const       \
    {                                                         \
        switch (_M_type)                                      \
        {                                                     \
        case Type::INT:                                       \
            return _M_int op value;                           \
        case Type::REAL:                                      \
            return _M_real op value;                          \
        case Type::BOOL:                                      \
            return _M_bool op value;                          \
        default:                                              \
            throw CompareException(type(), Type::REAL);       \
        }                                                     \
    }                                                         \
                                                              \
    bool Variant::operator op(const bool &value) const        \
    {                                                         \
        switch (_M_type)                                      \
        {                                                     \
        case Type::INT:                                       \
            return _M_int op value;                           \
        case Type::REAL:                                      \
            return _M_real op value;                          \
        case Type::BOOL:                                      \
            return _M_bool op value;                          \
        default:                                              \
            throw CompareException(type(), Type::BOOL);       \
        }                                                     \
    }                                                         \
                                                              \
    bool Variant::operator op(const char *value) const        \
    {                                                         \
        switch (_M_type)                                      \
        {                                                     \
        case Type::STRING:                                    \
            return *_M_string_P op value;                     \
        default:                                              \
            throw CompareException(type(), Type::STRING);     \
        }                                                     \
    }                                                         \
                                                              \
    bool Variant::operator op(const string & value) const     \
    {                                                         \
        switch (_M_type)                                      \
        {                                                     \
        case Type::STRING:                                    \
            return *_M_string_P op value;                     \
        default:                                              \
            throw CompareException(type(), Type::STRING);     \
        }                                                     \
    }                                                         \
                                                              \
    bool Variant::operator op(const array & value) const      \
    {                                                         \
        switch (_M_type)                                      \
        {                                                     \
        case Type::ARRAY:                                     \
            return _M_array_P op & value;                     \
        default:                                              \
            throw CompareException(type(), Type::ARRAY);      \
        }                                                     \
    }                                                         \
                                                              \
    bool Variant::operator op(const dict & value) const       \
    {                                                         \
        switch (_M_type)                                      \
        {                                                     \
        case Type::DICTIONARY:                                \
            return _M_dict_P op & value;                      \
        default:                                              \
            throw CompareException(type(), Type::DICTIONARY); \
        }                                                     \
    }                                                         \
                                                              \
    bool Variant::operator op(const Object & value) const     \
    {                                                         \
        switch (_M_type)                                      \
        {                                                     \
        case Type::OBJECT:                                    \
            return *_M_obj_P op value;                        \
        default:                                              \
            throw CompareException(type(), Type::OBJECT);     \
        }                                                     \
    }                                                         \
                                                              \
    bool Variant::operator op(const Function & value) const   \
    {                                                         \
        switch (_M_type)                                      \
        {                                                     \
        case Type::FUNCTION:                                  \
            return _M_func_P op & value;                      \
        default:                                              \
            throw CompareException(type(), Type::FUNCTION);   \
        }                                                     \
    }                                                         \
                                                              \
    bool Variant::operator op(const Variant & value) const    \
    {                                                         \
        if (!isConvertible(type(), value.type()))             \
            return false;                                     \
        switch (_M_type)                                      \
        {                                                     \
        case Type::NIL:                                       \
            return value._M_type op Type::NIL;                \
        case Type::INT:                                       \
            return _M_int op integer(value);                  \
        case Type::REAL:                                      \
            return _M_real op real(value);                    \
        case Type::BOOL:                                      \
            return _M_bool op bool(value);                    \
        case Type::STRING:                                    \
            return *_M_string_P op string(value);             \
        case Type::ARRAY:                                     \
            if (value.type() != Type::ARRAY)                  \
                throw CompareException(type(), value.type()); \
            return _M_array_P op value._M_array_P;            \
        case Type::DICTIONARY:                                \
            if (value.type() != Type::DICTIONARY)             \
                throw CompareException(type(), value.type()); \
            return _M_dict_P op value._M_dict_P;              \
        case Type::OBJECT:                                    \
            return *_M_obj_P op value;                        \
        case Type::FUNCTION:                                  \
            if (value.type() != Type::ARRAY)                  \
                throw CompareException(type(), value.type()); \
            return _M_func_P op value._M_func_P;              \
        default:                                              \
            throw CompareException(type(), value.type());     \
        }                                                     \
    }

    COMPARE_IMPL(==)
    COMPARE_IMPL(!=)
    COMPARE_IMPL(>)
    COMPARE_IMPL(>=)
    COMPARE_IMPL(<)
    COMPARE_IMPL(<=)

#define CALCULATE_IMPL(fname, op)                                  \
    int Variant::operator op(const int &value) const               \
    {                                                              \
        return integer(*this) op value;                            \
    }                                                              \
                                                                   \
    double Variant::operator op(const double &value) const         \
    {                                                              \
        return real(*this) op value;                               \
    }                                                              \
                                                                   \
    integer Variant::operator op(const integer & value) const      \
    {                                                              \
        return integer(*this) op value;                            \
    }                                                              \
                                                                   \
    real Variant::operator op(const real & value) const            \
    {                                                              \
        return real(*this) op value;                               \
    }                                                              \
                                                                   \
    bool Variant::operator op(const bool &value) const             \
    {                                                              \
        return bool(*this) op value;                               \
    }                                                              \
                                                                   \
    string Variant::operator op(const char *value) const           \
    {                                                              \
        return this->toString() op value;                          \
    }                                                              \
                                                                   \
    string Variant::operator op(const string & value) const        \
    {                                                              \
        return this->toString() op value;                          \
    }                                                              \
                                                                   \
    Variant Variant::operator op(const Object & value) const       \
    {                                                              \
        switch (_M_type)                                           \
        {                                                          \
        case Type::OBJECT:                                         \
            return *_M_obj_P op Variant{value};                    \
        default:                                                   \
            throw CalculateException(type(), Type::OBJECT);        \
        }                                                          \
    }                                                              \
                                                                   \
    Variant Variant::operator op(const Variant & value) const      \
    {                                                              \
        return fname(value);                                       \
    }                                                              \
    Variant Variant::fname(const Variant &value) const             \
    {                                                              \
        switch (_M_type)                                           \
        {                                                          \
        case Type::REAL:                                           \
        case Type::INT:                                            \
        {                                                          \
            Type improved = improve(type(), value.type());         \
            Variant v1 = convertTo(improved);                      \
            Variant v2 = value.convertTo(improved);                \
            if (improved == Type::INT)                             \
                return v1.seeAs<integer>() op v2.seeAs<integer>(); \
            else if (improved == Type::REAL)                       \
                return v1.seeAs<real>() op v2.seeAs<real>();       \
            else                                                   \
                throw Exception("Unexpected branches.");           \
        }                                                          \
        case Type::BOOL:                                           \
            return Variant{bool(*this) op bool(value)};            \
        case Type::STRING:                                         \
            return Variant{*_M_string_P op value.toString()};      \
        case Type::OBJECT:                                         \
            return Variant{*_M_obj_P op Variant{value}};           \
        default:                                                   \
            throw CalculateException(type(), value.type());        \
        }                                                          \
    }                                                              \
    int operator op(const int &v1, const Variant &v2)              \
    {                                                              \
        return v1 op integer(v2);                                  \
    }                                                              \
                                                                   \
    double operator op(const double &v1, const Variant &v2)        \
    {                                                              \
        return v1 op real(v2);                                     \
    }                                                              \
                                                                   \
    integer operator op(const integer &v1, const Variant &v2)      \
    {                                                              \
        return v1 op integer(v2);                                  \
    }                                                              \
                                                                   \
    real operator op(const real &v1, const Variant &v2)            \
    {                                                              \
        return v1 op real(v2);                                     \
    }                                                              \
                                                                   \
    bool operator op(const bool &v1, const Variant &v2)            \
    {                                                              \
        return v1 op bool(v2);                                     \
    }                                                              \
                                                                   \
    string operator op(const char *v1, const Variant &v2)          \
    {                                                              \
        return v1 op v2.toString();                                \
    }                                                              \
                                                                   \
    string operator op(const string &v1, const Variant &v2)        \
    {                                                              \
        return v1 op v2.toString();                                \
    }

#define CALCULATE_NUM_IMPL(fname, op)                              \
    int Variant::operator op(const int &value) const               \
    {                                                              \
        return integer(*this) op value;                            \
    }                                                              \
                                                                   \
    double Variant::operator op(const double &value) const         \
    {                                                              \
        return real(*this) op value;                               \
    }                                                              \
                                                                   \
    integer Variant::operator op(const integer & value) const      \
    {                                                              \
        return integer(*this) op value;                            \
    }                                                              \
                                                                   \
    real Variant::operator op(const real & value) const            \
    {                                                              \
        return real(*this) op value;                               \
    }                                                              \
                                                                   \
    bool Variant::operator op(const bool &value) const             \
    {                                                              \
        return bool(*this) op value;                               \
    }                                                              \
                                                                   \
    Variant Variant::operator op(const Object & value) const       \
    {                                                              \
        switch (_M_type)                                           \
        {                                                          \
        case Type::OBJECT:                                         \
            return *_M_obj_P op Variant{value};                    \
        default:                                                   \
            throw CalculateException(type(), Type::OBJECT);        \
        }                                                          \
    }                                                              \
                                                                   \
    Variant Variant::operator op(const Variant & value) const      \
    {                                                              \
        return fname(value);                                       \
    }                                                              \
    Variant Variant::fname(const Variant &value) const             \
    {                                                              \
        switch (_M_type)                                           \
        {                                                          \
        case Type::REAL:                                           \
        case Type::INT:                                            \
        {                                                          \
            Type improved = improve(type(), value.type());         \
            Variant v1 = convertTo(improved);                      \
            Variant v2 = value.convertTo(improved);                \
            if (improved == Type::INT)                             \
                return v1.seeAs<integer>() op v2.seeAs<integer>(); \
            else if (improved == Type::REAL)                       \
                return v1.seeAs<real>() op v2.seeAs<real>();       \
            else                                                   \
                throw Exception("Unexpected branches.");           \
        }                                                          \
        case Type::BOOL:                                           \
            return Variant{bool(*this) op bool(value)};            \
        case Type::OBJECT:                                         \
            return Variant{*_M_obj_P op value};                    \
        default:                                                   \
            throw CalculateException(type(), value.type());        \
        }                                                          \
    }
#define CALCULATE_INT_IMPL(fname, op)                         \
    int Variant::operator op(const int &value) const          \
    {                                                         \
        return integer(*this) op value;                       \
    }                                                         \
                                                              \
    integer Variant::operator op(const integer & value) const \
    {                                                         \
        return integer(*this) op value;                       \
    }                                                         \
                                                              \
    bool Variant::operator op(const bool &value) const        \
    {                                                         \
        return bool(*this) op value;                          \
    }                                                         \
                                                              \
    Variant Variant::operator op(const Object & value) const  \
    {                                                         \
        switch (_M_type)                                      \
        {                                                     \
        case Type::OBJECT:                                    \
            return *_M_obj_P op Variant{value};               \
        default:                                              \
            throw CalculateException(type(), Type::OBJECT);   \
        }                                                     \
    }                                                         \
                                                              \
    Variant Variant::operator op(const Variant & value) const \
    {                                                         \
        return fname(value);                                  \
    }                                                         \
    Variant Variant::fname(const Variant &value) const        \
    {                                                         \
        switch (_M_type)                                      \
        {                                                     \
        case Type::INT:                                       \
            return Variant{integer(*this) op integer(value)}; \
        case Type::BOOL:                                      \
            return Variant{bool(*this) op bool(value)};       \
        case Type::OBJECT:                                    \
            return Variant{*_M_obj_P op value};               \
        default:                                              \
            throw CalculateException(type(), value.type());   \
        }                                                     \
    }

    CALCULATE_IMPL(add, +)
    CALCULATE_NUM_IMPL(sub, -)
    CALCULATE_NUM_IMPL(mul, *)
    CALCULATE_NUM_IMPL(div, /)
    CALCULATE_INT_IMPL(mod, %)
    CALCULATE_INT_IMPL(band, &)
    CALCULATE_INT_IMPL(bor, |)
    CALCULATE_INT_IMPL(bxor, ^)
    CALCULATE_INT_IMPL(land, &&)
    CALCULATE_INT_IMPL(lor, ||)
    CALCULATE_INT_IMPL(lshift, <<)
    CALCULATE_INT_IMPL(rshift, >>)

    real Variant::power(real pow) const
    {
        return std::pow((real)(*this), pow);
    }

    Variant Variant::operator-() const
    {
        switch (_M_type)
        {
        case Type::INT:
            return Variant(-_M_int);
        case Type::REAL:
            return Variant(-_M_real);
        case Type::OBJECT:
            return Variant{-*_M_obj_P};
        default:
            throw UnaryException(type());
        }
    }

    Variant Variant::operator~() const
    {
        switch (_M_type)
        {
        case Type::INT:
            return Variant(~_M_int);
        case Type::OBJECT:
            return Variant(~*_M_obj_P);
        default:
            throw UnaryException(type());
        }
    }

    Variant Variant::operator!() const
    {
        if (type() == Type::OBJECT)
            return Variant{!*_M_obj_P};
        return Variant{!bool(*this)};
    }

    Variant Variant::operator++()
    {
        if (type() == Type::OBJECT)
            return Variant{++*_M_obj_P};
        convert(Type::INT);
        return ++_M_int;
    }
    Variant Variant::operator--()
    {
        if (type() == Type::OBJECT)
            return Variant{++*_M_obj_P};
        convert(Type::INT);
        return --_M_int;
    }

    bool Variant::isConvertible(Type t1, Type t2)
    {
        if (t2 == Type::OBJECT)
            return true;
        switch (t1)
        {
        case Type::INT:
        case Type::REAL:
        case Type::BOOL:
            return t2 == Type::INT || t2 == Type::REAL || t2 == Type::BOOL;
        default:
            return t1 == t2;
        }
        return false;
    }
    bool Variant::equals(const Variant &value)
    {
        if (type() != value.type())
            return false;
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
            if (value.type() != Type::ARRAY)
                throw CompareException(type(), value.type());
            return _M_array_P == value._M_array_P;
        case Type::DICTIONARY:
            if (value.type() != Type::DICTIONARY)
                throw CompareException(type(), value.type());
            return _M_dict_P == value._M_dict_P;
        case Type::OBJECT:
            return *_M_obj_P == value;
        case Type::FUNCTION:
            if (value.type() != Type::FUNCTION)
                throw CompareException(type(), value.type());
            return _M_func_P == value._M_func_P;
        default:
            throw CompareException(type(), value.type());
        }
    }
    Variant Variant::copy() const
    {
        return *this;
    }
    Variant Variant::deepCopy() const
    {
        Variant res;
        switch (_M_type)
        {
        case Type::INT:
            res._M_int = this->_M_int;
            break;
        case Type::REAL:
            res._M_real = this->_M_real;
            break;
        case Type::BOOL:
            res._M_bool = this->_M_bool;
            break;
        case Type::STRING:
            res._M_string_P = new string{*this->_M_string_P};
            break;
        case Type::ARRAY:
            res._M_array_P = new array{_M_array_P->size(), nullptr};
            for (uinteger i = 0; i < _M_array_P->size(); ++i)
                (*res._M_array_P)[i] = new Variant{(*_M_array_P)[i]->deepCopy()};
            break;
        case Type::DICTIONARY:
            res._M_dict_P = new dict;
            for (auto &&pair : *_M_dict_P)
                res._M_dict_P->insert({new Variant{pair.first->deepCopy()}, new Variant{pair.second->deepCopy()}});
            break;
        case Type::OBJECT:
            res._M_obj_P = new Object{*this->_M_obj_P};
            break;
        case Type::FUNCTION:
            res._M_func_P = new Function{*this->_M_func_P};
            break;
        }
        return res;
    }
    Ref<Variant> Variant::call(const array &args)
    {
        switch (type())
        {
        case Type::FUNCTION:
            return _M_func_P->call(args);
        case Type::OBJECT:
            return _M_obj_P->call(args);

        default:
            throw RuntimeException("The variant with the type of (%s) is not callable.", stringifyType(type()).c_str());
        }
    }
    Ref<Variant>& Variant::access(const array &args)
    {
        switch (type())
        {
        case Type::ARRAY:
        {
            if (args.size() == 1 && args[0]->type() == Type::INT)
            {
                return _M_array_P->at((integer)*args[0]);
            }
            // else if (args.size() >= 2 && args.size() <= 3)
            // {
            //     uinteger begin = (integer)args[0];
            //     uinteger end = (integer)args[1];
            //     uinteger step = args.size() == 3 ? (integer)args[2] : 1;
            //     array *res = new array{(uinteger)std::ceil((end - begin) / real(step)), nullptr};
            //     for (uinteger i = begin; i < end; i += step)
            //         (*res)[i - begin] = (*_M_array_P)[i];
            //     return new Variant{std::move(Owner<array>{res})};
            // }
            else
            {
                throw ArgumentRangedException(1, 3, args.size(), __FUNCTION__);
            }
        }
        case Type::DICTIONARY:
        {
            if (args.size() == 1)
                return _M_dict_P->at(args[0]);
            else
                throw ArgumentException(1, args.size(), __FUNCTION__);
        }
        case Type::OBJECT:
            return _M_obj_P->access(args);
        case Type::FUNCTION:
            return _M_func_P->access(args);

        default:
            throw RuntimeException("The variant with the type of (%s) is not accessible.", stringifyType(type()).c_str());
        }
    }
    size_t VariantHash::operator()(const Ref<Variant> &value) const
    {
        return value->hash();
    }
    size_t VariantEqual::operator()(const Ref<Variant> &lhs, const Ref<Variant> &rhs) const
    {
        return *lhs == *rhs;
    }
} // namespace phi