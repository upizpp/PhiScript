#include "lexer.hpp"
#include <cctype>
#include <exception.hpp>
#include <sstream>

namespace phi {
unique_ptr<Token> Lexer::next() {
    static std::ostringstream os;
    os.str("");

    skip_blank();
    if (std::isdigit(_M_sentinel)) {
        while (std::isdigit(_M_sentinel)) {
            os << _M_sentinel;
            read();
        }
        return unique_ptr<Token>(new Integer(std::stoll(os.str())));
    }

    if (std::isalpha(_M_sentinel) || _M_sentinel == '_') {
        while (std::isalpha(_M_sentinel) || isdigit(_M_sentinel) ||
               _M_sentinel == '_') {
            os << _M_sentinel;
            read();
        }
        return unique_ptr<Token>(new Word(os.str()));
    }

#define STRING_SCAN(symbol)                                                    \
    if (_M_sentinel == symbol) {                                               \
        read();                                                                \
        while (_M_sentinel != symbol) {                                        \
            os << _M_sentinel;                                                 \
            read();                                                            \
                                                                               \
            if (_M_sentinel == '\\') {                                         \
                read();                                                        \
                switch (_M_sentinel) {                                         \
                case 'n':                                                      \
                    os << '\n';                                                \
                    break;                                                     \
                case 'a':                                                      \
                    os << '\a';                                                \
                    break;                                                     \
                case 'b':                                                      \
                    os << '\b';                                                \
                    break;                                                     \
                case 'f':                                                      \
                    os << '\f';                                                \
                    break;                                                     \
                case 'r':                                                      \
                    os << '\r';                                                \
                    break;                                                     \
                case 't':                                                      \
                    os << '\t';                                                \
                    break;                                                     \
                case 'v':                                                      \
                    os << '\v';                                                \
                    break;                                                     \
                case '\\':                                                     \
                    os << '\\';                                                \
                    break;                                                     \
                case '\"':                                                     \
                    os << '\"';                                                \
                    break;                                                     \
                case '\'':                                                     \
                    os << '\'';                                                \
                    break;                                                     \
                                                                               \
                default:                                                       \
                    throw SyntaxException("Invalid escape sequence.");         \
                }                                                              \
                read();                                                        \
            }                                                                  \
        }                                                                      \
        read();                                                                \
        return unique_ptr<Token>(new Word(os.str(), true));                    \
    }
    STRING_SCAN('\"')
    STRING_SCAN('\'')

    char tmp = _M_sentinel;
    read();

#define OPERATOR(first, second)                                                \
    if (tmp == #first[0] && _M_sentinel == #second[0]) {                       \
        read();                                                                \
        return unique_ptr<Token>(new Word(#first #second));                    \
    }

    OPERATOR(<, =);
    OPERATOR(>, =);
    OPERATOR(=, =);
    OPERATOR(!, =);
    OPERATOR(>, >);
    OPERATOR(<, <);
    OPERATOR(+, +);
    OPERATOR(-, -);
    OPERATOR(@, @);

    return unique_ptr<Token>(new Token(tmp));
}
void Lexer::read() {
    _M_sentinel = _M_scanner->get();
    if (_M_sentinel == EOF)
        if (!_M_eof) {
            _M_sentinel = 0;
            _M_eof = true;
        }
}
void Lexer::skip_blank() {
    while (std::isspace(_M_sentinel))
        read();
}
} // namespace phi
