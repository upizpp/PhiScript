#include "preprocessor.hpp"
#include <algorithm>
#include <phi/compiler/lexer.hpp>
#include <phi/compiler/scanner.hpp>

namespace phi {
    PreprocessRule::PreprocessRule(const string &path) {
        FileScanner scanner(path);
        static std::ostringstream os;
        os.str();
        string section;
        while (!scanner.eof()) {
            string line = scanner.getLine();
            if (line.empty())
                continue;
            int level = line.find_first_not_of("\t ");
            if (level == 0) {
                if (!section.empty()) {
                    Lexer lexer{new StringScanner(os.str())};
                    os.str("");
                    replacements[new token::Word{section, token::Tag::ID}] =
                        lexer.getTokens();
                    section = "";
                }
                int end = line.find_last_of(':');
                if (end == string::npos)
                    throw SyntaxException(
                        "Each replacement header must end with ':'.");
                if (line.find_first_of("\t ") < end)
                    throw SyntaxException(
                        "The replacement header must be a single token.");
                section = line.substr(0, end);
            } else {
                os << line << '\n';
            }
        }

        if (!section.empty()) {
            Lexer lexer{new StringScanner(os.str())};
            replacements[new token::Word{section, token::Tag::ID}] =
                lexer.getTokens();
        }
    }
    Ref<token::Token> Preprocessor::getNextToken() {
        if (!_M_cache.empty()) {
            Ref<token::Token> result = _M_cache.top();
            _M_cache.pop();
            return result;
        }
        read();
        while (_M_ptr->tag() == '\n')
            read();
        return _M_ptr;
    }
    void Preprocessor::read() { _M_ptr = _M_generator->getNextToken(); }
} // namespace phi
