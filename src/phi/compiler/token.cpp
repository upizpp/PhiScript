#include "token.hpp"

namespace phi
{
    namespace token
    {
#define WORD(what, tag)                   \
    {                                     \
        what, new Word { what, Tag::tag } \
    }
        map<string, Ref<Word>> Word::_M_words{
            WORD("if", IF),
            WORD("else", ELSE),
            WORD("for", FOR),
            WORD("while", WHILE),
            WORD("do", DO),
            WORD("break", BREAK),
            WORD("continue", CONTINUE),
            WORD("import", IMPORT),
            WORD("true", TRUE),
            WORD("false", FALSE),
            WORD("&&", AND),
            WORD("||", OR),
            WORD("==", EQ),
            WORD("!=", NE),
            WORD(">=", GE),
            WORD("<=", LE),
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
        size_t Word::hash() const
        {
            return tag() + std::hash<string>()(value());
        }
        Ref<Word> Word::get(const string &word)
        {
            return _M_words[word];
        }

#define REIDENTIFY_IMPL         \
    switch (_M_tag)             \
    {                           \
    case Tag::ID:               \
    case Tag::STRING:           \
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
    } // namespace token
} // namespace phi
