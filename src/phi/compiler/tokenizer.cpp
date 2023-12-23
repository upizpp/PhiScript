#include "tokenizer.hpp"
#include "../exception.hpp"
#include "../phi_std.hpp"
#include "../phi_tool.hpp"
#include "../phi_entry.hpp"
#include "code_reader.hpp"
#include <cctype>
#include <cstring>
#include <sstream>

#define GET_IF(cond)                \
    while (cond)                    \
    {                               \
        ss << sentinel;             \
        sentinel = m_reader->get(); \
    }
#define GET_IF_B(cond, body)        \
    while (cond)                    \
    {                               \
        body;                       \
        ss << sentinel;             \
        sentinel = m_reader->get(); \
    }
#define PICK_IF(cond)                  \
    do                                 \
    {                                  \
        ss << sentinel;                \
        sentinel = m_reader->get();    \
    } while (cond);                    \
    m_reader->unget();                 \
    {                                  \
        std::string tmp = ss.str();    \
        if (sentinel == EOF)           \
        {                              \
            m_reader->clear();         \
        }                              \
        tmp.erase(tmp.end() - 1);      \
        ss.str(tmp);                   \
    }

namespace phi
{
    namespace compiler
    {
        Tokenizer::Tokenizer(CodeReader *reader) : m_reader(reader)
        {
            m_reader->setOwner(this);
        }

        Tokenizer::~Tokenizer()
        {
            delete m_reader;
        }

        void Tokenizer::nextColumn()
        {
            ++m_position.column;
        }

        void Tokenizer::nextLine()
        {
            m_eol = m_position.column;
            m_position.column = 1;
            ++m_position.line;
        }

        void Tokenizer::backward()
        {
            --m_position.column;
            if (m_position.column < 1)
            {
                m_position.column = m_eol;
                --m_position.line;
            }
        }

        bool Tokenizer::eof()
        {
            skipWhitespace();
            return m_reader->eof() || m_reader->peek() == EOF;
        }

        void Tokenizer::skipWhitespace()
        {
            // Cannot call this ->eof, otherwise it will be recursively called.
            if (m_reader->eof())
                return;
            char ch = m_reader->peek();
            bool flag = false;
            while (std::isspace(ch))
            {
                ch = m_reader->get();
                flag = true;
            }
            if (flag)
                m_reader->unget();
        }

