#include "lexer.hpp"
#include <cctype>
using namespace phi::token;

static uint8_t byte_length(phi::char_t b)
{
    if (b > 239)
        return 4;
    if (b > 223)
        return 3;
    if (b > 128)
        return 2;
    return 1;
}

namespace phi
{
    Ref<Token> Lexer::getNextToken()
    {
        using R = Ref<Token>;
        // skip whitespace
        while (true)
        {
            read();
            if (_M_peek == '\n')
                ++_M_line;
            else if (std::isspace(_M_peek))
                continue;
            else
                break;
        }

        switch (_M_peek)
        {
        case '&':
            return read('&') ? R(Word::get("AND")) : R(new Token('&'));
        case '|':
            return read('|') ? R(Word::get("OR")) : R(new Token('|'));
        case '=':
            return read('=') ? R(Word::get("EQ")) : R(new Token('='));
        case '<':
            return read('=') ? R(Word::get("LE")) : R(new Token('<'));
        case '>':
            return read('=') ? R(Word::get("GE")) : R(new Token('>'));
        }

        if (std::isdigit(_M_peek))
        {
            integer x = 0;
            do
            {
                x = x * 10 + _M_peek - '0';
                read();
            } while (std::isdigit(_M_peek));
            if (_M_peek != '.')
            {
                unget();
                return new Integer{x};
            }
            real y = x;
            real d = 10;
            read();
            do
            {
                y = y + (_M_peek - '0') / d;
                d *= 10;
                read();
            } while (std::isdigit(_M_peek));
            unget();
            return new Real{y};
        }

        if (isalpha(_M_peek) || _M_peek == '_')
        {
            static std::ostringstream os;
            os.str("");
            do
            {
                os << _M_peek;
                read();
            } while (isalpha(_M_peek) || _M_peek == '_' || std::isdigit(_M_peek));
            unget();
            string s = os.str();
            if (!Word::has(s))
                Word::put(s);
            return Word::get(s);
        }

        char_t peek = _M_peek;
        read();
        if (!read(EOF)) unget();
        return new Token(peek);
    }

} // namespace phi
