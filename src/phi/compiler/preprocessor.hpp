#pragma once
#include <phi/compiler/lexer.hpp>
#include <phi/compiler/token_generator.hpp>
#include <phi/typedef.hpp>
#include <stack>

namespace phi {
    // TODO : PreprocessRule with arguments
    struct PreprocessRule {
        PreprocessRule() {}
        PreprocessRule(const string &path);

        unordered_map<Ref<token::Token>, token::tokens> replacements;

        bool replaceable(Ref<token::Token> token) {
            return replacements.find(token) != replacements.end();
        }
        token::tokens &getReplacement(Ref<token::Token> token) {
            return replacements[token];
        }
    };

    class Preprocessor : public TokenGenerator {
      private:
        PreprocessRule _M_rule;
        Borrower<TokenGenerator> _M_generator;
        std::stack<Ref<token::Token>> _M_cache;
        Ref<token::Token> _M_ptr;

      private:
        void read();

      public:
        Preprocessor(TokenGenerator *generator,
                     PreprocessRule rule = PreprocessRule())
            : _M_generator(generator), _M_rule(rule) {}

        Ref<token::Token> getNextToken();
        bool eof() { return !_M_cache.empty() && _M_generator->eof(); }
    };
} // namespace phi