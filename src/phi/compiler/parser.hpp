#pragma once
#include <phi/compiler/ast.hpp>
#include <phi/compiler/token.hpp>
#include <set>

namespace phi
{
    class Parser
    {
    private:
        const token::tokens *_M_tokens;
        token::tokens::const_iterator _M_it;
        Ref<token::Token> _M_look;

    public:
        using node_t = Ref<ast::Node>;
        using token_t = Ref<token::Token>;

        Parser() : _M_tokens(nullptr) {}

        node_t parse(token::tokens& tokens);

    private:
        node_t program(const token::tokens& tokens);

        void match(token::tag_t tag);
        void match(std::set<token::tag_t> tags);
        void move();

        node_t block();
        node_t sequence();
        node_t expr();

        node_t comma(bool = true);
        node_t assign();
        node_t boolean();
        node_t bor();
        node_t band();
        node_t join();
        node_t equality();
        node_t rel();
        node_t shift();
        node_t as(); // addition and subtraction
        node_t mdm(); // Multiplication and division and mod
        node_t unary();
        node_t factor();

        node_t opt(node_t obj);
    };
} // namespace phi
