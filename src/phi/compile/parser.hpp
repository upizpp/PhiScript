#pragma once
#include <compile/ast.hpp>
#include <compile/token_generator.hpp>

namespace phi {
struct Parser {
    using node_t = unique_ptr<ast::Node>;
    Parser(TokenGenerator &generator) : _M_generator(generator) {}

    node_t parse();

  private:
    unique_ptr<token::Token> _M_look;

    void move();
    bool match(token::Tag);

    TokenGenerator &_M_generator;
};
} // namespace phi
