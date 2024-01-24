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
    DefineException(CompileException, Exception)
    DefineException(SyntaxException, CompileException)

    class BinaryException : public RuntimeException
    {
    private:
        Variant::Type _M_a;
        Variant::Type _M_b;

    public:
        using RuntimeException::RuntimeException;

        BinaryException(Variant::Type a, Variant::Type b) : _M_a(a), _M_b(b) {}

        virtual string className() override
        {
            return "BinaryException";
        }

        virtual string what() override
        {
            static std::ostringstream ss;
            ss.str("");
            ss << "Unable to operate between "
               << Variant::stringifyType(_M_a) << " and " << Variant::stringifyType(_M_b) << ".\n";
            ss << RuntimeException::what();
            return ss.str();
        }
    };

    class UnaryException : public RuntimeException
    {
    private:
        Variant::Type _M_t;

    public:
        using RuntimeException::RuntimeException;

        UnaryException(Variant::Type t) : _M_t(t) {}

        virtual string className() override
        {
            return "UnaryException";
        }

        virtual string what() override
        {
            static std::ostringstream ss;
            ss.str("");
            ss << "Unable to operate with the type of "
               << Variant::stringifyType(_M_t) << ".\n";
            ss << RuntimeException::what();
            return ss.str();
        }
    };

    DefineException(CalculateException, BinaryException)
        DefineException(ConversionException, BinaryException)
            DefineException(CompareException, BinaryException)

                class ArgumentException : public RuntimeException
    {
    private:
        integer _M_expected;
        integer _M_actually;
        string _M_method;

    public:
        using RuntimeException::RuntimeException;

        ArgumentException(integer expected, integer actually, const string &method) : _M_expected(expected), _M_actually(actually), _M_method(method) {}

        virtual string className() override
        {
            return "ArgumentException";
        }

        string what() override
        {
            static std::ostringstream ss;
            ss.str("");
            ss << "Too few arguments for method " << _M_method << " (expected " << _M_expected << " arguments, got " << _M_actually << " arguments).\n";
            ss << RuntimeException::what();
            return ss.str();
        }
    };

    class FileException : public Exception
    {
    private:
        string _M_filename;

    public:
        using Exception::Exception;

        FileException(const string& filename) : _M_filename(filename) {}

        virtual string className() override
        {
            return "FilenameException";
        }

        string what() override
        {
            static std::ostringstream ss;
            ss.str("");
            ss << "Unable to open file " << _M_filename << ".\n";
            ss << Exception::what();
            return ss.str();
        }
    };
} // namespace phi
