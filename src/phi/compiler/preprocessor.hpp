#pragma once
#include <phi/compiler/token.hpp>
#include <phi/typedef.hpp>

namespace phi
{
    struct PreprocessRule
    {
        unordered_map<token::Token, Ref<token::Token>> replacements;

        bool replaceable(Ref<token::Token> token) { return replacements.find(*token) != replacements.end(); }
    };

    class Preprocessor
    {
    private:
        token::tokens _M_tokens;
        Owner<PreprocessRule> _M_rule;

    public:
        Preprocessor(const token::tokens &t) : _M_tokens(t), _M_rule(nullptr) {}
        Preprocessor(const token::tokens &t, PreprocessRule *rule) : _M_tokens(t), _M_rule(rule) {}

        void setRule(PreprocessRule *rule) { _M_rule.reset(rule); }

        list<Ref<token::Token>> &getTokens();
    };
}