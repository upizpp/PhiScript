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
            STRING,
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

            bool operator==(const Token& token) const;
        };

        class Integer : public Token
        {
        private:
            integer _M_value;

        public:
            explicit Integer(integer value) : Token(Tag::INT), _M_value(value) {}

            integer value() const { return _M_value; }

            operator string() const { return '[' + std::to_string(_M_value) + ']'; }
        };

        class Real : public Token
        {
        private:
            real _M_value;

        public:
            explicit Real(real value) : Token(Tag::REAL), _M_value(value) {}

            real value() const { return _M_value; }

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
            static void put(const string &, Tag);
            static bool has(const string &);
            static Ref<Word> get(const string &);

            const string &value() const { return _M_value; }

            void merge(const Word &word) { _M_value += word.value(); }

            operator string() const
            {
                if (tag() == Tag::STRING)
                    return '"' + _M_value + '"';
                else
                    return '{' + _M_value + '}';
            }
        };

        using tokens = list<Ref<Token>>;

        template <typename A, typename B>
        inline bool equals(const A &a, const B &b)
        {
            return false;
        }
        inline bool equals(const token::Token &a, const token::Token &b)
        {
            return a.tag() == b.tag();
        }
        inline bool equals(const token::Word &a, const token::Word &b)
        {
            return a.tag() == b.tag() && a.value() == b.value();
        }
        inline bool equals(const token::Integer &a, const token::Integer &b)
        {
            return a.tag() == b.tag() && a.value() == b.value();
        }
        inline bool equals(const token::Real &a, const token::Real &b)
        {
            return a.tag() == b.tag() && a.value() == b.value();
        }

    } // namespace token

    inline std::ostream &operator<<(std::ostream &os, const token::Token &token)
    {
        return os << token.reidentify()->operator std::string();
    }
} // namespace phi

namespace std
{
    template<>
    struct hash<phi::token::Token>
    {
        size_t operator()(const phi::token::Token &token) const
        {
            return size_t(&token);
        }
    };
}