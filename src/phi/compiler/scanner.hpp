#pragma once
#include <fstream>
#include <phi/exception.hpp>
#include <sstream>

namespace phi {
    class Scanner {
      public:
        virtual char_t get() = 0;
        virtual char_t peek() = 0;
        virtual void unget() = 0;
        virtual void reset() = 0;
        virtual bool eof() = 0;
        virtual Ref<string> chunk() = 0;
    };

    class StreamScanner : public Scanner {
      private:
        virtual std::istream &get_stream() = 0;

      public:
        virtual char_t get() override { return get_stream().get(); }
        virtual char_t peek() override { return get_stream().peek(); }
        virtual void unget() override { get_stream().unget(); }
        virtual bool eof() override { return get_stream().eof(); }
        virtual void reset() override {
            get_stream().clear();
            get_stream().seekg(0, std::ios::beg);
        }
        string getLine() {
            string s;
            getline(get_stream(), s);
            return s;
        }
    };

    class StringScanner : public StreamScanner {
      private:
        std::istringstream _M_is;
        Ref<string> _M_chunk;
        virtual std::istream &get_stream() override { return _M_is; }

      public:
        explicit StringScanner(const std::string &str) : _M_is(str) {}
        StringScanner(const std::string &str, const std::string &chunk)
            : _M_chunk(new string{chunk}) {}

        virtual Ref<string> chunk() override { return _M_chunk; }
    };

    class FileScanner : public StreamScanner {
      private:
        std::ifstream _M_is;
        Ref<string> _M_filename;
        virtual std::istream &get_stream() override { return _M_is; }

      public:
        explicit FileScanner(const std::string &path)
            : _M_is(path), _M_filename(new string{path}) {
            if (!_M_is.good())
                throw FileException(path);
        }
        virtual Ref<string> chunk() override { return _M_filename; }
    };
} // namespace phi
