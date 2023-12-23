#include "variant.hpp"
#include "exception.hpp"
#include "function.hpp"
#include "object.hpp"
#include "phi_entry.hpp"
#include "reference.hpp"
#include "singleton.hpp"
#include <algorithm>
#include <cmath>
#include <phi/builtin/algorithms/string.hpp>
#include <phi/builtin/modules/modules>
#include <sstream>
#include <unordered_set>

#define OPERATOR(fname, op)                                                         \
    const Variant Variant::fname(const Variant &value) const                        \
    {                                                                               \
        switch (type())                                                             \
        {                                                                           \
        case Type::INT:                                                             \
        {                                                                           \
            switch (value.type())                                                   \
            {                                                                       \
            case Type::INT:                                                         \
                return *getInteger() op * value.getInteger();                       \
            case Type::REAL:                                                        \
                return *getInteger() op * value.getReal();                          \
            default:                                                                \
                throw OperandException(std::vector<const Variant *>{this, &value}); \
            }                                                                       \
            break;                                                                  \
        }                                                                           \
        case Type::REAL:                                                            \
        {                                                                           \
            switch (value.type())                                                   \
            {                                                                       \
            case Type::INT:                                                         \
                return *getReal() op * value.getInteger();                          \
            case Type::REAL:                                                        \
                return *getReal() op * value.getReal();                             \
            default:                                                                \
                throw OperandException(std::vector<const Variant *>{this, &value}); \
            }                                                                       \
            break;                                                                  \
        }                                                                           \
        default:                                                                    \
            throw OperandException(std::vector<const Variant *>{this, &value});     \
        }                                                                           \
        return Variant();                                                           \
    }
#define OPERATOR_I(fname, op)                                                       \
    const Variant Variant::fname(const Variant &value) const                        \
    {                                                                               \
        switch (type())                                                             \
        {                                                                           \
        case Type::INT:                                                             \
        {                                                                           \
            switch (value.type())                                                   \
            {                                                                       \
            case Type::INT:                                                         \
                return *getInteger() op * value.getInteger();                       \
            default:                                                                \
                throw OperandException(std::vector<const Variant *>{this, &value}); \
            }                                                                       \
            break;                                                                  \
        }                                                                           \
        default:                                                                    \
            throw OperandException(std::vector<const Variant *>{this, &value});     \
        }                                                                           \
        return Variant();                                                           \
    }
#define OPERATOR_S(fname, op)                                                       \
    const Variant Variant::fname(const Variant &value) const                        \
    {                                                                               \
        switch (type())                                                             \
        {                                                                           \
        case Type::INT:                                                             \
        {                                                                           \
            switch (value.type())                                                   \
            {                                                                       \
            case Type::INT:                                                         \
                return *getInteger() op * value.getInteger();                       \
            case Type::REAL:                                                        \
                return *getInteger() op * value.getReal();                          \
            case Type::STRING:                                                      \
                return toString() op * value.getString();                           \
            default:                                                                \
                throw OperandException(std::vector<const Variant *>{this, &value}); \
            }                                                                       \
            break;                                                                  \
        }                                                                           \
        case Type::REAL:                                                            \
        {                                                                           \
            switch (value.type())                                                   \
            {                                                                       \
            case Type::INT:                                                         \
                return *getReal() op * value.getInteger();                          \
            case Type::REAL:                                                        \
                return *getReal() op * value.getReal();                             \
            case Type::STRING:                                                      \
                return toString() op * value.getString();                           \
            default:                                                                \
                throw OperandException(std::vector<const Variant *>{this, &value}); \
            }                                                                       \
            break;                                                                  \
        }                                                                           \
        case Type::STRING:                                                          \
        {                                                                           \
            switch (value.type())                                                   \
            {                                                                       \
            case Type::STRING:                                                      \
                return *getString() op * value.getString();                         \
            default:                                                                \
                return *getString() op value.toString();                            \
            }                                                                       \
        }                                                                           \
        default:                                                                    \
            throw OperandException(std::vector<const Variant *>{this, &value});     \
        }                                                                           \
        return Variant();                                                           \
    }

namespace phi
{
    Ref<Variant> Variant::Null(new Variant(Variant::Type::NIL));

    Variant::Variant()
    {
        m_type = Type::NIL;
        LTRACE_F("INITIALIZE %p, content: %s.", this, toString().c_str());
    }

    Variant::Variant(integer value) : m_type(Type::INT)
    {
        m_data = new integer;
        *get<integer>() = value;
        LTRACE_F("INITIALIZE %p, content: %s.", this, toString().c_str());
    }

    Variant::Variant(real value) : m_type(Type::REAL)
    {
        m_data = new real;
        *get<real>() = value;
        LTRACE_F("INITIALIZE %p, content: %s.", this, toString().c_str());
    }

    Variant::Variant(bool value) : m_type(Type::BOOL)
    {
        m_data = new bool;
        *get<bool>() = value;
        LTRACE_F("INITIALIZE %p, content: %s.", this, toString().c_str());
    }

    Variant::Variant(const char *value) : Variant(string(value))
    {
    }

    Variant::Variant(const string &value) : m_type(Type::STRING)
    {
        m_data = new string;
        *get<string>() = value;
        LTRACE_F("INITIALIZE %p, content: %s.", this, toString().c_str());
    }

    Variant::Variant(const array &value) : m_type(Type::ARRAY)
    {
        m_data = new array;
        *get<array>() = value;
        LTRACE_F("INITIALIZE %p, content: %s.", this, toString().c_str());
    }

    Variant::Variant(const list &value) : m_type(Type::LIST)
    {
        m_data = new list;
        *get<list>() = value;
        LTRACE_F("INITIALIZE %p, content: %s.", this, toString().c_str());
    }

    Variant::Variant(const dictionary &value) : m_type(Type::DICTIONARY)
    {
        m_data = new dictionary;

        *get<dictionary>() = value;
        LTRACE_F("INITIALIZE %p, content: %s.", this, toString().c_str());
    }

    Variant::Variant(const Object &value) : m_type(Type::OBJECT)
    {
        m_data = new Object;
        *get<Object>() = value;
        LTRACE_F("INITIALIZE %p, content: %s.", this, toString().c_str());
    }

