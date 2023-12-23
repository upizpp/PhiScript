#pragma once
#include "token.hpp"

namespace phi
{
    namespace runtime
    {
        class Expression;
    }
    namespace compiler
    {

        class StaticAnalyzer
        {
        public:
            static void analyze(Token::Container *);
            static void analyze(runtime::Expression*);
        };
    } // namespace compiler
} // namespace phi
