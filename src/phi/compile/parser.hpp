#pragma once
#include <compile/ast.hpp>
#include <compile/token_generator.hpp>

namespace phi {
struct Parser {
    using node_t = unique_ptr<ast::Expr>;
    using token_t = unique_ptr<token::Token>;
    Parser(TokenGenerator &generator);

    node_t parse();

  private:
    token_t _M_look;
    TokenGenerator &_M_generator;

    void move();
    void match(uint16_t);
    void match(uint16_t, token_t &);

    node_t body();
    node_t block();
    node_t sequence();
    node_t stmt();
    node_t expr();
    node_t assign();
    node_t boolean();
    node_t bor();
    node_t band();
    node_t join();
    node_t equality();
    node_t rel();
    node_t shift();
    node_t as();  // addition and subtraction
    node_t mdm(); // Multiplication, division and mod
    node_t power();
    node_t unary();
    node_t factor();

    node_t opt(node_t &);
    node_t args();
};
} // namespace phi