    Variant::Variant(Object &&value) : m_type(Type::OBJECT)
    {
        m_data = new Object;
        *get<Object>() = std::move(value);
        LTRACE_F("INITIALIZE %p, content: %s.", this, toString().c_str());
    }

    Variant::Variant(Object *value) : m_type((Type::OBJECT))
    {
        m_data = value;
        m_is_borrowed = true;
        LTRACE_F("INITIALIZE %p, content: %s.", this, toString().c_str());
    }

    Variant::Variant(const Function &value) : m_type(Type::FUNCTION)
    {
        m_data = new Function(value);
        LTRACE_F("INITIALIZE %p, content: %s.", this, toString().c_str());
    }

    Variant::Variant(void *ptr, Flag flag): m_type(Type::ANY)
    {
        m_data = ptr;
        m_flag = flag;
    }

    Variant::Variant(const Variant &value)
    {
        m_is_borrowed = value.m_is_borrowed;
        m_flag = value.m_flag;
        changeType(value.type());
        switch (m_type)
        {
        case Type::INT:
            *get<integer>() = *value.get<integer>();
            break;
        case Type::REAL:
            *get<real>() = *value.get<real>();
            break;
        case Type::BOOL:
            *get<bool>() = *value.get<bool>();
            break;
        case Type::STRING:
            *get<string>() = *value.get<string>();
            break;
        case Type::ARRAY:
        {
            *get<array>() = *value.get<array>();
            break;
        }
        case Type::LIST:
        {
            *get<list>() = *value.get<list>();
            break;
        }
        case Type::DICTIONARY:
        {
            *get<dictionary>() = *value.get<dictionary>();
            break;
        }
        case Type::OBJECT:
            *get<Object>() = *value.get<Object>();
            break;
        case Type::FUNCTION:
            *get<Function>() = *value.get<Function>();
            break;
        default:
            break;
        }
        LTRACE_F("INITIALIZE %p, content: %s.", this, toString().c_str());
    }

    Variant::Variant(Variant &&value)
    {
        m_is_borrowed = value.m_is_borrowed;
        m_flag = value.m_flag;
        m_type = value.type();
        m_data = value.m_data;

        value.m_data = nullptr;
        value.m_type = Type::NIL;
        LTRACE_F("MOVE INITIALIZE %p, content: %s.", this, toString().c_str());
    }

    void Variant::changeType(Type type)
    {
        // if (m_locked)
        //     throw ConversionException("Cannot change the type of a locked variant.");
        if (this->type() == type)
            return;
        m_type = type;
        switch (m_type)
        {
        case Type::INT:
            m_data = new integer;
            break;
        case Type::REAL:
            m_data = new real;
            break;
        case Type::BOOL:
            m_data = new bool;
            break;
        case Type::STRING:
            m_data = new string;
            break;
        case Type::ARRAY:
            m_data = new array;
            break;
        case Type::LIST:
            m_data = new list;
            break;
        case Type::DICTIONARY:
            m_data = new dictionary;
            break;
        case Type::OBJECT:
            m_data = new Object;
            break;
        case Type::FUNCTION:
            m_data = new Function;
            break;
        default:
            break;
        }
    }

    void Variant::freeMemory()
    {
        LTRACE_F("FREE %p, content: %s.", this, toString().c_str());

        if (m_is_borrowed)
            return;

        switch (m_type)
        {
        case Type::INT:
            delete get<integer>();
            break;
        case Type::REAL:
            delete get<real>();
            break;
        case Type::BOOL:
            delete get<bool>();
            break;
        case Type::STRING:
            delete get<string>();
            break;
        case Type::ARRAY:
        {
            array *ptr = get<array>();
            delete ptr;
            break;
        }
        case Type::LIST:
        {
            list *ptr = get<list>();
            delete ptr;
            break;
        }
        case Type::DICTIONARY:
        {
            dictionary *ptr = get<dictionary>();
            delete get<dictionary>();
            break;
        }
        case Type::OBJECT:
            delete get<Object>();
            break;
        case Type::FUNCTION:
            delete get<Function>();
            break;
        default:
            break;
        }
        m_data = nullptr;
    }

    static std::unordered_set<const Variant *> _constants;

    void Variant::makeConstant() const
    {
        _constants.insert(this);
    }

    bool Variant::isConstant() const
    {
        return _constants.find(this) != _constants.end();
    }

    Variant::Variant(Type type)
    {
        changeType(type);
        LTRACE_F("INITIALIZE %p, content: %s.", this, toString().c_str());
    }

    Variant::~Variant()
    {
        freeMemory();
    }

    Variant &Variant::operator=(integer value)
    {
        if (isConstant())
            throw RuntimeException("Attempting to assign a value to a constant.");
        if (m_data != nullptr)
        {
            Variant::~Variant();
        }

        m_type = Type::INT;
        m_data = new integer;
        *get<integer>() = value;
        return *this;
    }

    Variant &Variant::operator=(real value)
    {
        if (isConstant())
            throw RuntimeException("Attempting to assign a value to a constant.");
        if (m_data != nullptr)
        {
            Variant::~Variant();
        }

        m_type = Type::REAL;
        m_data = new real;
        *get<real>() = value;
        return *this;
    }

    Variant &Variant::operator=(bool value)
    {
        if (isConstant())
            throw RuntimeException("Attempting to assign a value to a constant.");
        if (m_data != nullptr)
        {
            Variant::~Variant();
        }

        m_type = Type::BOOL;
        m_data = new bool;
        *get<bool>() = value;
        return *this;
    }

    Variant &Variant::operator=(const string &value)
    {
        if (isConstant())
            throw RuntimeException("Attempting to assign a value to a constant.");
        if (m_data != nullptr)
        {
            Variant::~Variant();
        }

        m_type = Type::STRING;
        m_data = new string;
        *get<string>() = value;
        return *this;
    }

    Variant &Variant::operator=(const array &value)
    {
        if (isConstant())
            throw RuntimeException("Attempting to assign a value to a constant.");
        if (m_data != nullptr)
        {
            Variant::~Variant();
        }

        m_type = Type::ARRAY;
        m_data = new array;
        *get<array>() = value;
        return *this;
    }

