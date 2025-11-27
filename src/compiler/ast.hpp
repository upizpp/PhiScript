#pragma once

#include "token.hpp"
#include <global.hpp>

namespace phi {

namespace ast {

struct Node {
    void print() { print(0); }
    virtual void print(int64_t level) = 0;

  protected:
    void print(int64_t level, std::string msg);
};
struct Expr : Node {
    using Node::print;
};
struct Block : Expr {
    shared_ptr<Node> body;

    Block(shared_ptr<Node> body) : body(std::move(body)) {}

  protected:
    void print(int64_t level) override;
};

struct Sequence : Expr {
    shared_ptr<Node> expr;
    shared_ptr<Sequence> next;

    Sequence() = default;
    Sequence(shared_ptr<Node> expr, shared_ptr<Sequence> next)
        : expr(std::move(expr)), next(std::move(next)) {}

  protected:
    void print(int64_t level) override;
};

struct Constant : Expr {
    unique_ptr<Token> value;

    Constant(unique_ptr<Token> value) : value(std::move(value)) {}

  protected:
    void print(int64_t level) override;
};

struct Unary : Expr {
    unique_ptr<Token> op;
    shared_ptr<Node> expr;

    Unary(unique_ptr<Token> op, shared_ptr<Node> expr)
        : expr(std::move(expr)), op(std::move(op)) {}

  protected:
    void print(int64_t level) override;
};

struct Binary : Expr {
    shared_ptr<Node> lhs;
    shared_ptr<Node> rhs;
    unique_ptr<Token> op;

    Binary(unique_ptr<Token> op, shared_ptr<Node> lhs, shared_ptr<Node> rhs)
        : lhs(std::move(lhs)), rhs(std::move(rhs)), op(std::move(op)) {}

  protected:
    void print(int64_t level) override;
};

struct Load : Expr {
    shared_ptr<Token> what;
    bool allocate;

    Load(shared_ptr<Token> what, bool allocate = false)
        : what(std::move(what)), allocate(allocate) {}

  protected:
    void print(int64_t level) override;
};

} // namespace ast

} // namespace phi
