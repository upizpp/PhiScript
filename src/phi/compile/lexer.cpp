#include "lexer.hpp"
#include <array>
#include <cctype>
#include <follower.hpp>
#include <set>

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

static bool is_valid_double_operator_char(phi::char_t c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '%' ||
           c == '=' || c == '!' || c == '&' || c == '|' || c == '^' ||
           c == '~' || c == '<' || c == '>';
}

namespace phi {
using namespace token;
unique_ptr<Token> Lexer::next() {
    if (_M_peek == '\0')
        read();

    while (_M_peek == '\t' || _M_peek == ' ')
        read();
    if (_M_peek == '\n') {
        read();
        set_line(line + 1);
        return make_unique<Token>('\n');
    }
    if (_M_peek == EOF) {
        _M_eof = true;
        return make_unique<Token>(Tag::EOF);
    }

    if (std::isdigit(_M_peek)) {
        int64_t x = read_integer();
        if (_M_peek != '.')
            return make_unique<Integer>(x);
        read();
        real_t y = x;
        real_t d = 10;
        do {
            y = y + (_M_peek - '0') / d;
            d *= 10;
            read();
        } while (std::isdigit(_M_peek));
        return make_unique<Real>(y);
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
            return make_unique<Word>(ss.str(), Tag::STRING);
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
        } while (is_valid_identifier_char(_M_peek) || std::isdigit(_M_peek));
        string s = ss.str();
        if (auto res = Token::getKeyword(s))
            return res;
        return make_unique<Word>(s);
    }

    if (is_valid_double_operator_char(_M_peek)) {
        static const auto DoubleOperators = to_array<std::string_view>(
            {">=", "<=", "==", "!=", "&&", "||", "<<", ">>", "**", "->", "@@",
             "++", "--", "=>"});
        char_t tmp = _M_peek;
        std::set<uint8_t> operators;
        for (uint8_t i = 0; i < DoubleOperators.size(); ++i)
            operators.insert(i);

        int16_t result = -1;
        bool first = true;
        for (uint8_t i = 0; true; ++i, read()) {
            bool found = false;
            for (uint8_t j = 0; j < DoubleOperators.size(); ++j)
                if (_M_peek != DoubleOperators[j][i] &&
                    operators.find(j) != operators.end()) {
                    operators.erase(j);
                    found = true;
                }
            if (first && operators.empty())
                _M_peek = '\0';
            if (operators.size() == 1 &&
                i == DoubleOperators[*operators.begin()].size() - 1 &&
                result == -1) {
                result = *operators.begin();
                _M_peek = '\0';
            }
            if (!found || operators.size() == 0)
                break;
            first = false;
        }
        if (result == -1)
            return make_unique<Token>(tmp);
        return Token::getDoubleOperator({DoubleOperators[result].data()});
    }
    char_t tmp = _M_peek;
    read();
    return make_unique<Token>(tmp);
}
void Lexer::set_line(uint64_t v) {
    line = v;
    ProgramFollower::get().line = line;
}
void Lexer::read() { _M_scanner >> _M_peek; }
bool Lexer::read(char_t expected) {
    read();
    if (_M_peek == expected) {
        _M_peek = '\0';
        return true;
    } else {
        return false;
    }
}
int64_t Lexer::read_integer() {
    int64_t x = 0;
    do {
        x = x * 10 + (_M_peek - '0');
        read();
    } while (std::isdigit(_M_peek));
    return x;
}
} // namespace phi