    Variant &Variant::operator=(const list &value)
    {
        if (isConstant())
            throw RuntimeException("Attempting to assign a value to a constant.");
        if (m_data != nullptr)
        {
            Variant::~Variant();
        }

        m_type = Type::LIST;
        m_data = new list;
        *get<list>() = value;
        return *this;
    }

    Variant &Variant::operator=(const dictionary &value)
    {
        if (isConstant())
            throw RuntimeException("Attempting to assign a value to a constant.");
        if (m_data != nullptr)
        {
            Variant::~Variant();
        }

        m_type = Type::DICTIONARY;
        m_data = new dictionary;
        *get<dictionary>() = value;
        return *this;
    }

    Variant &Variant::operator=(const Object &value)
    {
        if (isConstant())
            throw RuntimeException("Attempting to assign a value to a constant.");
        if (m_data != nullptr)
        {
            Variant::~Variant();
        }

        m_type = Type::OBJECT;
        m_data = new Object;
        *get<Object>() = value;
        return *this;
    }

    Variant &Variant::operator=(const Function &value)
    {
        if (isConstant())
            throw RuntimeException("Attempting to assign a value to a constant.");
        if (m_data != nullptr)
        {
            Variant::~Variant();
        }

        m_type = Type::FUNCTION;
        m_data = new Function;
        *get<Function>() = value;
        return *this;
    }

    Variant &Variant::operator=(const Variant &value)
    {
        if (isConstant())
            throw RuntimeException("Attempting to assign a value to a constant.");
        if (data() != nullptr && type() != value.type())
        {
            Variant::~Variant();
        }

        m_is_borrowed = value.m_is_borrowed;
        m_flag = value.m_flag;
        changeType(value.type());
        switch (m_type)
        {
        case Type::INT:
            *get<integer>() = *value.get<integer>();
            break;
        case Type::REAL:
            *get<real>() = *value.get<real>();
            break;
        case Type::BOOL:
            *get<bool>() = *value.get<bool>();
            break;
        case Type::STRING:
            *get<string>() = *value.get<string>();
            break;
        case Type::ARRAY:
            *get<array>() = *value.get<array>();
            break;
        case Type::LIST:
            *get<list>() = *value.get<list>();
            break;
        case Type::DICTIONARY:
            *get<dictionary>() = *value.get<dictionary>();
            break;
        case Type::OBJECT:
            *get<Object>() = *value.get<Object>();
            break;
        case Type::FUNCTION:
            *get<Function>() = *value.get<Function>();
            break;
        default:
            break;
        }
        return *this;
    }

    Variant &Variant::operator=(Variant &&value)
    {
        if (isConstant())
            throw RuntimeException("Attempting to assign a value to a constant.");
        if (m_data != nullptr)
        {
            Variant::~Variant();
        }

        m_type = value.type();
        m_data = value.m_data;
        m_is_borrowed = value.m_is_borrowed;
        m_flag = value.m_flag;

        value.m_type = Type::NIL;
        value.m_data = nullptr;

        return *this;
    }

    bool Variant::operator<(integer value) const
    {
        if (type() != Type::INT)
            throw TypeException(type(), __FUNCTION__);
        return *get<integer>() < value;
    }

    bool Variant::operator<(real value) const
    {
        if (type() != Type::REAL)
            throw TypeException(type(), __FUNCTION__);
        return *get<real>() < value;
    }

    bool Variant::operator<(bool value) const
    {
        if (type() != Type::BOOL)
            throw TypeException(type(), __FUNCTION__);
        return *get<bool>() < value;
    }

    bool Variant::operator<(const char *value) const
    {
        return this->operator<(string(value));
    }

    bool Variant::operator<(const string &value) const
    {
        if (type() != Type::STRING)
            throw TypeException(type(), __FUNCTION__);
        return *get<string>() < value;
    }

    bool Variant::operator<(const Object &value) const
    {
        if (type() != Type::OBJECT)
            throw TypeException(type(), __FUNCTION__);
        return *get<Object>() < value;
    }

    bool Variant::operator<(const Variant &value) const
    {
        switch (type())
        {
        case Type::INT:
            return *get<integer>() < (integer)value;
        case Type::REAL:
            return *get<real>() < (real)value;
        case Type::OBJECT:
            return *get<Object>() < value;
        default:
        {
            if (type() != value.type())
                throw SyntaxException("Cannot compare a (%s) variable with a (%s) variable.", stringifyType(type()).c_str(), stringifyType(value.type()).c_str());
            switch (type())
            {
            case Type::BOOL:
                return *get<bool>() < *value.get<bool>();
            case Type::STRING:
                return *get<string>() < *value.get<string>();
            default:
                return data() < value.data();
            }
        }
        }
    }

    bool Variant::operator>(integer value) const
    {
        if (type() != Type::INT)
            throw TypeException(type(), __FUNCTION__);
        return *get<integer>() > value;
    }

    bool Variant::operator>(real value) const
    {
        if (type() != Type::REAL)
            throw TypeException(type(), __FUNCTION__);
        return *get<real>() > value;
    }

    bool Variant::operator>(bool value) const
    {
        if (type() != Type::BOOL)
            throw TypeException(type(), __FUNCTION__);
        return *get<bool>() > value;
    }

    bool Variant::operator>(const char *value) const
    {
        return this->operator>(string(value));
    }

    bool Variant::operator>(const string &value) const
    {
        if (type() != Type::STRING)
            throw TypeException(type(), __FUNCTION__);
        return *get<string>() > value;
    }

    bool Variant::operator>(const Object &value) const
    {
        if (type() != Type::OBJECT)
            throw TypeException(type(), __FUNCTION__);
        return *get<Object>() > value;
    }

    bool Variant::operator>(const Variant &value) const
    {
        switch (type())
        {
        case Type::INT:
            return *get<integer>() > (integer)value;
        case Type::REAL:
            return *get<real>() > (real)value;
        case Type::OBJECT:
            return *get<Object>() > value;
        default:
        {
            if (type() != value.type())
                throw SyntaxException("Cannot compare a (%s) variable with a (%s) variable.", stringifyType(type()).c_str(), stringifyType(value.type()).c_str());
            switch (type())
            {
            case Type::BOOL:
                return *get<bool>() > *value.get<bool>();
            case Type::STRING:
                return *get<string>() > *value.get<string>();
            default:
                return data() > value.data();
            }
        }
        }
    }

