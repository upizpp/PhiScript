#pragma once

#include <exception.hpp>
#include <fstream>
#include <iostream>
#include <sstream>

namespace phi {
template <typename CharT> struct BasicScanner {
    virtual CharT get() = 0;
    virtual CharT peek() = 0;
    virtual void unget() = 0;
    virtual bool eof() = 0;
    virtual void reset() = 0;
    virtual const string &chunk() const = 0;
};

template <typename CharT> struct BasicStreamScanner : BasicScanner<CharT> {
    virtual CharT get() override { return getStream().get(); }
    virtual CharT peek() override { return getStream().peek(); }
    virtual void unget() override { getStream().unget(); }
    virtual bool eof() override {
        return getStream().eof() || peek() == (char)EOF;
    }
    virtual void reset() override { getStream().seekg(0); }

  private:
    virtual std::basic_istream<CharT> &getStream() = 0;
};

template <typename CharT>
struct BasicStringScanner : BasicStreamScanner<CharT> {
    BasicStringScanner(const std::basic_string<CharT> &str) : _M_stream(str) {
        static std::ostringstream ss;
        ss.str("");
        ss << "string_chunk(" << this << ") = ";
        _M_chunk = ss.str();
    }

    virtual const string &chunk() const { return _M_chunk; };

  private:
    std::basic_istream<CharT> &getStream() override { return _M_stream; }
    std::basic_istringstream<CharT> _M_stream;
    string _M_chunk;
};
using StringScanner = BasicStringScanner<char>;
using WStringScanner = BasicStringScanner<wchar_t>;
using String16Scanner = BasicStringScanner<char16_t>;
using String32Scanner = BasicStringScanner<char32_t>;

template <typename CharT> struct BasicFileScanner : BasicStreamScanner<CharT> {
    BasicFileScanner(const std::string &filename)
        : _M_stream(filename), _M_chunk(filename) {
        if (!_M_stream)
            throw FileException(filename);
    }
    virtual const string &chunk() const override { return _M_chunk; }

  private:
    std::basic_istream<CharT> &getStream() override { return _M_stream; }
    std::basic_ifstream<CharT> _M_stream;
    string _M_chunk;
};
using FileScanner = BasicFileScanner<char>;
using WFileScanner = BasicFileScanner<wchar_t>;
using File16Scanner = BasicFileScanner<char16_t>;
using File32Scanner = BasicFileScanner<char32_t>;

} // namespace phi
