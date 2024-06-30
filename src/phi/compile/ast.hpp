#pragma once
#include <compile/token.hpp>
#include <typedef.hpp>

namespace phi {
namespace ast {
struct Node {
    enum Type { NODE, EXPR };
    virtual Type getType() { return Type::NODE; }

    Node();

    uint64_t line;
};
struct Expr : Node {
    Expr() : Node() {}
};
struct UnaryExpr : Expr {
    UnaryExpr(unique_ptr<Expr> e, unique_ptr<token::Token> o)
        : Expr(), expr(move(e)), op(move(o)) {}

    unique_ptr<Expr> expr;
    unique_ptr<token::Token> op;
};
struct BinaryExpr : Expr {
    BinaryExpr(unique_ptr<Expr> l, unique_ptr<Expr> r,
               unique_ptr<token::Token> o)
        : Expr(), left(move(l)), right(move(r)), op(move(o)) {}

    unique_ptr<Expr> left;
    unique_ptr<Expr> right;
    unique_ptr<token::Token> op;
};

} // namespace ast

} // namespace phi