    bool Variant::operator>=(integer value) const
    {
        if (type() != Type::INT)
            throw TypeException(type(), __FUNCTION__);
        return *get<integer>() >= value;
    }

    bool Variant::operator>=(real value) const
    {
        if (type() != Type::REAL)
            throw TypeException(type(), __FUNCTION__);
        return *get<real>() >= value;
    }

    bool Variant::operator>=(bool value) const
    {
        if (type() != Type::BOOL)
            throw TypeException(type(), __FUNCTION__);
        return *get<bool>() >= value;
    }

    bool Variant::operator>=(const char *value) const
    {
        return this->operator>=(string(value));
    }

    bool Variant::operator>=(const string &value) const
    {
        if (type() != Type::STRING)
            throw TypeException(type(), __FUNCTION__);
        return *get<string>() >= value;
    }

    bool Variant::operator>=(const Object &value) const
    {
        if (type() != Type::OBJECT)
            throw TypeException(type(), __FUNCTION__);
        return *get<Object>() >= value;
    }

    bool Variant::operator>=(const Variant &value) const
    {
        switch (type())
        {
        case Type::INT:
            return *get<integer>() >= (integer)value;
        case Type::REAL:
            return *get<real>() >= (real)value;
        case Type::OBJECT:
            return *get<Object>() >= value;
        default:
        {
            if (type() != value.type())
                throw SyntaxException("Cannot compare a (%s) variable with a (%s) variable.", stringifyType(type()).c_str(), stringifyType(value.type()).c_str());
            switch (type())
            {
            case Type::BOOL:
                return *get<bool>() >= *value.get<bool>();
            case Type::STRING:
                return *get<string>() >= *value.get<string>();
            default:
                return data() >= value.data();
            }
        }
        }
    }

    bool Variant::operator<=(integer value) const
    {
        if (type() != Type::INT)
            throw TypeException(type(), __FUNCTION__);
        return *get<integer>() <= value;
    }

    bool Variant::operator<=(real value) const
    {
        if (type() != Type::REAL)
            throw TypeException(type(), __FUNCTION__);
        return *get<real>() <= value;
    }

    bool Variant::operator<=(bool value) const
    {
        if (type() != Type::BOOL)
            throw TypeException(type(), __FUNCTION__);
        return *get<bool>() <= value;
    }

    bool Variant::operator<=(const char *value) const
    {
        return this->operator<=(string(value));
    }

    bool Variant::operator<=(const string &value) const
    {
        if (type() != Type::STRING)
            throw TypeException(type(), __FUNCTION__);
        return *get<string>() <= value;
    }

    bool Variant::operator<=(const Object &value) const
    {
        if (type() != Type::OBJECT)
            throw TypeException(type(), __FUNCTION__);
        return *get<Object>() <= value;
    }

    bool Variant::operator<=(const Variant &value) const
    {
        switch (type())
        {
        case Type::INT:
            return *get<integer>() <= (integer)value;
        case Type::REAL:
            return *get<real>() <= (real)value;
        case Type::OBJECT:
            return *get<Object>() <= value;
        default:
        {
            if (type() != value.type())
                throw SyntaxException("Cannot compare a (%s) variable with a (%s) variable.", stringifyType(type()).c_str(), stringifyType(value.type()).c_str());
            switch (type())
            {
            case Type::BOOL:
                return *get<bool>() <= *value.get<bool>();
            case Type::STRING:
                return *get<string>() <= *value.get<string>();
            default:
                return data() <= value.data();
            }
        }
        }
    }

    bool Variant::operator==(integer value) const
    {
        if (type() != Type::INT)
            throw TypeException(type(), __FUNCTION__);
        return *get<integer>() == value;
    }

    bool Variant::operator==(real value) const
    {
        if (type() != Type::REAL)
            throw TypeException(type(), __FUNCTION__);
        return *get<real>() == value;
    }

    bool Variant::operator==(bool value) const
    {
        if (type() != Type::BOOL)
            throw TypeException(type(), __FUNCTION__);
        return *get<bool>() == value;
    }

    bool Variant::operator==(const char *value) const
    {
        return this->operator==(string(value));
    }

    bool Variant::operator==(const string &value) const
    {
        if (type() != Type::STRING)
            throw TypeException(type(), __FUNCTION__);
        return *get<string>() == value;
    }

    bool Variant::operator==(const Object &value) const
    {
        if (type() != Type::OBJECT)
            throw TypeException(type(), __FUNCTION__);
        return *get<Object>() == value;
    }

    bool Variant::operator==(const Variant &value) const
    {
        switch (type())
        {
        case Type::INT:
            return *get<integer>() == (integer)value;
        case Type::REAL:
            return *get<real>() == (real)value;
        case Type::OBJECT:
            return *get<Object>() == value;
        default:
        {
            if (type() != value.type())
                throw SyntaxException("Cannot compare a (%s) variable with a (%s) variable.", stringifyType(type()).c_str(), stringifyType(value.type()).c_str());
            switch (type())
            {
            case Type::BOOL:
                return *get<bool>() == *value.get<bool>();
            case Type::STRING:
                return *get<string>() == *value.get<string>();
            default:
                return data() == value.data();
            }
        }
        }
    }

    bool Variant::operator!=(integer value) const
    {
        if (type() != Type::INT)
            throw TypeException(type(), __FUNCTION__);
        return *get<integer>() != value;
    }

    bool Variant::operator!=(real value) const
    {
        if (type() != Type::REAL)
            throw TypeException(type(), __FUNCTION__);
        return *get<real>() != value;
    }

    bool Variant::operator!=(bool value) const
    {
        if (type() != Type::BOOL)
            throw TypeException(type(), __FUNCTION__);
        return *get<bool>() != value;
    }

    bool Variant::operator!=(const char *value) const
    {
        return this->operator!=(string(value));
    }

    bool Variant::operator!=(const string &value) const
    {
        if (type() != Type::STRING)
            throw TypeException(type(), __FUNCTION__);
        return *get<string>() != value;
    }

