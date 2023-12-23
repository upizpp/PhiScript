#pragma once
#include "data_type.hpp"
#include "variant.hpp"
#include <format>
#include <sstream>
#include <string>
#include <vector>
#include <cstdio>

#define DefineException(name) \
    virtual string getClassName() const override { return #name; }

namespace phi
{
    using std::string;

    class Exception
    {
    protected:
        string m_detail;

        virtual string msg() const
        {
            return "Exception";
        }

    public:
        virtual string getClassName() const { return "Exception"; }

        explicit Exception() {}
        explicit Exception(const string &detail) : m_detail(detail) {}
        template<typename... Args>
        explicit Exception(const string &fmt, Args... args){
            char* buffer = new char[0xfff];
            std::sprintf(buffer, fmt.c_str(), args...);
            m_detail = buffer;
            delete[] buffer;
        }

        string what() const
        {
            return msg() + "\n" + m_detail;
        }
    };

    class ConversionException : public Exception
    {
    private:
        virtual string msg() const override
        {
            return "Conversion failed.";
        }

    public:
        DefineException(ConversionException) using Exception::Exception;
    };

    class ArgumentException : public Exception
    {
    private:
        virtual string msg() const override
        {
            return "Invalid argument.";
        }

    public:
        DefineException(ArgumentException) using Exception::Exception;
    };

    class OperandException : public ArgumentException
    {
    private:
        string m_operands;

        virtual string msg() const override
        {
            std::ostringstream ss;
            ss << "Invalid operands: ";
            ss << m_operands;
            return ss.str();
        }

    public:
        DefineException(OperandException) using ArgumentException::ArgumentException;

        OperandException(const std::vector<const Variant *> &operands)
        {
            std::ostringstream ss;
            bool first = true;
            for (auto &&item : operands)
            {
                if (first)
                {
                    first = false;
                }
                else
                {
                    ss << ", ";
                }
                ss << Variant::stringifyType(item->type());
            }
            m_operands = ss.str();
        }
    };

    class MemoryException : public Exception
    {
    private:
        virtual string msg() const override
        {
            return "Out of memory.";
        }

    public:
        DefineException(MemoryException) using Exception::Exception;
    };

    class ReferenceException : public Exception
    {
    private:
        virtual string msg() const override
        {
            return "Invalid reference.";
        }

    public:
        DefineException(ReferenceException) using Exception::Exception;
    };

    class TypeException : public Exception
    {
    private:
        string m_func;
        Variant::Type m_type;

        virtual string msg() const override
        {
            return std::format("Invalid type '{}' for operating on '{}'.", Variant::stringifyType(m_type), m_func);
        }

    public:
        using Exception::Exception;
        TypeException(Variant::Type type, const string &func) : m_func(func), m_type(type) {}
    };

    class FileException : public Exception
    {
    private:
        string m_filename;

        virtual string msg() const override
        {
            return std::format("Unable to open the file {}.", m_filename);
        }

    public:
        DefineException(FileException) using Exception::Exception;
        FileException(const string &filename) : m_filename(filename) {}
    };

    class SyntaxException: public Exception
    {
    private:
        virtual string msg() const override
        {
            return "Syntax Error.";
        }
    public:
        DefineException(SyntaxException) using Exception::Exception;
    };

    class ConstantException: public SyntaxException
    {
        private:
            virtual string msg() const override
            {
                return "Invalid constant.";
            }
        public:
            DefineException(ConstantException) using SyntaxException::SyntaxException;
    };

    class RuntimeException: public Exception
    {
    private:
        virtual string msg() const override
        {
            return "Runtime Error.";
        }
    public:
        DefineException(RuntimeException) using Exception::Exception;
    };
} // namespace phi
