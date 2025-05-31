#include "token.hpp"
#include <iostream>
#include <string>

namespace phi {
std::map<string, Token::Tag> Word::_M_words{{
    {"if", Token::Tag::IF},         {"else", Token::Tag::ELSE},
    {"for", Token::Tag::FOR},       {"while", Token::Tag::WHILE},
    {"break", Token::Tag::BREAK},   {"continue", Token::Tag::CONTINUE},
    {"fn", Token::Tag::FN},         {"func", Token::Tag::FN},
    {"return", Token::Tag::RETURN}, {"class", Token::Tag::CLASS},
    {"import", Token::Tag::IMPORT}, {"export", Token::Tag::EXPORT},
    {"as", Token::Tag::AS},         {"and", Token::Tag::AND},
    {"or", Token::Tag::OR},         {"not", Token::Tag::NOT},
    {"@@", Token::Tag::DCP},        {"++", Token::Tag::INC},
    {"--", Token::Tag::DEC},        {"==", Token::Tag::EQ},
    {"!=", Token::Tag::NE},         {"<=", Token::Tag::LE},
    {">=", Token::Tag::GE},         {"<<", Token::Tag::LSHIFT},
    {">>", Token::Tag::RSHIFT},
}};
inline const string &Token::readString() const { return as<Word>().value(); }
inline int_t Token::readInt() const { return as<Integer>().value(); }
inline real_t Token::readReal() const { return as<Real>().value(); }
string Token::stringify() const {
    if ((int)tag() == EOF)
        return "{EOF}";
    if ((int)tag() < 256)
        return string{(char)tag()};
    switch (tag()) {
    case Tag::INT:
        return "[" + std::to_string(readInt()) + "]";
    case Tag::REAL:
        return "[" + std::to_string(readReal()) + "]";
    case Tag::STRING:
        return "\"" + readString() + "\"";
    case Tag::IDENTIFIER:
        return readString();
    default:
        return "{" + readString() + "}";
    }
}
std::ostream &operator<<(std::ostream &os, const Token &token) {
    return os << token.stringify();
}
} // namespace phi