    bool Variant::operator!=(const Object &value) const
    {
        if (type() != Type::OBJECT)
            throw TypeException(type(), __FUNCTION__);
        return *get<Object>() != value;
    }

    bool Variant::operator!=(const Variant &value) const
    {
        switch (type())
        {
        case Type::INT:
            return *get<integer>() != (integer)value;
        case Type::REAL:
            return *get<real>() != (real)value;
        case Type::OBJECT:
            return *get<Object>() != value;
        default:
        {
            if (type() != value.type())
                throw SyntaxException("Cannot compare a (%s) variable with a (%s) variable.", stringifyType(type()).c_str(), stringifyType(value.type()).c_str());
            switch (type())
            {
            case Type::BOOL:
                return *get<bool>() != *value.get<bool>();
            case Type::STRING:
                return *get<string>() != *value.get<string>();
            default:
                return data() != value.data();
            }
        }
        }
    }

    list::iterator Variant::getIterator(integer position)
    {
        if (type() != Type::LIST)
        {
            throw TypeException(type(), __FUNCTION__);
        }
        list *l = get<list>();
        list::iterator it;
        if (position < l->size() / 2)
        {
            it = l->begin();
            for (integer i = 0; i < position; ++i, ++it)
                ;
        }
        else
        {
            it = l->end();
            for (integer i = 0; i < position; ++i, ++it)
                ;
        }
        return it;
    }

    list::const_iterator Variant::getIterator(integer position) const
    {
        if (type() != Type::LIST)
        {
            throw TypeException(type(), __FUNCTION__);
        }
        const list *l = get<list>();
        list::const_iterator it;
        if (position < l->size() / 2)
        {
            it = l->begin();
            for (integer i = 0; i < position; ++i, ++it)
                ;
        }
        else
        {
            it = l->end();
            for (integer i = 0; i < position; ++i, ++it)
                ;
        }
        return it;
    }

    void Variant::pushBack(const Reference<Variant> &element)
    {
        if (type() == Type::ARRAY)
            getArray()->push_back(element);
        else if (type() == Type::LIST)
            getList()->push_back(element);
        else
            throw TypeException(type(), __FUNCTION__);
    }

    void Variant::pushFront(const Reference<Variant> &element)
    {
        if (type() == Type::LIST)
            getList()->push_front(element);
        else
            throw TypeException(type(), __FUNCTION__);
    }

    void Variant::insert(integer position, const Reference<Variant> &element)
    {
        if (type() == Type::ARRAY)
        {
            array *arr = get<array>();
            arr->insert(arr->begin() + position, element);
        }
        else if (type() == Type::LIST)
        {
            list *l = get<list>();
            l->insert(getIterator(position), element);
        }
        else
            throw TypeException(type(), __FUNCTION__);
    }

    void Variant::remove(integer position)
    {
        if (type() == Type::ARRAY)
        {
            array *arr = get<array>();
            arr->erase(arr->begin() + position);
        }
        else if (type() == Type::LIST)
        {
            list::iterator it = getIterator(position);
            get<list>()->erase(it);
        }
        else
        {
            throw TypeException(type(), __FUNCTION__);
        }
    }

    void Variant::remove(integer from, integer to)
    {
        if (type() == Type::ARRAY)
        {
            array *arr = get<array>();
            arr->erase(arr->begin() + from, arr->begin() + to);
        }
        else if (type() == Type::LIST)
        {
            list::iterator from_it = getIterator(from);
            list::iterator to_it = getIterator(to);
            get<list>()->erase(from_it, to_it);
        }
        else
        {
            throw TypeException(type(), __FUNCTION__);
        }
    }

    string Variant::toString() const
    {
        std::ostringstream ss;
        switch (type())
        {
        case Type::NIL:
            ss << "Null";
            break;
        case Type::INT:
            ss << *get<integer>();
            break;
        case Type::REAL:
            ss << *get<real>();
            break;
        case Type::BOOL:
            ss << (*get<bool>() ? "true" : "false");
            break;
        case Type::STRING:
            ss << '"' << *get<string>() << '"';
            break;
        case Type::ARRAY:
        {
            bool is_first = true;
            ss << "[";
            for (auto &&item : (*get<array>()))
            {
                if (is_first)
                {
                    is_first = false;
                }
                else
                {
                    ss << ", ";
                }
                ss << item->toString();
            }
            ss << "]";
            break;
        }
        case Type::LIST:
        {
            bool is_first = true;
            ss << "[";
            for (auto &&item : (*get<list>()))
            {
                if (is_first)
                {
                    is_first = false;
                }
                else
                {
                    ss << ", ";
                }
                ss << item->toString();
            }
            ss << "]";
            break;
        }
        case Type::DICTIONARY:
        {
            bool is_first = true;
            ss << "{";
            for (auto &&item : (*get<dictionary>()))
            {
                if (is_first)
                {
                    is_first = false;
                }
                else
                {
                    ss << ", ";
                }
                ss << item.first.toString() << ": " << item.second->toString();
            }
            ss << "}";
            break;
        }
        case Type::OBJECT:
        {
            ss << get<Object>()->toString();
            break;
        }
        case Type::FUNCTION:
        {
            ss << get<Function>()->stringify();
            break;
        }
        default:
            break;
        }
        return ss.str();
    }

    string Variant::stringifyType(Type type)
    {
        switch (type)
        {
        case Type::INT:
            return "Integer";
        case Type::REAL:
            return "Real";
        case Type::BOOL:
            return "Boolean";
        case Type::STRING:
            return "String";
        case Type::ARRAY:
            return "Array";
        case Type::LIST:
            return "List";
        case Type::DICTIONARY:
            return "Dictionary";
        case Type::OBJECT:
            return "Object";
        case Type::FUNCTION:
            return "Function";
        case Type::NIL:
            return "Null";
        case Type::ANY:
            return "Any";
        default:
            return "Unknown(" + std::to_string(uint8_t(type)) + ")";
        }
    }

