#pragma once
#include <compile/token.hpp>
#include <typedef.hpp>
#include <vector>

namespace phi {
namespace ast {
struct Expr {
    enum Type {
        EXPR,
        STRING,
        INTEGER,
        REAL,
        SEQUENCE,
        BLOCK,
        UNARY_EXPR,
        BINARY_EXPR,
        IF,
        WHILE,
        FOR,
        ACCESS,
        CALL,
        FUNC
    };
    Expr();

    uint64_t line;

    virtual void print(int16_t level);
    virtual Type getType() { return Type::EXPR; }
};
struct IntegerExpr : Expr {
    int64_t value;
    IntegerExpr(int64_t v) : value(v), Expr() {}
    virtual Type getType() override { return Type::INTEGER; }
    virtual void print(int16_t level) override;
};
struct RealExpr : Expr {
    real_t value;
    RealExpr(real_t v) : value(v), Expr() {}
    virtual Type getType() override { return Type::REAL; }
    virtual void print(int16_t level) override;
};
struct StringExpr : Expr {
    shared_ptr<string> value;
    StringExpr(const shared_ptr<string> &v) : value(v), Expr() {}
    virtual Type getType() override { return Type::STRING; }
    virtual void print(int16_t level) override;
};
struct Sequence : Expr {
    unique_ptr<Expr> current;
    unique_ptr<Expr> next;

    Sequence(unique_ptr<Expr> &&c, unique_ptr<Expr> &&n)
        : current(move(c)), next(move(n)), Expr() {}
    Type getType() override { return Type::SEQUENCE; }
    virtual void print(int16_t level) override;
};

struct Block : Expr {
    using Expr::Expr;

    unique_ptr<Expr> body;

    Block(unique_ptr<Expr> &&b) : body(move(b)), Expr() {}
    Type getType() override { return Type::EXPR; }
    virtual void print(int16_t level) override;
};
struct UnaryExpr : Expr {

    unique_ptr<Expr> expr;
    unique_ptr<token::Token> op;
    UnaryExpr(unique_ptr<Expr> &&e, unique_ptr<token::Token> &&o)
        : Expr(), expr(move(e)), op(move(o)) {}
    Type getType() override { return Type::UNARY_EXPR; }
    virtual void print(int16_t level) override;
};
struct BinaryExpr : Expr {
    unique_ptr<Expr> left;
    unique_ptr<Expr> right;
    unique_ptr<token::Token> op;
    BinaryExpr(unique_ptr<Expr> &&l, unique_ptr<Expr> &&r,
               unique_ptr<token::Token> o)
        : Expr(), left(move(l)), right(move(r)), op(move(o)) {}

    Type getType() override { return Type::BINARY_EXPR; }
    virtual void print(int16_t level) override;
};
struct Load : Expr {
    shared_ptr<string> identifier;
    bool withVar;

    Load(const shared_ptr<string> &i, bool wv) : identifier(i), withVar(wv) {}
    Type getType() override { return Type::EXPR; }
    virtual void print(int16_t level) override;
};

struct If : Expr {
    unique_ptr<Expr> condition;
    unique_ptr<Expr> body;
    unique_ptr<Expr> elseBody;

    If(unique_ptr<Expr> &&c, unique_ptr<Expr> &&b, unique_ptr<Expr> &&eb)
        : condition(move(c)), body(move(b)), elseBody(move(eb)), Expr() {}

    virtual void print(int16_t level) override;
    virtual Type getType() override { return Type::IF; }
};
struct While : Expr {
    unique_ptr<Expr> condition;
    unique_ptr<Expr> body;
    unique_ptr<Expr> elseBody;

    While(unique_ptr<Expr> &&c, unique_ptr<Expr> &&b, unique_ptr<Expr> &&eb)
        : condition(move(c)), body(move(b)), elseBody(move(eb)), Expr() {}

    virtual void print(int16_t level) override;
    virtual Type getType() override { return Type::IF; }
};
struct For : Expr {
    unique_ptr<Expr> initializer;
    unique_ptr<Expr> condition;
    unique_ptr<Expr> update;
    unique_ptr<Expr> body;
    unique_ptr<Expr> elseBody;

    For(unique_ptr<Expr> &&i, unique_ptr<Expr> &&c, unique_ptr<Expr> &&u,
        unique_ptr<Expr> &&b, unique_ptr<Expr> &&eb)
        : initializer(move(i)), condition(move(c)), update(move(u)),
          body(move(b)), elseBody(move(eb)), Expr() {}

    virtual void print(int16_t level) override;
    virtual Type getType() override { return Type::FOR; }
};
struct Access : Expr {
    unique_ptr<Expr> target;
    unique_ptr<Expr> args;

    Access(unique_ptr<Expr> &&t, unique_ptr<Expr> &&a)
        : target(move(t)), args(move(a)), Expr() {}
    virtual void print(int16_t level) override;
    virtual Type getType() override { return Type::ACCESS; }
};
struct Call : Expr {
    unique_ptr<Expr> target;
    unique_ptr<Expr> args;

    Call(unique_ptr<Expr> &&t, unique_ptr<Expr> &&a)
        : target(move(t)), args(move(a)), Expr() {}
    virtual void print(int16_t level) override;
    virtual Type getType() override { return Type::CALL; }
};
struct Func : Expr {
    std::vector<shared_ptr<std::string>> args;
    unique_ptr<Expr> body;
    shared_ptr<string> name;

    Func(const std::vector<shared_ptr<std::string>> &a, unique_ptr<Expr> &&b,
         const shared_ptr<string> &n)
        : args(a), body(move(b)), name(n), Expr() {}

    virtual void print(int16_t level) override;
    virtual Type getType() override { return Type::FUNC; }
};

} // namespace ast

} // namespace phi
