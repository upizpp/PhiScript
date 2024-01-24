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

#define CHECK_EOF              \
    {                          \
        char_t peek = _M_peek; \
        read();                \
        if (!read(EOF))        \
            unget();           \
        _M_peek = peek;        \
    }

namespace phi
{
    Ref<Token> Lexer::getNextToken()
    {
        using R = Ref<Token>;
        static std::ostringstream os;
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

        if (isalpha(_M_peek) || _M_peek == '_' || byte_length(_M_peek) > 1)
        {
            os.str("");
            do
            {
                uint8_t len = byte_length(_M_peek);
                if (len > 1)
                {
                    for (size_t i = 0; i < len; i++, read())
                        os << _M_peek;
                    unget();
                }
                else
                    os << _M_peek;
                read();
            } while (!eof() && (isalpha(_M_peek) || _M_peek == '_' || std::isdigit(_M_peek) || byte_length(_M_peek) > 1));
            unget();
            string s = os.str();
            if (!Word::has(s))
                Word::put(s);
            return Word::get(s);
        }

#define STRING_IMPL(terminal)                                    \
    if (_M_peek == terminal)                                     \
    {                                                            \
        os.str("");                                              \
        read();                                                  \
        do                                                       \
        {                                                        \
            char_t ch = _M_peek;                                 \
            if (_M_peek == '\\')                                 \
            {                                                    \
                read();                                          \
                switch (_M_peek)                                 \
                {                                                \
                case 'n':                                        \
                    ch = '\n';                                   \
                    break;                                       \
                case 't':                                        \
                    ch = '\t';                                   \
                    break;                                       \
                case 'r':                                        \
                    ch = '\r';                                   \
                    break;                                       \
                case 'b':                                        \
                    ch = '\b';                                   \
                    break;                                       \
                case 'f':                                        \
                    ch = '\f';                                   \
                    break;                                       \
                case 'a':                                        \
                    ch = '\a';                                   \
                    break;                                       \
                case '\\':                                       \
                    ch = '\\';                                   \
                    break;                                       \
                case '0':                                        \
                    ch = '\0';                                   \
                    break;                                       \
                case '\'':                                       \
                    ch = '\'';                                   \
                    break;                                       \
                case '"':                                        \
                    ch = '"';                                    \
                    break;                                       \
                default:                                         \
                    throw SyntaxException("Bad escape symbol."); \
                }                                                \
            }                                                    \
            os << ch;                                            \
            read();                                              \
        } while (_M_peek != terminal);                           \
        CHECK_EOF;                                               \
        string s = os.str();                                     \
        if (!Word::has(s))                                       \
            Word::put(s, Tag::STRING);                           \
        return Word::get(s);                                     \
    }
        STRING_IMPL('"');
        STRING_IMPL('\'');

        CHECK_EOF;
        return new Token(_M_peek);
    }

    list<Ref<token::Token>> Lexer::getTokens()
    {
        list<Ref<token::Token>> tokens;
        while (!eof())
            tokens.push_back(getNextToken());
        return tokens;
    }

} // namespace phi