    integer Variant::_hash() const
    {
        switch (type())
        {
        case Type::INT:
            return std::hash<integer>()(*get<integer>());
        case Type::REAL:
            return std::hash<real>()(*get<real>());
        case Type::BOOL:
            return std::hash<bool>()(*get<bool>());
        case Type::STRING:
            return std::hash<string>()(*get<string>());
        case Type::ARRAY:
        {
            integer result = 0;
            integer index = 0;
            integer mask = 0xff;
            for (auto &&item : *get<array>())
            {
                result ^= item->hash();
                result ^= (mask << index) | (mask << (sizeof(mask) * 8 - index));
                index = (index + 1) % (sizeof(mask) * 8);
            }

            return result;
        }
        case Type::LIST:
        {
            integer result = 0;
            integer index = 0;
            integer mask = 0xff;
            for (auto &&item : *get<list>())
            {
                result ^= item->hash();
                result ^= (mask << index) | (mask << (sizeof(mask) * 8 - index));
                index = (index + 1) % (sizeof(mask) * 8);
            }

            return result;
        }
        case Type::DICTIONARY:
        {
            integer result = 0;
            integer prime = 0xffff;

            for (auto &&item : *get<dictionary>())
            {
                std::size_t keyHash = item.first.hash();
                std::size_t valueHash = item.second->hash();

                result ^= (keyHash ^ valueHash) + prime;
            }

            return result;
        }
        case Type::OBJECT:
        {
            integer result = get<Object>()->hash();
            if (result == -1)
            {
                return std::hash<char *>()(reinterpret_cast<char *>(m_data));
            }
            return result;
        }
        default:
            return std::hash<char *>()(reinterpret_cast<char *>(m_data));
        }
    }

    integer *Variant::getInteger()
    {
        if (type() != Type::INT)
            throw ConversionException("Attempt to obtain a value of type INT on a variable that is not of type INT.");
        return get<integer>();
    }

    real *Variant::getReal()
    {
        if (type() != Type::REAL)
            throw ConversionException("Attempt to obtain a value of type REAL on a variable that is not of type REAL.");
        return get<real>();
    }

    bool *Variant::getBool()
    {
        if (type() != Type::BOOL)
            throw ConversionException("Attempt to obtain a value of type BOOL on a variable that is not of type BOOL.");
        return get<bool>();
    }

    string *Variant::getString()
    {
        if (type() != Type::STRING)
            throw ConversionException("Attempt to obtain a value of type STRING on a variable that is not of type STRING.");
        return get<string>();
    }

    array *Variant::getArray()
    {
        if (type() != Type::ARRAY)
            throw ConversionException("Attempt to obtain a value of type ARRAY on a variable that is not of type ARRAY.");
        return get<array>();
    }

    list *Variant::getList()
    {
        if (type() != Type::LIST)
            throw ConversionException("Attempt to obtain a value of type LIST on a variable that is not of type LIST.");
        return get<list>();
    }

    dictionary *Variant::getDictionary()
    {
        if (type() != Type::DICTIONARY)
            throw ConversionException("Attempt to obtain a value of type DICTIONARY on a variable that is not of type DICTIONARY.");
        return get<dictionary>();
    }

    Function *Variant::getFunction()
    {
        if (type() != Type::FUNCTION)
            throw ConversionException("Attempt to obtain a value of type FUNCTION on a variable that is not of type FUNCTION.");
        return get<Function>();
    }

    Object *Variant::getObject()
    {
        if (type() != Type::OBJECT)
            throw ConversionException("Attempt to obtain a value of type OBJECT on a variable that is not of type OBJECT.");
        return get<Object>();
    }

    void *Variant::getAny()
    {
        if (type() != Type::ANY)
            throw ConversionException("Attempt to obtain a value of type ANY on a variable that is not of type ANY.");
        return m_data;
    }

    const integer *Variant::getInteger() const
    {
        if (type() != Type::INT)
            throw ConversionException("Attempt to obtain a value of type INT on a variable that is not of type INT.");
        return get<integer>();
    }

    const real *Variant::getReal() const
    {
        if (type() != Type::REAL)
            throw ConversionException("Attempt to obtain a value of type REAL on a variable that is not of type REAL.");
        return get<real>();
    }

    const bool *Variant::getBool() const
    {
        if (type() != Type::BOOL)
            throw ConversionException("Attempt to obtain a value of type BOOL on a variable that is not of type BOOL.");
        return get<bool>();
    }

    const string *Variant::getString() const
    {
        if (type() != Type::STRING)
            throw ConversionException("Attempt to obtain a value of type STRING on a variable that is not of type STRING.");
        return get<string>();
    }

    const array *Variant::getArray() const
    {
        if (type() != Type::ARRAY)
            throw ConversionException("Attempt to obtain a value of type ARRAY on a variable that is not of type ARRAY.");
        return get<array>();
    }

    const list *Variant::getList() const
    {
        if (type() != Type::LIST)
            throw ConversionException("Attempt to obtain a value of type LIST on a variable that is not of type LIST.");
        return get<list>();
    }

    const dictionary *Variant::getDictionary() const
    {
        if (type() != Type::DICTIONARY)
            throw ConversionException("Attempt to obtain a value of type DICTIONARY on a variable that is not of type DICTIONARY.");
        return get<dictionary>();
    }

    const Function *Variant::getFunction() const
    {
        if (type() != Type::FUNCTION)
            throw ConversionException("Attempt to obtain a value of type FUNCTION on a variable that is not of type FUNCTION.");
        return get<Function>();
    }

    const Object *Variant::getObject() const
    {
        if (type() != Type::OBJECT)
            throw ConversionException("Attempt to obtain a value of type OBJECT on a variable that is not of type OBJECT.");
        return get<Object>();
    }

    const void *Variant::getAny() const
    {
        if (type() != Type::ANY)
            throw ConversionException("Attempt to obtain a value of type ANY on a variable that is not of type ANY.");
        return m_data;
    }

    integer Variant::left_shift(integer bits) const
    {
        if (type() != Type::INT)
            throw TypeException(type(), __FUNCTION__);
        return *get<integer>() << bits;
    }

    integer Variant::right_shift(integer bits) const
    {
        if (type() != Type::INT)
            throw TypeException(type(), __FUNCTION__);
        return *get<integer>() >> bits;
    }

    Variant Variant::negative() const
    {
        if (type() == Type::INT)
            return -(*get<integer>());
        else if (type() == Type::REAL)
            return -(*get<real>());
        else
            throw TypeException(type(), __FUNCTION__);
    }

