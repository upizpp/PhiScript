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
            NOT,
            LE,
            GE,
            EQ,
            NE,
            INC, // increase
            RED, // reduce
            LSHIFT,
            RSHIFT,
            POW,
            DCPY, // deep copy
            // keywords
            RETURN,
            FN,
            VAR,
            IF,
            ELSE,
            FOR,
            WHILE,
            DO,
            BREAK,
            CONTINUE,
            TRUE,
            FALSE,
            IMPORT,
            AS,
            EVAL,
            DELETE,
            LIKELY,
            UNLIKELY
        };

        string toString(tag_t tag);

        class Token
        {
        private:
            Tag _M_tag;
            integer _M_line;

        public:
            Token() : _M_tag((Tag)0) {}
            explicit Token(tag_t tag) : _M_tag((Tag)tag) {}

            tag_t tag() const { return _M_tag; }

            virtual operator string() const
            {
                return '(' + token::toString(tag()) + ")\t\t\t- line: " + std::to_string(line());
            }
            virtual string toString() const
            {
                return '(' + token::toString(tag()) + ')';
            }
            virtual string stringify() const
            {
                return token::toString(tag());
            }

            const Token *reidentify() const;
            Token *reidentify();

            Token *line(integer l)
            {
                _M_line = l;
                return this;
            }
            integer line() const { return _M_line; }

            bool operator==(const Token &token) const;

            virtual size_t hash() const;
        };

        class Integer : public Token
        {
        private:
            integer _M_value;

        public:
            explicit Integer(integer value) : Token(Tag::INT), _M_value(value) {}

            integer value() const { return _M_value; }
            size_t hash() const override;

            operator string() const override
            {
                return '[' + std::to_string(_M_value) + "]\t\t\t- line: " + std::to_string(line());
            }
            virtual string toString() const override
            {
                return '[' + std::to_string(_M_value) + ']';
            }
            virtual string stringify() const override
            {
                return std::to_string(_M_value);
            }
        };

        class Real : public Token
        {
        private:
            real _M_value;

        public:
            explicit Real(real value) : Token(Tag::REAL), _M_value(value) {}

            real value() const { return _M_value; }
            size_t hash() const override;

            operator string() const override
            {
                return "[" + std::to_string(_M_value) + "]\t\t\t- line: " + std::to_string(line());
            }
            virtual string toString() const override
            {
                return '[' + std::to_string(_M_value) + ']';
            }
            virtual string stringify() const override
            {
                return std::to_string(_M_value);
            }
        };

        class Word : public Token
        {
        private:
            Ref<string> _M_value;

            static map<string, Ref<string>> _M_symbols;
            static map<string, Ref<Word>> _M_words;

        public:
            Word(const Word &word) : Token(word.tag())
            {
                value(word.value());
                line(word.line());
            }
            explicit Word(string v, tag_t tag) : Token(tag)
            {
                value(v);
            }

            static void put(const string &);
            static void put(const string &, Tag);
            static bool has(const string &);
            static Ref<Word> get(const string &);

            const string &value() const { return *_M_value; }
            void value(const string &value);
            Ref<string> valueP() { return _M_value; }
            size_t hash() const override;

            void merge(const Word &word) { value(value() + word.value()); }

            operator string() const override
            {
                if (tag() == Tag::STRING)
                    return '"' + *_M_value + "\"\t\t\t- line: " + std::to_string(line());
                else
                    return '{' + *_M_value + "}\t\t\t- line: " + std::to_string(line());
            }

            virtual string toString() const
            {
                if (tag() == Tag::STRING)
                    return '"' + *_M_value + '"';
                else
                    return '{' + *_M_value + '}';
            }

            virtual string stringify() const override
            {
                return value();
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
        return os << token.reidentify()->toString();
    }
} // namespace phi

namespace std
{
    template <>
    struct hash<phi::token::Token>
    {
        size_t operator()(const phi::token::Token &token) const
        {
            return token.hash();
        }
    };

    template <>
    struct equal_to<phi::Ref<phi::token::Token>>
    {
        bool operator()(const phi::Ref<phi::token::Token> &a, const phi::Ref<phi::token::Token> &b) const
        {
            return phi::token::equals(*a, *b);
        }
    };
}