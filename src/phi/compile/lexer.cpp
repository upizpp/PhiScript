#include "lexer.hpp"
#include <cctype>
#include <follower.hpp>

constexpr phi::char_t EOF = 255;

static uint8_t byte_length(phi::char_t b) {
    if (b > 239)
        return 4;
    if (b > 223)
        return 3;
    if (b > 128)
        return 2;
    return 1;
}

static bool is_valid_identifier_char(phi::char_t c) {
    return c != EOF &&
           (isalpha(c) || c == '$' || c == '_' || byte_length(c) > 1);
}

namespace phi {

unique_ptr<token::Token> phi::Lexer::next() {
    while (_M_peek == '\t' || _M_peek == ' ')
        read();
    if (_M_peek == '\n') {
        read();
        set_line(line + 1);
        return make_unique<token::Token>('\n');
    }
    if (_M_peek == EOF) {
        _M_eof = true;
        return make_unique<token::Token>(token::Type::EOF);
    }

    if (std::isdigit(_M_peek)) {
        int64_t x = read_integer();
        if (_M_peek != '.')
            return make_unique<token::Integer>(x);
        read();
        real_t y = x;
        real_t d = 10;
        do {
            y = y + (_M_peek - '0') / d;
            d *= 10;
            read();
        } while (std::isdigit(_M_peek));
        return make_unique<token::Real>(y);
    }

    constexpr char_t symbols[] = {'\'', '\"'};
    for (char_t c : symbols) {
        if (_M_peek == c) {
            read();
            std::ostringstream ss;
            while (_M_peek != c) {
                if (_M_peek == '\\') {
                    read();
                    switch (_M_peek) {
                    case 'a':
                        _M_peek = '\a';
                        break;
                    case 'n':
                        _M_peek = '\n';
                        break;
                    case 't':
                        _M_peek = '\t';
                        break;
                    case 'r':
                        _M_peek = '\r';
                        break;
                    case '\\':
                        _M_peek = '\\';
                        break;
                    case '\"':
                        _M_peek = '\"';
                        break;
                    case '\'':
                        _M_peek = '\'';
                        break;
                    case '0':
                        _M_peek = '\0';
                        break;
                    }
                }
                ss << _M_peek;
                read();
            }
            read();
            return make_unique<token::Word>(ss.str(), token::Type::STRING);
        }
    }

    if (is_valid_identifier_char(_M_peek)) {
        std::ostringstream ss;
        do {
            uint8_t len = byte_length(_M_peek);
            if (len == 1) {
                ss << _M_peek;
                read();
                continue;
            }
            for (size_t i = 0; i < len; i++, read())
                ss << _M_peek;
        } while (is_valid_identifier_char(_M_peek));
        string s = ss.str();
        if (auto res = token::Token::getToken(s))
            return res;
        return make_unique<token::Word>(s);
    }

    char_t tmp = _M_peek;
    read();
    return make_unique<token::Token>(tmp);
}
void Lexer::set_line(uint64_t v) {
    line = v;
    ProgramFollower::get().line = line;
}
void Lexer::read() { _M_scanner >> _M_peek; }
int64_t Lexer::read_integer() {
    int64_t x = 0;
    do {
        x = x * 10 + (_M_peek - '0');
        read();
    } while (std::isdigit(_M_peek));
    return x;
}
} // namespace phi