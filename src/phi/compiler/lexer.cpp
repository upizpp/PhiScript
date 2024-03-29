#include "lexer.hpp"
#include <cctype>
#include <phi/runtime/follower.hpp>
#include <phi/singleton.hpp>
using namespace phi::token;

static uint8_t byte_length(phi::char_t b) {
    if (b > 239)
        return 4;
    if (b > 223)
        return 3;
    if (b > 128)
        return 2;
    return 1;
}

namespace phi {
    void Lexer::skipWhitespace() {
        while (true) {
            read();
            if (_M_peek == '\n')
                ++_M_line;
            else if (std::isspace(_M_peek))
                continue;
            else
                break;
        }
    }

    void Lexer::checkEOF() {
        char_t peek = _M_peek;
        skipWhitespace();
        unget();
        // if (!read(EOF))
        _M_peek = peek;
    }

    Ref<Token> Lexer::getNextTokenImpl() {
        using R = Ref<Token>;
        static std::ostringstream os;
        skipWhitespace();

        Singleton<ProgramFollower>::instance()->position(
            {"__lexer__", _M_line, _M_scanner->chunk()});

        {
            R res;
            uinteger l = line();
            switch (_M_peek) {
            case '&':
                return res =
                           (read('&') ? R(Word::get("&&")) : R(new Token('&'))),
                       checkEOF(), res;
            case '|':
                return res =
                           (read('|') ? R(Word::get("||")) : R(new Token('|'))),
                       checkEOF(), res;
            case '=':
                return res = (read('=')   ? R(Word::get("=="))
                              : read('>') ? R(Word::get("=>"))
                                          : R(new Token('='))),
                       checkEOF(), res;
            case '!':
                return res =
                           (read('=') ? R(Word::get("!=")) : R(new Token('!'))),
                       checkEOF(), res;
            case '<':
                return res = (read('=')   ? R(Word::get("<="))
                              : read('<') ? R(Word::get("<<"))
                                          : R(new Token('<'))),
                       checkEOF(), res;
            case '>':
                return res = (read('=')   ? R(Word::get(">="))
                              : read('>') ? R(Word::get(">>"))
                                          : R(new Token('>'))),
                       checkEOF(), res;
            case '+':
                return res =
                           (read('+') ? R(Word::get("++")) : R(new Token('+'))),
                       checkEOF(), res;
            case '-':
                return res =
                           (read('-') ? R(Word::get("--")) : R(new Token('-'))),
                       checkEOF(), res;
            case '*':
                return res =
                           (read('*') ? R(Word::get("**")) : R(new Token('*'))),
                       checkEOF(), res;
            case '@':
                return res =
                           (read('@') ? R(Word::get("@@")) : R(new Token('@'))),
                       checkEOF(), res;
            }
        }

        if (std::isdigit(_M_peek)) {
            integer x = 0;
            do {
                x = x * 10 + _M_peek - '0';
                read();
            } while (std::isdigit(_M_peek));
            if (_M_peek != '.') {
                unget();
                checkEOF();
                return new Integer{x};
            }
            real y = x;
            real d = 10;
            read();
            do {
                y = y + (_M_peek - '0') / d;
                d *= 10;
                read();
            } while (std::isdigit(_M_peek));
            unget();
            checkEOF();
            return new Real{y};
        }

        if (_M_peek != char_t(EOF) &&
            (isalpha(_M_peek) || _M_peek == '_' || byte_length(_M_peek) > 1)) {
            os.str("");
            do {
                uint8_t len = byte_length(_M_peek);
                if (len > 1) {
                    for (size_t i = 0; i < len; i++, read())
                        os << _M_peek;
                    unget();
                } else
                    os << _M_peek;
                read();
            } while (!eof() &&
                     (isalpha(_M_peek) || _M_peek == '_' ||
                      std::isdigit(_M_peek) || byte_length(_M_peek) > 1));
            unget();
            string s = os.str();
            if (!Word::has(s))
                Word::put(s);
            uinteger l = line();
            checkEOF();
            return Word::get(s);
        }

#define STRING_IMPL(terminal)                                                  \
    if (_M_peek == terminal) {                                                 \
        os.str("");                                                            \
        read();                                                                \
        if (_M_peek == terminal)                                               \
            return new Word{"", Tag::STRING};                                  \
        do {                                                                   \
            char_t ch = _M_peek;                                               \
            if (eof())                                                         \
                throw SyntaxException("Unexpected EOF. (Bad String)");         \
            if (_M_peek == '\\') {                                             \
                read();                                                        \
                switch (_M_peek) {                                             \
                case 'n':                                                      \
                    ch = '\n';                                                 \
                    break;                                                     \
                case 't':                                                      \
                    ch = '\t';                                                 \
                    break;                                                     \
                case 'r':                                                      \
                    ch = '\r';                                                 \
                    break;                                                     \
                case 'b':                                                      \
                    ch = '\b';                                                 \
                    break;                                                     \
                case 'f':                                                      \
                    ch = '\f';                                                 \
                    break;                                                     \
                case 'a':                                                      \
                    ch = '\a';                                                 \
                    break;                                                     \
                case '\\':                                                     \
                    ch = '\\';                                                 \
                    break;                                                     \
                case '0':                                                      \
                    ch = '\0';                                                 \
                    break;                                                     \
                case '\'':                                                     \
                    ch = '\'';                                                 \
                    break;                                                     \
                case '"':                                                      \
                    ch = '"';                                                  \
                    break;                                                     \
                default:                                                       \
                    throw SyntaxException("Bad escape symbol.");               \
                }                                                              \
            }                                                                  \
            os << ch;                                                          \
            read();                                                            \
        } while (_M_peek != terminal);                                         \
        checkEOF();                                                            \
        return new Word{os.str(), Tag::STRING};                                \
    }
        STRING_IMPL('"');
        STRING_IMPL('\'');

        uinteger l = line();
        checkEOF();
        return new Token(_M_peek);
    }

    Ref<token::Token> Lexer::getNextToken() {
        uinteger l = line();
        return getNextTokenImpl()->line(l)->chunk(_M_scanner->chunk());
    }

    tokens Lexer::getTokens() {
        tokens tokens;
        while (!eof()) {
            auto token = getNextToken();
            if (token->tag() == char_t(EOF))
                break;
            tokens.push_back(token);
        }
        return tokens;
    }

} // namespace phi
