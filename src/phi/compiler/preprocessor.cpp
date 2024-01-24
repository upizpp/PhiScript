#include "preprocessor.hpp"
#include <algorithm>

namespace phi
{
    list<Ref<token::Token>> &Preprocessor::getTokens()
    {
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
} // namespace phi
