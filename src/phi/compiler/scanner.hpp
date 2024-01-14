#pragma once
#include <fstream>
#include <phi/exception.hpp>
#include <sstream>

namespace phi
{
    class Scanner
    {
    public:
        virtual char get() = 0;
        virtual char peek() = 0;
        virtual void unget() = 0;
        virtual void reset() = 0;
        virtual bool eof() = 0;
    };

    class StreamScanner : public Scanner
    {
    private:
        virtual std::istream &get_stream() = 0;

    public:
        virtual char get() override { return get_stream().get(); }
        virtual char peek() override { return get_stream().peek(); }
        virtual void unget() override { get_stream().unget(); }
        virtual bool eof() override { return get_stream().eof(); }
        virtual void reset() override
        {
            get_stream().clear();
            get_stream().seekg(0, std::ios::beg);
        }
    };

    class StringScanner : public StreamScanner
    {
    private:
        std::istringstream _M_is;
        virtual std::istream &get_stream() override { return _M_is; }

    public:
        explicit StringScanner(const std::string &str) : _M_is(str) {}
    };

    class FileScanner : public StreamScanner
    {
    private:
        std::ifstream _M_is;
        virtual std::istream &get_stream() override { return _M_is; }

    public:
        explicit FileScanner(const std::string &path) : _M_is(path)
        {
            if (!_M_is.good())
                throw FileException(path);
        }
    };
} // namespace phi
