#pragma once
#include "token.hpp"

namespace phi
{
    namespace compiler
    {
        class Preprocessor
        {
        public:
            static void handleString(Token::Container* tokens);
            static void replaceToken(Token::Container* tokens, Token from, Token to);
        };
    } // namespace compiler
} // namespace phi
