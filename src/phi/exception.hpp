#pragma once
#include <phi/typedef.hpp>
#include <phi/variant.hpp>
#include <sstream>

#define DefineException(name, parent)       \
    class name : public parent              \
    {                                       \
    public:                                 \
        using parent::parent;               \
        virtual string className() override \
        {                                   \
            return #name;                   \
        }                                   \
    };

namespace phi
{
    class Exception
    {
    private:
        string _M_what;
    public:
        Exception() = default;
        Exception(const string &what) : _M_what(what) {}

        virtual string className()
        {
            return "Exception";
        }
        virtual string what()
        {
            return _M_what;
        }
    };

    DefineException(RuntimeException, Exception)

        // formatting bug
        ;

    class ConversionException : public RuntimeException
    {
    private:
        Variant::Type _M_from;
        Variant::Type _M_to;

    public:
        using RuntimeException::RuntimeException;

        ConversionException(Variant::Type from, Variant::Type to) : _M_from(from), _M_to(to) {}

        virtual string className() override
        {
            return "ConversionException";
        }

        virtual string what() override
        {
            static std::ostringstream ss;
            ss.str("");
            ss << "Cannot convert the type from "
               << Variant::stringifyType(_M_from) << " to " << Variant::stringifyType(_M_to) << ".\n";
            ss << RuntimeException::what();
            return ss.str();
        }
    };
} // namespace phi
