#include "token.hpp"
#include <map>
#include <sstream>

namespace phi {

std::ostream &token::operator<<(std::ostream &os, const Token &token) {
    return os << token.toString();
}

string token::Token::toString() const {
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
            return static_cast<const Word &>(*this).word;
    }
}
unique_ptr<token::Token> token::Token::getToken(const string &keyword) {
    static const std::map<string, token::Type> Keywords = {
        {"pass", token::Type::PASS},
        {"return", token::Type::RETURN},
        {"fn", token::Type::FN},
        {"var", token::Type::VAR},
        {"if", token::Type::IF},
        {"else", token::Type::ELSE},
        {"for", token::Type::FOR},
        {"while", token::Type::WHILE},
        {"do", token::Type::DO},
        {"break", token::Type::BREAK},
        {"continue", token::Type::CONTINUE},
        {"true", token::Type::TRUE},
        {"false", token::Type::FALSE},
        {"export", token::Type::EXPORT},
        {"import", token::Type::IMPORT},
        {"as", token::Type::AS},
        {"eval", token::Type::EVAL},
        {"delete", token::Type::DELETE},
        {"likely", token::Type::LIKELY},
        {"unlikely", token::Type::UNLIKELY},
    };
    if (Keywords.find(keyword) != Keywords.end())
        return unique_ptr<token::Token>(
            new Word(keyword, Keywords.at(keyword)));
    return unique_ptr<token::Token>();
}
} // namespace phi
