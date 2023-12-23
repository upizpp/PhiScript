#include "token.hpp"
#include <map>
#include <sstream>

namespace phi
{
    namespace compiler
    {
        std::set<std::string> Token::symbolsTable;

        Token::Token(const Position &position, const string &content, const Type &type) : m_position(position), m_type(type)
        {
            if (symbolsTable.find(content) == symbolsTable.end())
                symbolsTable.insert(content);
            m_content = &*symbolsTable.find(content);
        }

        string Token::stringify() const
        {
            std::ostringstream ss;
            ss << m_content;
            ss << "\t-\t";
            ss << stringifyType(type());
            ss << "\t-\t";
            ss << m_position.stringify();
            return ss.str();
        }

        string Token::stringifyType(const Type &type)
        {
            switch (type)
            {
            case Type::KEYWORD:
                return "KEYWORD";
            case Type::IDENTIFIER:
                return "IDENTIFIER";
            case Type::SYMBOL:
                return "SYMBOL";
            case Type::OPERATOR:
                return "OPERATOR";
            case Type::STRING:
                return "STRING";
            case Type::NUMBER:
                return "NUMBER";
            case Type::BOOLEAN:
                return "BOOLEAN";
            case Type::EOFT:
                return "EOFT";
            case Type::MAX:
                return "MAX";
            default:
                return "UNKNOWN";
            }
        }

        std::map<std::string, uinteger> priorityMap = {
            {"==", 0},
            {">=", 0},
            {"<=", 0},
            {">", 0},
            {"<", 0},
            {"=", 1},
            {"||", 2},
            {"^^", 2},
            {"&&", 3},
            {"!", 4},
            {"~", 5},
            {"|", 6},
            {"^", 7},
            {"&", 8},
            {">>", 9},
            {"<<", 9},
            {"+", 10},
            {"-", 10},
            {"*", 11},
            {"/", 11},
            {"%", 11},
            {".", 12},
            {"(", 1000},
        };
        std::map<std::string, Token::KeywordType> keywordMap = {
            {"var", Token::KeywordType::VAR},
            {"if", Token::KeywordType::IF},
            {"else", Token::KeywordType::ELSE},
            {"for", Token::KeywordType::FOR},
            {"while", Token::KeywordType::WHILE},
            {"func", Token::KeywordType::FUNC},
            {"import", Token::KeywordType::IMPORT},
            {"as", Token::KeywordType::AS},
            {"export", Token::KeywordType::EXPORT},
            {"break", Token::KeywordType::BREAK},
            {"continue", Token::KeywordType::CONTINUE},
            {"fn", Token::KeywordType::FUNCTION},
            {"return", Token::KeywordType::RETURN},
        };

        Token::KeywordType Token::keywordType() const
        {
            if (type() != Type::KEYWORD)
                return KeywordType::INVALID;
            return keywordMap[content()];
        }

        uinteger Token::priority() const
        {
            auto it = priorityMap.find(content());
            if (it == priorityMap.end())
                return -1;
            return it->second;
        }

        string Position::stringify() const
        {
            std::ostringstream ss;
            ss << '"' << filename << '"' << ':';
            ss << line << ":";
            ss << column;
            return ss.str();
        }

        std::ostream &operator<<(std::ostream &os, const Position &pos)
        {
            os << pos.stringify();
            return os;
        }

        std::ostream &operator<<(std::ostream &os, const Token &token)
        {
            os << token.stringify();
            return os;
        }
    } // namespace compiler
} // namespace phi