    Variant Variant::reverse() const
    {
        if (type() == Type::INT)
            return ~(*get<integer>());
        else
            throw TypeException(type(), __FUNCTION__);
    }

    integer Variant::power(real pow) const
    {
        if (type() == Type::INT)
            return std::pow(*get<integer>(), pow);
        else if (type() == Type::REAL)
            return std::pow(*get<real>(), pow);
        else
            throw TypeException(type(), __FUNCTION__);
    }

    Variant &Variant::getProperty(const Variant &key)
    {
        if (type() == Type::ARRAY)
            return *get<array>()->at(*key.getInteger());
        else if (type() == Type::LIST)
            return **getIterator(*key.getInteger());
        else if (type() == Type::DICTIONARY)
            return *get<dictionary>()->at(key);
        else
            throw TypeException(type(), __FUNCTION__);
    }

    const Variant &Variant::getProperty(const Variant &key) const
    {
        if (type() == Type::ARRAY)
            return *get<array>()->at(*key.getInteger());
        else if (type() == Type::LIST)
            return **getIterator(*key.getInteger());
        else if (type() == Type::DICTIONARY)
            return *get<dictionary>()->at(key);
        else
            throw TypeException(type(), __FUNCTION__);
    }

    void Variant::setProperty(const Variant &key, const Ref<Variant> &value)
    {
        if (type() == Type::ARRAY)
            getArray()->at(*key.getInteger()) = value;
        else if (type() == Type::LIST)
            *getIterator(*key.getInteger()) = value;
        else if (type() == Type::DICTIONARY)
            (*getDictionary())[key] = value;
        else
            throw TypeException(type(), __FUNCTION__);
    }

    bool Variant::hasProperty(const Variant &key) const
    {
        if (type() == Type::ARRAY)
            return *key.getInteger() < get<array>()->size();
        else if (type() == Type::LIST)
            return *key.getInteger() < get<list>()->size();
        else if (type() == Type::DICTIONARY)
            return get<dictionary>()->count(key);
        else
            throw TypeException(type(), __FUNCTION__);
    }

    Variant Variant::call(array &args)
    {
        if (flag() == FLAG_METHOD)
            return get<Method>()->call(args);
        switch (type())
        {
        case Type::NIL:
            throw RuntimeException("Attempt to call a null value.");
        case Type::OBJECT:
            return get<Object>()->call(args);
        case Type::FUNCTION:
            return get<Function>()->call(args);
        default:
            throw TypeException(type(), __FUNCTION__);
        }
    }

    static inline integer parseIndex(Variant *value, integer index)
    {
        switch (value->type())
        {
        case Variant::Type::ARRAY:
            if (index >= 0)
                return index;
            else
                return value->getArray()->size() + index;
        default:
            break;
        }
        return 0;
    }

    std::map<Variant::Type, Module *> type_lib_map{
        {Variant::Type::ARRAY, &builtin::array_lib},
        {Variant::Type::STRING, &builtin::string_lib},
    };

    Ref<Variant> Variant::access(const array &args, const Ref<Variant> &this_reference)
    {
        if (args.size() == 1 && args[0]->type() == Type::STRING)
        {
            const string &key = *args[0]->getString();
            auto pick_lib = [this, &key, &this_reference](Module *lib) -> Variant
            {
                if (lib->has(key) && m_data != lib)
                {
                    Variant result = lib->access(key);
                    if (result.flag() == FLAG_METHOD)
                        result.get<Method>()->bind(0, this_reference);
                    return result;
                }
                return *Variant::Null;
            };
            Variant value;
            if (type_lib_map.find(type()) != type_lib_map.end() &&
                (value = pick_lib(type_lib_map[type()])) != *Variant::Null)
                return value;
            if ((value = pick_lib(&builtin::variant_lib)) != *Variant::Null)
                return value;
        }
        switch (type())
        {
        case Type::NIL:
        {
            throw RuntimeException("Attempt to index a nil value.");
        }
        case Type::ARRAY:
        {
            if (args.size() == 0)
                return get<array>()->at(0);
            else if (args.size() == 1)
                return get<array>()->at(parseIndex(this, *args[0]));
            else if (args.size() == 2 || args.size() == 3)
            {
                integer from = parseIndex(this, *args[0]);
                integer to = parseIndex(this, *args[1]);
                integer step = args.size() == 3 ? integer(*args[2]) : 1;
                Ref<Variant> result(new Variant{Variant::ARRAY});
                if (from <= to)
                    for (integer i = from; i <= to; i += step)
                        result->pushBack(get<array>()->at(parseIndex(this, i)));
                else
                    for (integer i = from; i >= to; i -= step)
                        result->pushBack(get<array>()->at(parseIndex(this, i)));
                return result;
            }
            else
                throw ArgumentException(__FUNCTION__);
            break;
        }
        case Type::DICTIONARY:
        {
            if (args.size() == 1)
                return getProperty(*args[0]);
            else
                throw ArgumentException(__FUNCTION__);
            break;
        }
        case Type::OBJECT:
        {
            return getObject()->access(args, m_flag);
            break;
        }

        default:
            break;
        }
        return *Variant::Null;
    }

    uinteger Variant::size() const
    {
        switch (type())
        {
        case Type::ARRAY:
            return getArray()->size();
        case Type::LIST:
            return getList()->size();
        case Type::DICTIONARY:
            return getDictionary()->size();
        default:
            throw TypeException(__FUNCTION__);
        }
    }

    void Variant::removeProperty(const Variant &key)
    {
        if (type() == Type::ARRAY || type() == Type::LIST)
            remove(key);
        else if (type() == Type::DICTIONARY)
            get<dictionary>()->erase(key);
        else
            throw TypeException(type(), __FUNCTION__);
    }

    Variant Variant::copy() const
    {
        switch (type())
        {
        case Type::ARRAY:
        {
            Variant result{Type::ARRAY};
            uinteger s = getArray()->size();
            result.getArray()->reserve(s);
            for (integer i = 0; i < s; i++)
                result.getArray()->push_back(getProperty(i));
            return result;
        }
        case Type::LIST:
        {
            Variant result{Type::LIST};
            result.getList()->resize(getList()->size());
            auto r_it = result.getList()->begin();
            for (auto it = getList()->begin(); it != getList()->end(); it++)
                *(r_it++) = *it;
            return result;
        }
        case Type::DICTIONARY:
        {
            Variant result{Type::DICTIONARY};
            for (auto pair : *getDictionary())
                result.setProperty(pair.first, *pair.second);
            return result;
        }
        default:
            return Variant{*this};
        }
    }

