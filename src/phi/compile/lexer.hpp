#pragma once
#include <compile/scanner.hpp>
#include <compile/token_generator.hpp>

namespace phi {
struct Lexer : TokenGenerator {
    Lexer(Scanner &scanner) : _M_scanner(scanner), _M_peek(0), _M_eof(false) {
        set_line(1);
    }

    virtual unique_ptr<token::Token> next() override;
    virtual bool eof() override { return _M_eof; }
    virtual void reset() override {
        _M_eof = false;
        _M_scanner.reset();
    }
    virtual string chunk() override { return _M_scanner.chunk(); }

    uint64_t line;

  private:
    void set_line(uint64_t v);
    void read();
    bool read(char_t);
    int64_t read_integer();

    bool _M_eof;
    char_t _M_peek;
    Scanner &_M_scanner;
};

} // namespace phi
