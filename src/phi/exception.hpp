#pragma once
#include <phi/typedef.hpp>
#include <phi/variant.hpp>
#include <sstream>

#define DefineException(name, parent)                                          \
    class name : public parent {                                               \
      public:                                                                  \
        using parent::parent;                                                  \
        virtual string className() const override { return #name; }            \
    }

namespace phi {
    class Exception {
      private:
        string _M_what;

      public:
        Exception() = default;
        Exception(const string &what) : _M_what(what) {}
        Exception(const char *what) { _M_what.assign(what); }
        template <typename... Args>
        Exception(const char *format, Args &&...args) {
            char *buffer = new char[1024];
            snprintf(buffer, 1024, format, args...);
            _M_what.assign(buffer);
            delete buffer;
        }

        virtual string className() const { return "Exception"; }
        virtual string what() const { return _M_what; }
    };

    DefineException(NoImplementException, Exception);
    DefineException(RuntimeException, Exception);
    DefineException(ValueException, RuntimeException);
    DefineException(LoadException, RuntimeException);
    DefineException(CompileException, Exception);
    DefineException(SyntaxException, CompileException);

    class CompilerException : public RuntimeException {
      public:
        using RuntimeException::RuntimeException;

        virtual string className() const override {
            return "CompilerException";
        }

        virtual string what() const override {
            static std::ostringstream ss;
            ss.str("");
            ss << "This exception is caused by a compiler bug, please report "
                  "it.\n"
               << RuntimeException::what();
            return ss.str();
        }
    };

    class VariantPacker;
    class OperateNullException : public RuntimeException {
      private:
        string _M_value;
        string _M_operation;

      public:
        // using RuntimeException::RuntimeException;
        OperateNullException(const VariantPacker &value,
                             const string &operation);

        virtual string className() const override {
            return "OperateNullException";
        }
        virtual string what() const override {
            static std::ostringstream ss;
            ss.str("");
            ss << "Attempt to " << _M_operation << " a null value. " << _M_value
               << endl;
            ss << RuntimeException::what();
            return ss.str();
        }
    };

    class BinaryException : public RuntimeException {
      private:
        Variant::Type _M_a;
        Variant::Type _M_b;

      public:
        using RuntimeException::RuntimeException;

        BinaryException(Variant::Type a, Variant::Type b) : _M_a(a), _M_b(b) {}

        BinaryException &setTypes(Variant::Type a, Variant::Type b) {
            _M_a = a;
            _M_b = b;
            return *this;
        }

        virtual string className() const override { return "BinaryException"; }

        virtual string what() const override {
            static std::ostringstream ss;
            ss.str("");
            ss << "Unable to operate between " << Variant::stringifyType(_M_a)
               << " and " << Variant::stringifyType(_M_b) << ".\n";
            ss << RuntimeException::what();
            return ss.str();
        }
    };

    class UnaryException : public RuntimeException {
      private:
        Variant::Type _M_t;

      public:
        using RuntimeException::RuntimeException;

        explicit UnaryException(Variant::Type t) : _M_t(t) {}

        virtual string className() const override { return "UnaryException"; }

        virtual string what() const override {
            static std::ostringstream ss;
            ss.str("");
            ss << "Unable to operate with the type of "
               << Variant::stringifyType(_M_t) << ".\n";
            ss << RuntimeException::what();
            return ss.str();
        }
    };

    DefineException(CalculateException, BinaryException);
    DefineException(ConversionException, BinaryException);
    DefineException(CompareException, BinaryException);

    class ArgumentException : public RuntimeException {
      private:
        integer _M_expected;
        integer _M_actually;
        string _M_method;

      public:
        using RuntimeException::RuntimeException;

        ArgumentException(integer expected, integer actually,
                          const string &method)
            : _M_expected(expected), _M_actually(actually), _M_method(method) {}

        virtual string className() const override {
            return "ArgumentException";
        }

        string what() const override {
            static std::ostringstream ss;
            ss.str("");
            ss << "Too few arguments for method " << _M_method << " (expected "
               << _M_expected << " arguments, got " << _M_actually
               << " arguments).\n";
            ss << RuntimeException::what();
            return ss.str();
        }
    };

    class ArgumentRangedException : public RuntimeException {
      private:
        integer _M_least;
        integer _M_most;
        integer _M_actually;
        string _M_method;

      public:
        using RuntimeException::RuntimeException;

        ArgumentRangedException(integer least, integer most, integer actually,
                                const string &method)
            : _M_least(least), _M_most(most), _M_actually(actually),
              _M_method(method) {}

        virtual string className() const override {
            return "ArgumentRangedException";
        }

        string what() const override {
            static std::ostringstream ss;
            ss.str("");
            ss << "Bad arguments for method " << _M_method
               << " (expected at least " << _M_least << " and at most "
               << _M_most << " arguments, got " << _M_actually
               << " arguments).\n";
            ss << RuntimeException::what();
            return ss.str();
        }
    };

    class FileException : public Exception {
      private:
        string _M_filename;

      public:
        using Exception::Exception;

        FileException(const string &filename) : _M_filename(filename) {}

        virtual string className() const override {
            return "FilenameException";
        }

        string what() const override {
            static std::ostringstream ss;
            ss.str("");
            ss << "Unable to open the file \"" << _M_filename << "\".\n";
            ss << Exception::what();
            return ss.str();
        }
    };
} // namespace phi
