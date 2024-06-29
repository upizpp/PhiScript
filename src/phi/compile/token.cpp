#include "token.hpp"
#include <map>
#include <sstream>

namespace phi {
using namespace token;

std::ostream &token::operator<<(std::ostream &os, const Token &token) {
    return os << token.toString();
}

string Token::toString() const {
    switch (id) {
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
        if (id < 256)
            return string({(char)id});
        else
            switch (id) {
            case Type::GE:
                return ">=";
            default:
                return static_cast<const Word &>(*this).word;
            }
    }
}
unique_ptr<Token> Token::getKeyword(const string &keyword) {
    static const std::map<string, Type> Keywords = {
        {"pass", Type::PASS},
        {"return", Type::RETURN},
        {"fn", Type::FN},
        {"var", Type::VAR},
        {"if", Type::IF},
        {"else", Type::ELSE},
        {"for", Type::FOR},
        {"while", Type::WHILE},
        {"do", Type::DO},
        {"break", Type::BREAK},
        {"continue", Type::CONTINUE},
        {"true", Type::TRUE},
        {"false", Type::FALSE},
        {"export", Type::EXPORT},
        {"import", Type::IMPORT},
        {"as", Type::AS},
        {"eval", Type::EVAL},
        {"delete", Type::DELETE},
        {"likely", Type::LIKELY},
        {"unlikely", Type::UNLIKELY},
    };
    if (Keywords.find(keyword) != Keywords.end())
        return unique_ptr<Token>(new Word(keyword, Keywords.at(keyword)));
    return unique_ptr<Token>();
}
unique_ptr<Token> Token::getDoubleOperator(const string &op) {
    static const std::map<string, Type> DoubleOperators = {
        {">=", Type::GE},     {"<=", Type::LE},     {"==", Type::EQ},
        {"!=", Type::NE},     {"&&", Type::AND},    {"||", Type::OR},
        {"<<", Type::LSHIFT}, {">>", Type::RSHIFT}, {"**", Type::POW},
        {"->", Type::ARROW},  {"@@", Type::DCPY},   {"++", Type::INC},
        {"--", Type::RED},
    };
    if (DoubleOperators.find(op) != DoubleOperators.end())
        return unique_ptr<Token>(new Word(op, DoubleOperators.at(op)));
    return unique_ptr<Token>();
}
} // namespace phi
