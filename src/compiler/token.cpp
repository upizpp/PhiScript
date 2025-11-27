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
    {"var", Token::Tag::VAR},       {"true", Token::Tag::TRUE},
    {"false", Token::Tag::FALSE},   {"null", Token::Tag::NIL},
    {"as", Token::Tag::AS},         {"and", Token::Tag::AND},
    {"or", Token::Tag::OR},         {"not", Token::Tag::NOT},
    {"@@", Token::Tag::DCP},        {"++", Token::Tag::INC},
    {"--", Token::Tag::DEC},        {"==", Token::Tag::EQ},
    {"!=", Token::Tag::NE},         {"<=", Token::Tag::LE},
    {">=", Token::Tag::GE},         {"<<", Token::Tag::LSHIFT},
    {">>", Token::Tag::RSHIFT},     {"**", Token::Tag::POW},
}};
const std::map<Token::Tag, string> &get_rev_words() {
    static std::map<Token::Tag, string> rev_words;
    if (rev_words.empty()) {
        for (auto &[key, value] : Word::_M_words)
            rev_words[value] = key;
    }
    return rev_words;
}

inline const string &Token::readString() const { return as<Word>().value(); }
inline int_t Token::readInt() const { return as<Integer>().value(); }
inline real_t Token::readReal() const { return as<Real>().value(); }
string Token::stringify() const {
    switch ((uint16_t)tag()) {
    case 0:
        return "{EOF}";
    case '\n':
        return "{\\n}";
    }
    if ((uint16_t)tag() < 256)
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
string stringify(const Token::Tag &tag) {
    if (tag < 256)
        return string(1, (char)tag);
    auto rev_words = phi::get_rev_words();
    if (rev_words.count(tag))
        return rev_words.at(tag);
    switch (tag) {
    case Token::Tag::INT:
        return "INT";
    case Token::Tag::REAL:
        return "REAL";
    case Token::Tag::STRING:
        return "STRING";
    case Token::Tag::IDENTIFIER:
        return "IDENTIFIER";
    default:
        return "{" + stringify(tag) + "}";
    }
}
std::ostream &operator<<(std::ostream &os, const Token &token) {
    return os << token.stringify();
}
} // namespace phi
