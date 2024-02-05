#include "preprocessor.hpp"
#include <algorithm>
#include <phi/compiler/lexer.hpp>
#include <phi/compiler/scanner.hpp>

namespace phi
{
    token::tokens &Preprocessor::getTokens()
    {
        if (_M_rule)
        {
            // replace
            while (true)
            {
                bool replaced = false;
                for (auto it = _M_tokens.begin(); it != _M_tokens.end(); ++it)
                {
                    if (_M_rule->replaceable(*it))
                    {
                        replaced = true;
                        auto &replacement = _M_rule->getReplacement(*it);
                        auto temp = it;
                        it = _M_tokens.insert(it, replacement.begin(), replacement.end());
                        _M_tokens.erase(temp);
                    }
                }
                if (!replaced)
                    break;
            }
        }
        // merge strings
        auto it = std::adjacent_find(_M_tokens.begin(), _M_tokens.end(), [](Ref<token::Token> a, Ref<token::Token> b)
                                     { return a->tag() == token::Tag::STRING && b->tag() == token::Tag::STRING; });
        while (it != _M_tokens.end())
        {
            auto a = it;
            auto b = it;
            ++b;
            (Ref<token::Word>(*a))->merge(*Ref<token::Word>(*b));
            _M_tokens.erase(b);
            it = std::adjacent_find(_M_tokens.begin(), _M_tokens.end(), [](Ref<token::Token> a, Ref<token::Token> b)
                                    { return a->tag() == token::Tag::STRING && b->tag() == token::Tag::STRING; });
        }
        return _M_tokens;
    }

    PreprocessRule::PreprocessRule(const string &path)
    {
        FileScanner scanner(path);
        static std::ostringstream os;
        os.str();
        string section;
        while (!scanner.eof())
        {
            string line = scanner.getLine();
            if (line.empty())
                continue;
            int level = line.find_first_not_of("\t ");
            if (level == 0)
            {
                if (!section.empty())
                {
                    Lexer lexer{new StringScanner(os.str())};
                    os.str("");
                    replacements[new token::Word{section, token::Tag::ID}] = lexer.getTokens();
                    section = "";
                }
                int end = line.find_last_of(':');
                if (end == string::npos)
                    throw SyntaxException("Each replacement header must end with ':'.");
                if (line.find_first_of("\t ") < end)
                    throw SyntaxException("The replacement header must be a single token.");
                section = line.substr(0, end);
            }
            else
            {
                os << line << '\n';
            }
        }

        if (!section.empty())
        {
            Lexer lexer{new StringScanner(os.str())};
            replacements[new token::Word{section, token::Tag::ID}] = lexer.getTokens();
        }
    }
} // namespace phi
