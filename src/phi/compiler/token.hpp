#pragma once
#include "../data_type.hpp"
#include "../reference.hpp"
#include <iostream>
#include <list>
#include <string>
#include <set>

namespace phi
{
    using std::string;
    namespace compiler
    {
        struct Position
        {
            integer line = 1;
            integer column = 1;
            Ref<string> filename;

            string stringify() const;
        };

        class Token
        {
        public:
            typedef std::list<Token> Container;
            enum class Type: uint8_t
            {
                KEYWORD,
                IDENTIFIER,
                SYMBOL,
                OPERATOR,
                STRING,
                NUMBER,
                BOOLEAN,
                EOFT,
                MAX
            };
            enum class KeywordType: uint8_t
            {
                INVALID,
                VAR,
                IF,
                ELSE,
                FOR,
                WHILE,
                FUNC,
                IMPORT,
                AS,
                EXPORT,
                BREAK,
                CONTINUE,
                FUNCTION,
                RETURN,
                MAX
            };

        private:
            const string* m_content = nullptr;
            Position m_position;
            Type m_type;

            static std::set<std::string> symbolsTable;

        public:
            Token() {}
            Token(const Type &type) : m_type(type) {}
            Token(const Position &pos) : m_position(pos) {}
            Token(const Position &position, const string &content, const Type &type);

            Position &position() { return m_position; }
            const Position &position() const { return m_position; }
            const string &content() const { return *m_content; }
            Type &type() { return m_type; }
            const Type &type() const { return m_type; }

            bool isPunctuationType() const { return (m_type == Type::SYMBOL && !(content() == ")" || content() == "]" || content() == "}")) || m_type == Type::OPERATOR; }
            KeywordType keywordType() const;
            uinteger priority() const;
            string stringify() const;

            bool operator==(const Token &token) const { return m_content == token.m_content; }

            static string stringifyType(const Type &);
        };

        std::ostream &operator<<(std::ostream &, const Position &);
        std::ostream &operator<<(std::ostream &, const Token &);
    } // namespace compiler
} // namespace phi
