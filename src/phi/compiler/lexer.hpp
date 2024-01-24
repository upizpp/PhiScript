#pragma once
#include <phi/compiler/scanner.hpp>
#include <phi/compiler/token.hpp>

namespace phi
{
    class Lexer
    {
    private:
        Owner<Scanner> _M_scanner;
        uinteger _M_line;

        char_t _M_peek;

        void read() { _M_peek = _M_scanner->get(); }
        bool read(char_t ch)
        {
            read();
            if (_M_peek != ch)
            {
                unget();
                return false;
            }
            _M_peek = '\0';
            return true;
        }
        void unget() { if (!_M_scanner->eof()) _M_scanner->unget(); }

    public:
        Lexer(Scanner *scanner) : _M_scanner(scanner), _M_line(0) {}

        uinteger line() const { return _M_line; }

        Ref<token::Token> getNextToken();
        list<Ref<token::Token>> getTokens();

        bool eof() { return _M_scanner->eof() || _M_peek == char_t(EOF); }
    };
} // namespace phi