        Token Tokenizer::getNextToken()
        {
            // White space characters have been skipped in the eof function
            if (eof())
                return Token(Token::Type::EOFT);

            std::ostringstream ss;

            Position pos = position();
            PhiSetPosition(pos);
            Token::Type type = Token::Type::MAX;
            char sentinel = m_reader->get();

            if (sentinel == EOF)
                return Token(Token::Type::EOFT);

            if (sentinel == '/')
            {
                char next = m_reader->peek();
                // single line comment
                if (next == '/')
                {
                    while (sentinel != '\n' && sentinel != EOF)
                    {
                        sentinel = m_reader->get();
                    }
                    sentinel = m_reader->get();
                    if (sentinel == EOF)
                    {
                        return Token(Token::Type::EOFT);
                    }
                    pos = position();
                }
                if (next == '*')
                {
                    while (!(sentinel == '*' && m_reader->peek() == '/') && sentinel != EOF)
                    {
                        sentinel = m_reader->get();
                    }
                    m_reader->get();
                    sentinel = m_reader->get();
                    if (sentinel == EOF)
                    {
                        return Token(Token::Type::EOFT);
                    }
                    skipWhitespace();
                    sentinel = m_reader->get();
                    if (sentinel == EOF)
                    {
                        m_reader->clear();
                        m_reader->unget();
                        sentinel = m_reader->get();
                        if (sentinel == EOF)
                            return Token(Token::Type::EOFT);
                    }
                    pos = position();
                }
            }

            if (isSymbol(sentinel))
            {
                type = Token::Type::SYMBOL;
                ss << sentinel;
                goto finish;
            }

            if (std::isdigit(sentinel) || (sentinel == '-' && std::isdigit(m_reader->peek())))
            {
                if (sentinel == '-')
                {
                    sentinel = m_reader->get();
                    ss << '-';
                }
                type = Token::Type::NUMBER;
                integer base = 10;
                if (sentinel == '0')
                {
                    if (std::isalpha(m_reader->peek()))
                    {
                        char base_ch = m_reader->get();
                        if (!isValidBase(base_ch))
                            throw ConstantException("Invalid base '%c'.", base);
                        base = getBase(base_ch);
                        sentinel = m_reader->get();
                        ss << base_ch;
                    }
                    else if (std::isdigit(m_reader->peek()))
                    {
                        ss << 'o';
                        base = 8;
                        sentinel = m_reader->get();
                    }
                }
                bool float_flag = false;
                GET_IF_B(isValidCharInBase(sentinel, base) || sentinel == '.', {
                    if (sentinel == '.')
                    {
                        if (base != 10)
                            throw ConstantException("Floating point constants other than base 10 are not allowed.");
                        if (float_flag)
                            throw ConstantException("There is a non unique decimal point in a number.");
                        float_flag = true;
                    }
                });
                m_reader->unget();
                goto finish;
            }

            if (sentinel == '"')
            {
                type = Token::Type::STRING;
                sentinel = m_reader->get();
                bool raw_mode = false;
                if (sentinel == '"' && m_reader->peek() == '"')
                {
                    m_reader->get();
                    sentinel = m_reader->get();
                    raw_mode = true;
                }
                GET_IF_B(sentinel != '"' || raw_mode, {
                    if (!raw_mode)
                    {
                        if (sentinel == '\n')
                            throw ConstantException("Broken string.");
                        if (sentinel == '\\')
                        {
                            char escape = m_reader->get();
                            switch (escape)
                            {
                            case 'a':
                                sentinel = '\a';
                                break;
                            case 'b':
                                sentinel = '\b';
                                break;
                            case 'f':
                                sentinel = '\f';
                                break;
                            case 'n':
                                sentinel = '\n';
                                break;
                            case 'r':
                                sentinel = '\r';
                                break;
                            case 't':
                                sentinel = '\t';
                                break;
                            case 'v':
                                sentinel = '\v';
                                break;
                            case '\'':
                                sentinel = '\'';
                                break;
                            case '\"':
                                sentinel = '\"';
                                break;
                            case '0':
                                sentinel = '\0';
                                break;
                            case '\\':
                                sentinel = '\\';
                                break;
                            default:
                                throw ConstantException("Unknown escape '\\%s'", escape);
                            }
                        }
                    }
                    else
                    {
                        if (sentinel == '\"' && m_reader->peek() == '\"')
                        {
                            m_reader->get();
                            if (m_reader->peek() == '\"')
                            {
                                sentinel = m_reader->get();
                                break;
                            }
                            else
                            {
                                m_reader->unget();
                            }
                        }
                    }
                });
                goto finish;
            }

            if (isOperator(toString(sentinel)))
            {
                type = Token::Type::OPERATOR;
                PICK_IF(isOperator(ss.str()));
                m_reader->unget();
                goto finish;
            }

            // and not std::isdigit(sentinel)
            if (isValidIdentifierChar(sentinel))
            {
                GET_IF(isValidIdentifierChar(sentinel));
                m_reader->unget();
                std::string s(ss.str());
                if (isKeyword(s))
                    type = Token::Type::KEYWORD;
                else if (s == "true" || s == "false")
                    type = Token::Type::BOOLEAN, ss.str(s == "true" ? "1" : "0");
                else
                    type = Token::Type::IDENTIFIER;
                goto finish;
            }

            throw SyntaxException("Unknown character '%c'.", sentinel);

        finish:
            return Token(pos, ss.str(), type);
        }

        std::list<Token> Tokenizer::getTokens()
        {
            m_reader->reset();
            std::list<Token> result;
            auto&& back_it = result.end();
            while (!eof())
            {
                back_it = result.insert(back_it, getNextToken());
                // m_reader->get(); 
                ++back_it;
            }
            --back_it;
            if (back_it->type() == Token::Type::EOFT)
                result.erase(back_it);
            return result;
        }

    } // namespace compiler
} // namespace phi
