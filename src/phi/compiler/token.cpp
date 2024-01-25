#include "token.hpp"

namespace phi
{
    namespace token
    {
#define WORD(what, tag)                   \
    {                                     \
        what, new Word { what, Tag::tag } \
    }

        map<string, Ref<string>> Word::_M_symbols;
        map<string, Ref<Word>> Word::_M_words{
            WORD("if", IF),
            WORD("else", ELSE),
            WORD("for", FOR),
            WORD("while", WHILE),
            WORD("do", DO),
            WORD("break", BREAK),
            WORD("continue", CONTINUE),
            WORD("import", IMPORT),
            WORD("as", AS),
            WORD("var", VAR),
            WORD("eval", EVAL),
            WORD("true", TRUE),
            WORD("false", FALSE),
            WORD("&&", AND),
            WORD("||", OR),
            WORD("==", EQ),
            WORD("!=", NE),
            WORD(">=", GE),
            WORD("<=", LE),
            WORD("++", INC),
            WORD("--", RED),
        };

        void Word::put(const string &word)
        {
            _M_words.insert({word, new Word(word, Tag::ID)});
        }
        void Word::put(const string &word, Tag tag)
        {
            _M_words.insert({word, new Word(word, tag)});
        }
        bool Word::has(const string &word)
        {
            return _M_words.find(word) != _M_words.end();
        }
        void Word::value(const string &value)
        {
            if (_M_symbols.find(value) == _M_symbols.end())
            {
                _M_symbols[value] = new string{value};
            }
            _M_value = _M_symbols[value];
        }
        size_t Word::hash() const
        {
            return tag() + std::hash<string>()(*_M_value);
        }
        Ref<Word> Word::get(const string &word)
        {
            return new Word{*_M_words[word]};
        }

#define REIDENTIFY_IMPL         \
    switch (_M_tag)             \
    {                           \
    case Tag::ID:               \
    case Tag::STRING:           \
    case Tag::VAR:              \
    case Tag::IF:               \
    case Tag::ELSE:             \
    case Tag::FOR:              \
    case Tag::WHILE:            \
    case Tag::DO:               \
    case Tag::BREAK:            \
    case Tag::CONTINUE:         \
    case Tag::TRUE:             \
    case Tag::FALSE:            \
    case Tag::IMPORT:           \
    case Tag::AS:               \
    case Tag::EVAL:             \
    case Tag::AND:              \
    case Tag::OR:               \
    case Tag::LE:               \
    case Tag::GE:               \
    case Tag::EQ:               \
    case Tag::NE:               \
    case Tag::INC:              \
    case Tag::RED:              \
        return (Word *)this;    \
    case Tag::REAL:             \
        return (Real *)this;    \
    case Tag::INT:              \
        return (Integer *)this; \
    }                           \
    return this

        Token *Token::reidentify()
        {
            REIDENTIFY_IMPL;
        }

        const Token *Token::reidentify() const
        {

            REIDENTIFY_IMPL;
        }

        bool Token::operator==(const Token &token) const
        {
            return equals(*this, token);
        }

        size_t Token::hash() const
        {
            return tag();
        }

        size_t Integer::hash() const
        {
            return tag() + value();
        }

        size_t Real::hash() const
        {
            return tag() + value();
        }

        string toString(tag_t tag)
        {
            if (tag < 256)
                return string(1, char(tag));
            switch (tag)
            {
            case Tag::ID:
                return "identifier";
            case Tag::STRING:
                return "string";
            case Tag::INT:
                return "integer";
            case Tag::REAL:
                return "real";
            case Tag::VAR:
                return "var";
            case Tag::IF:
                return "if";
            case Tag::ELSE:
                return "else";
            case Tag::FOR:
                return "for";
            case Tag::WHILE:
                return "while";
            case Tag::DO:
                return "do";
            case Tag::BREAK:
                return "break";
            case Tag::CONTINUE:
                return "continue";
            case Tag::TRUE:
                return "true";
            case Tag::FALSE:
                return "false";
            case Tag::IMPORT:
                return "import";
            case Tag::AS:
                return "as";
            case Tag::EVAL:
                return "eval";
            case Tag::AND:
                return "&&";
            case Tag::OR:
                return "||";
            case Tag::EQ:
                return "==";
            case Tag::NE:
                return "!=";
            case Tag::GE:
                return ">=";
            case Tag::LE:
                return "<=";
            case Tag::INC:
                return "++";
            case Tag::RED:
                return "--";
            }
            return "unknown";
        }
    } // namespace token
} // namespace phi
