#pragma once

#include <fstream>
#include <istream>
#include <sstream>
#include <typedef.hpp>

namespace phi {
template <typename T> struct BasicScanner {
    virtual bool eof() = 0;
    virtual void reset() = 0;
    virtual T next() = 0;

    template <typename U>
    std::enable_if_t<std::is_convertible_v<U, T>, BasicScanner<T> &>
    operator>>(U &ch) {
        ch = next();
        return *this;
    }
};

using Scanner = BasicScanner<char>;
using WScanner = BasicScanner<wchar_t>;

template <typename T> struct BasicIStreamScanner : BasicScanner<T> {
    virtual bool eof() override { return get_stream().eof(); }
    virtual void reset() override { get_stream().seekg(0); }
    virtual T next() override { return get_stream().get(); }

  private:
    virtual std::basic_istream<T> &get_stream() = 0;
};

template <typename T> struct BasicFileScanner : BasicIStreamScanner<T> {
    BasicFileScanner(const std::string &filename) : _M_file(filename) {}

  private:
    virtual std::basic_istream<T> &get_stream() override { return _M_file; }
    std::basic_ifstream<T> _M_file;
};
using FileScanner = BasicFileScanner<char>;
using WFileScanner = BasicFileScanner<wchar_t>;

template <typename T> struct BasicStringScanner : BasicIStreamScanner<T> {
    BasicStringScanner(const std::basic_string<T> &str) : _M_str(str) {}

  private:
    virtual std::basic_istream<T> &get_stream() override { return _M_str; }
    std::basic_istringstream<T> _M_str;
};

using StringScanner = BasicStringScanner<char>;
using WStringScanner = BasicStringScanner<wchar_t>;

} // namespace phi
