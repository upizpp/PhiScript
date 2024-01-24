#include "token.hpp"

namespace phi
{
    namespace token
    {
        map<string, Ref<Word>> Word::_M_words;
        map<string, Ref<Word>> test{
            {"if", new Word{"if", Tag::IF}},
            {"else", new Word{"else", Tag::ELSE}},
            {"for", new Word{"for", Tag::FOR}},
            {"while", new Word{"while", Tag::WHILE}},
            {"do", new Word{"do", Tag::DO}},
            {"break", new Word{"break", Tag::BREAK}},
            {"continue", new Word{"continue", Tag::CONTINUE}},
            {"import", new Word{"continue", Tag::IMPORT}},
            {"true", new Word{"true", Tag::TRUE}},
            {"false", new Word{"false", Tag::FALSE}},
            {"AND", new Word{"&&", Tag::AND}},
            {"OR", new Word{"||", Tag::OR}},
            {"EQ", new Word{"==", Tag::EQ}},
            {"NE", new Word{"!=", Tag::NE}},
            {"GE", new Word{">=", Tag::GE}},
            {"LE", new Word{"<=", Tag::LE}},
        };

        void Word::put(const string &word)
        {
            _M_words.insert({word, new Word(word, Tag::ID)});
        }
        bool Word::has(const string &word)
        {
            return _M_words.find(word) != _M_words.end();
        }
        Ref<Word> Word::get(const string &word)
        {
            return _M_words[word];
        }

        Token *Token::reidentify()
        {
            switch (_M_tag)
            {
            case Tag::ID:
                return (Word *)this;
            case Tag::REAL:
                return (Real *)this;
            case Tag::INT:
                return (Integer *)this;
            }
            return this;
        }
        const Token *Token::reidentify() const
        {

            switch (_M_tag)
            {
            case Tag::ID:
                return (Word *)this;
            case Tag::REAL:
                return (Real *)this;
            case Tag::INT:
                return (Integer *)this;
            }
            return this;
        }
    } // namespace token
} // namespace phi
