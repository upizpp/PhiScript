#include "token.hpp"
#include <map>
#include <sstream>

namespace phi {
using namespace token;

std::ostream &token::operator<<(std::ostream &os, const Token &token) {
    return os << token.toString();
}

string Token::toString() const {
    switch (tag) {
    case EOF:
        return "<EOF>";
    case '\n':
        return "<ENDL>";
    case STRING:
        return '\"' + static_cast<const Word &>(*this).word + '\"';
    case ID:
        return static_cast<const Word &>(*this).word;
    case INT:
        return std::to_string(static_cast<const Integer &>(*this).num);
    case REAL:
        return std::to_string(static_cast<const Real &>(*this).num);
    default:
        if (tag < 256)
            return string({(char)tag});
        else
            switch (tag) {
            case Tag::GE:
                return ">=";
            default:
                return static_cast<const Word &>(*this).word;
            }
    }
}
string token::Token::tagToString(Tag tag) {
    switch (tag) {
    case EOF:
        return "<EOF>";
    case '\n':
        return "<ENDL>";
    case STRING:
        return "<string>";
    case ID:
        return "<identifier>";
    case INT:
        return "<int>";
    case REAL:
        return "<real>";
    default:
        if (tag < 256)
            return string({(char)tag});
        else
            return "<unknown>";
    }
}
unique_ptr<Token> Token::getKeyword(const string &keyword) {
    static const std::map<string, Tag> Keywords = {
        {"pass", Tag::PASS},
        {"return", Tag::RETURN},
        {"fn", Tag::FN},
        {"var", Tag::VAR},
        {"if", Tag::IF},
        {"else", Tag::ELSE},
        {"for", Tag::FOR},
        {"while", Tag::WHILE},
        {"do", Tag::DO},
        {"break", Tag::BREAK},
        {"continue", Tag::CONTINUE},
        {"true", Tag::TRUE},
        {"false", Tag::FALSE},
        {"export", Tag::EXPORT},
        {"import", Tag::IMPORT},
        {"as", Tag::AS},
        {"eval", Tag::EVAL},
        {"delete", Tag::DELETE},
        {"likely", Tag::LIKELY},
        {"unlikely", Tag::UNLIKELY},
    };
    if (Keywords.find(keyword) != Keywords.end())
        return unique_ptr<Token>(new Word(keyword, Keywords.at(keyword)));
    return unique_ptr<Token>();
}
unique_ptr<Token> Token::getDoubleOperator(const string &op) {
    static const std::map<string, Tag> DoubleOperators = {
        {">=", Tag::GE},     {"<=", Tag::LE},     {"==", Tag::EQ},
        {"!=", Tag::NE},     {"&&", Tag::AND},    {"||", Tag::OR},
        {"<<", Tag::LSHIFT}, {">>", Tag::RSHIFT}, {"**", Tag::POW},
        {"->", Tag::ARROW},  {"@@", Tag::DCPY},   {"++", Tag::INC},
        {"--", Tag::RED},
    };
    if (DoubleOperators.find(op) != DoubleOperators.end())
        return unique_ptr<Token>(new Word(op, DoubleOperators.at(op)));
    return unique_ptr<Token>();
}
} // namespace phi
