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
        return '\"' + getString() + '\"';
    case ID:
        return getString();
    case INT:
        return std::to_string(getInt());
    case REAL:
        return std::to_string(getReal());
    default:
        if (tag < 256)
            return string({(char)tag});
        else
            switch (tag) {
            case Tag::GE:
                return ">=";
            default:
                return static_cast<const Word &>(*this).word();
            }
    }
}
string Token::tagToString(Tag tag) {
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
        {"in", Tag::IN},
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
const int64_t &token::Token::getInt() const {
    return static_cast<const Integer &>(*this).num;
}
const real_t &token::Token::getReal() const {
    return static_cast<const Real &>(*this).num;
}
const string &token::Token::getString() const {
    return static_cast<const Word &>(*this).word();
}
const shared_ptr<string> &token::Token::getStringPtr() const {
    return static_cast<const Word &>(*this).wordPtr();
}
Word::Word(const string &w) : Word(w, Tag::ID) {}
std::map<string, shared_ptr<string>> Word::_M_words;
token::Word::Word(const string &w, uint16_t tag) : Token(tag) {
    if (_M_words.find(w) != _M_words.end())
        _M_word = _M_words[w];
    else
        _M_word = _M_words[w] = make_shared<string>(w);
}
} // namespace phi
