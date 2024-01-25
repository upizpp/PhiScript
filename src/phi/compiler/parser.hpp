#pragma once
#include <phi/compiler/ast.hpp>
#include <phi/compiler/token.hpp>

namespace phi
{
    class Parser
    {
    private:
        const token::tokens *_M_tokens;
        token::tokens::const_iterator _M_it;
        Ref<token::Token> _M_look;

    public:
        using node = Ref<ast::Node>;

        Parser() : _M_tokens(nullptr) {}

        node parse(token::tokens& tokens);
        node program(const token::tokens &tokens);

    private:
        void match(token::tag_t tag);
        void move();

        node block();
        node sequence();
        node expr();

        node assign();
        node boolean();
        node join();
        node equality();
        node rel();
        node as(); // addition and subtraction
        node md(); // Multiplication and division
        node unary();
        node factor();
        node access();
    };
} // namespace phi
