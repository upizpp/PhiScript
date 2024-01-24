#pragma once
#include <phi/compiler/token.hpp>
#include <phi/typedef.hpp>

namespace phi
{
    // TODO : PreprocessRule with arguments
    struct PreprocessRule
    {
        PreprocessRule() {}
        PreprocessRule(const string &path);

        unordered_map<Ref<token::Token>, token::tokens> replacements;

        bool replaceable(Ref<token::Token> token) { return replacements.find(token) != replacements.end(); }
        token::tokens &getReplacement(Ref<token::Token> token) { return replacements[token]; }
    };

    class Preprocessor
    {
    private:
        token::tokens _M_tokens;
        Owner<PreprocessRule> _M_rule;

    public:
        Preprocessor(const token::tokens &t) : _M_tokens(t), _M_rule(nullptr) {}
        Preprocessor(const token::tokens &t, const PreprocessRule &rule) : _M_tokens(t), _M_rule(new PreprocessRule{rule}) {}

        void setRule(const PreprocessRule &rule) { _M_rule.reset(new PreprocessRule{rule}); }

        list<Ref<token::Token>> &getTokens();
    };
}