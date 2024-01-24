#pragma once
#include <iostream>
#include <phi/typedef.hpp>

namespace phi
{
    namespace token
    {
        using tag_t = int_least16_t;
        enum Tag : tag_t
        {
            ID = 256,
            INT,
            REAL,
            // operators
            AND,
            OR,
            LE,
            GE,
            EQ,
            NE,
            // keywords
            IF,
            ELSE,
            FOR,
            WHILE,
            DO,
            BREAK,
            CONTINUE,
            TRUE,
            FALSE,
            IMPORT
        };

        class Token
        {
        private:
            tag_t _M_tag;

        public:
            Token() : _M_tag(0) {}
            explicit Token(tag_t tag) : _M_tag(tag) {}

            tag_t tag() const { return _M_tag; }

            virtual operator string() const
            {
                if (_M_tag < 256)
                    return '(' + string(1, char(_M_tag)) + ')';
                else
                    return '(' + std::to_string(_M_tag) + ')';
            }

            const Token *reidentify() const;
            Token *reidentify();
        };

        class Integer : public Token
        {
        private:
            integer _M_value;

        public:
            explicit Integer(integer value) : Token(Tag::INT), _M_value(value) {}

            operator string() const { return '[' + std::to_string(_M_value) + ']'; }
        };

        class Real : public Token
        {
        private:
            real _M_value;

        public:
            explicit Real(real value) : Token(Tag::REAL), _M_value(value) {}

            operator string() const { return "[" + std::to_string(_M_value) + ']'; }
        };

        class Word : public Token
        {
        private:
            string _M_value;

            static map<string, Ref<Word>> _M_words;

        public:
            explicit Word(string value, tag_t tag) : Token(tag), _M_value(value) {}

            static void put(const string &);
            static bool has(const string &);
            static Ref<Word> get(const string &);

            operator string() const { return "{" + _M_value + "}"; }
        };
    } // namespace token

    inline std::ostream &operator<<(std::ostream &os, const token::Token &token)
    {
        return os << token.reidentify()->operator std::string();
    }
} // namespace phi