    Variant Variant::copyDeep() const
    {
        switch (type())
        {
        case Type::ARRAY:
        {
            Variant result{Type::ARRAY};
            uinteger s = getArray()->size();
            result.getArray()->reserve(s);
            for (integer i = 0; i < s; i++)
                result.getArray()->push_back(getProperty(i).copyDeep());
            return result;
        }
        case Type::LIST:
        {
            Variant result{Type::LIST};
            result.getList()->resize(getList()->size());
            auto r_it = result.getList()->begin();
            for (auto it = getList()->begin(); it != getList()->end(); it++)
                **(r_it++) = (*it)->copyDeep();
            return result;
        }
        case Type::DICTIONARY:
        {
            Variant result{Type::DICTIONARY};
            for (auto pair : *getDictionary())
                result.setProperty(pair.first, pair.second->copyDeep());
            return result;
        }
        default:
            return Variant{*this};
        }
    }

    Variant::operator integer() const
    {
        switch (type())
        {
        case Type::INT:
            return *getInteger();
        case Type::REAL:
            return *getReal();
        case Type::STRING:
        {
            std::string str{*getString()};
            if (!algorithm::isInteger(str))
                throw ConversionException("Connot convert (%s) to integer.", str.c_str());
            return std::stoll(str);
        }
        default:
            throw ConversionException("Cannot convert a(n) %s to integer.", stringifyType(type()).c_str());
        }
    }

    Variant::operator real() const
    {
        switch (type())
        {
        case Type::INT:
            return *getInteger();
        case Type::REAL:
            return *getReal();
        case Type::STRING:
        {
            std::string str{*getString()};
            if (!algorithm::isNumber(str))
                throw ConversionException("Cannot convert (%s) to real.", str.c_str());
            return std::stold(*getString());
        }
        default:
            throw ConversionException("Cannot convert a(n) %s to integer.", stringifyType(type()).c_str());
        }
    }

    Variant::operator bool() const
    {
        switch (type())
        {
        case Type::NIL:
            return false;
        case Type::INT:
            return *get<integer>() != 0;
        case Type::REAL:
            return *get<real>() != 0.0;
        case Type::BOOL:
            return *getBool();
        case Type::STRING:
            return get<string>()->empty();
        case Type::ARRAY:
            return get<array>()->empty();
        case Type::LIST:
            return get<list>()->empty();
        case Type::DICTIONARY:
            return get<dictionary>()->empty();
        case Type::FUNCTION:
            return true;
        case Type::OBJECT:
            return get<Object>()->toBool();
        case Type::ANY:
            return true;
        default:
            return false;
        }
        return false;
    }

    Variant::operator string() const
    {
        std::ostringstream ss;
        switch (type())
        {
        case Type::NIL:
            ss << "Null";
            break;
        case Type::INT:
            ss << *get<integer>();
            break;
        case Type::REAL:
            ss << *get<real>();
            break;
        case Type::BOOL:
            ss << (*get<bool>() ? "true" : "false");
            break;
        case Type::STRING:
            ss << *get<string>();
            break;
        case Type::ARRAY:
        {
            bool is_first = true;
            ss << "[";
            for (auto &&item : (*get<array>()))
            {
                if (is_first)
                {
                    is_first = false;
                }
                else
                {
                    ss << ", ";
                }
                ss << item->toString();
            }
            ss << "]";
            break;
        }
        case Type::LIST:
        {
            bool is_first = true;
            ss << "[";
            for (auto &&item : (*get<list>()))
            {
                if (is_first)
                {
                    is_first = false;
                }
                else
                {
                    ss << ", ";
                }
                ss << item->toString();
            }
            ss << "]";
            break;
        }
        case Type::DICTIONARY:
        {
            bool is_first = true;
            ss << "{";
            for (auto &&item : (*get<dictionary>()))
            {
                if (is_first)
                {
                    is_first = false;
                }
                else
                {
                    ss << ", ";
                }
                ss << item.first.toString() << ": " << item.second->toString();
            }
            ss << "}";
            break;
        }
        case Type::OBJECT:
        {
            ss << get<Object>()->toString();
            break;
        }
        case Type::FUNCTION:
        {
            ss << get<Function>()->stringify();
            break;
        }
        default:
            break;
        }
        return ss.str();
    }

    Variant::operator array() const
    {
        return *getArray();
    }

    Variant::operator list() const
    {
        return *getList();
    }

    Variant::operator dictionary() const
    {
        return *getDictionary();
    }

    Variant::operator Object() const
    {
        return *getObject();
    }

    Variant::operator Function() const
    {
        return *getFunction();
    }

    OPERATOR_S(add, +)
    OPERATOR(sub, -)
    OPERATOR(mul, *)
    OPERATOR(div, /)
    OPERATOR_I(mod, %)
    OPERATOR_I(band, &)
    OPERATOR_I(bor, |)
    OPERATOR_I(bxor, ^)

    Variant makeArray(const real_array &arr)
    {
        Variant result(Variant::Type::ARRAY);

        for (const Variant &element : arr)
        {
            Ref<Variant> temp(element);
            result.pushBack(temp);
        }

        return result;
    }

    Variant makeList(const real_list &arr)
    {
        Variant result(Variant::Type::LIST);

        for (const Variant &element : arr)
        {
            Ref<Variant> temp(element);
            result.pushBack(temp);
        }

        return result;
    }

    Variant makeDict(const real_dictionary &dict)
    {
        Variant result(Variant::Type::DICTIONARY);

        for (auto &&pair : dict)
        {
            Ref<Variant> temp(pair.second);
            result.setProperty(pair.first, temp);
        }

        return result;
    }

    size_t VariantEqual::operator()(const Reference<Variant> &a, const Reference<Variant> &b) const
    {
        return *a == *b;
    }

    size_t VariantHash::operator()(const Variant &value) const
    {
        return value.hash();
    }
} // namespace phi
