#include "phi_std.hpp"
#include "variant.hpp"
#include <iostream>
#include <map>
#include <unordered_set>
#include <phi/exception.hpp>

namespace phi
{
    namespace compiler
    {
        typedef std::unordered_set<std::string> search_container;
        static const search_container keywords{
            "var",
            "if",
            "else",
            "for",
            "while",
            "func",
            "import",
            "as",
            "export",
            "break",
            "continue",
            "fn",
            "return"
        };
        static const search_container structured_keywords{
            "if",
            "else",
            "for",
            "while"
        };
        // Due to tokenizer implementation reasons,
        // for any non single character operator like 'abcd',
        // a valid operator 'abc' must exist.
        static const search_container operators{
            "+",
            "-",
            "*",
            "/",
            "%",
            "**",
            "-",
            "~",
            "|",
            "&",
            "^",
            "@",
            "@@",
            "<<",
            ">>",
            "==",
            "!=",
            "<",
            "<=",
            ">",
            ">=",
            "&&",
            "||",
            "!",
            "^^",
            "=",
            "+=",
            "-=",
            "*=",
            "/=",
            "%=",
            "&=",
            "|=",
            "^=",
            "<<=",
            ">>=",
            ".",
            "..",
            "..."
        };
        // The symbol must be a single character.
        static std::unordered_set<char> symbols{
            '(',
            ')',
            '{',
            '}',
            '[',
            ']',
            ';',
            ':',
            ',',
            '#',
            '\\',
        };

        static std::map<std::string, runtime::OPCode::Command> binaryCommands = {
#include "operator_info"
        };
        static std::map<std::string, runtime::OPCode::Command> unaryCommands = {
            {"-", runtime::OPCode::Command::NEG},
            {"~", runtime::OPCode::Command::BNOT},
            {"@", runtime::OPCode::Command::COPY},
            {"@@", runtime::OPCode::Command::COPYDEEP},
        };

        runtime::OPCode::Command getBinaryCommand(const std::string &op)
        {
            if (binaryCommands.find(op) == binaryCommands.end())
                throw SyntaxException("Unknown operator(%s).", op.c_str());
            return binaryCommands[op];
        }

        runtime::OPCode::Command getUnaryCommand(const std::string &op)
        {
            return unaryCommands[op];
        }

        bool isUnary(const std::string &op)
        {
            return unaryCommands.find(op) != unaryCommands.end();
        }

        bool isBinary(const std::string &op)
        {
            return binaryCommands.find(op) != binaryCommands.end();
        }

        bool isKeyword(const std::string &s)
        {
            return keywords.find(s) != keywords.end();
        }

        bool isStructuredKeyword(const std::string &s)
        {
            return structured_keywords.find(s) != structured_keywords.end();
        }

        bool isOperator(const std::string &s)
        {
            return operators.find(s) != operators.end();
        }

        bool isSymbol(char ch)
        {
            return symbols.find(ch) != symbols.end();
        }

        bool isValidBase(char ch)
        {
            return getBase(ch) != -1;
        }

        bool isValidIdentifierChar(char ch)
        {
            return std::isalpha(ch) || std::isdigit(ch) || ch == '_' || ch == '$';
        }

        bool isValidIdentifier(const std::string &str)
        {
            if (std::isdigit(str[0]))
                return false;
            for (const char &ch : str)
                if (!isValidIdentifierChar(ch))
                    return false;
            return true;
        }

        int8_t getBase(char ch)
        {
            switch (ch)
            {
            case 'b':
                return 2;
            case 'o':
                return 8;
            case 'x':
                return 16;
            default:
                return -1;
            }
        }

        bool isValidCharInBase(char ch, integer base)
        {
            if (!std::isdigit(ch) && !std::isalpha(ch))
                return false;
            if (base <= 10)
            {
                if (std::isalpha(ch))
                    return false;
                if (ch - '0' >= base)
                    return false;
            }
            else if (std::isalpha(ch) && std::tolower(ch) - 'a' >= base - 10)
            {
                return false;
            }
            return true;
        }

        Variant parseNumber(std::string str)
        {
            if (str.empty())
                return Variant();

            int8_t sign = 1;
            if (str[0] == '-')
            {
                sign = -1;
                str.erase(0, 1);
            }
            int8_t base = getBase(str[0]);
            if (base != -1)
                str.erase(0, 1);
            else
                base = 10;

            if (str.find(".") != std::string::npos)
                return Variant(sign * std::stof(str));
            else
                return Variant(sign * std::stoi(str, nullptr, base));
        }
    } // namespace compiler
} // namespace phi
