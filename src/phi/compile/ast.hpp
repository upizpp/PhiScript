#pragma once
#include <compile/token.hpp>
#include <runtime/program.hpp>
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
        BOOL,
        SEQUENCE,
        BLOCK,
        UNARY_EXPR,
        BINARY_EXPR,
        IF,
        WHILE,
        FOR,
        ACCESS,
        CALL,
        FUNC,
        LOAD,
        RETURN,
        BREAK,
        CONTINUE,
    };
    Expr();
    Expr(uint64_t l) : line(l) {}

    uint64_t line;

    virtual void print(int16_t level);
    virtual Type getType() { return Type::EXPR; }
};
struct IntegerExpr : Expr {
    int64_t value;
    IntegerExpr(int64_t v, uint64_t line_) : value(v), Expr(line_) {}
    virtual Type getType() override { return Type::INTEGER; }
    virtual void print(int16_t level) override;
};
struct RealExpr : Expr {
    real_t value;
    RealExpr(real_t v, uint64_t line_) : value(v), Expr(line_) {}
    virtual Type getType() override { return Type::REAL; }
    virtual void print(int16_t level) override;
};
struct BoolExpr : Expr {
    bool value;
    BoolExpr(bool v, uint64_t line_) : value(v), Expr(line_) {}
    virtual Type getType() override { return Type::BOOL; }
    virtual void print(int16_t level) override;
};
struct StringExpr : Expr {
    shared_ptr<string> value;
    StringExpr(const shared_ptr<string> &v, uint64_t line_)
        : value(v), Expr(line_) {}
    virtual Type getType() override { return Type::STRING; }
    virtual void print(int16_t level) override;
};
struct Sequence : Expr {
    unique_ptr<Expr> current;
    unique_ptr<Expr> next;

    Sequence(unique_ptr<Expr> &&c, unique_ptr<Expr> &&n, uint64_t line_)
        : current(move(c)), next(move(n)), Expr(line_) {}
    Type getType() override { return Type::SEQUENCE; }
    virtual void print(int16_t level) override;
};

struct Block : Expr {
    using Expr::Expr;

    unique_ptr<Expr> body;

    Block(unique_ptr<Expr> &&b, uint64_t line_) : body(move(b)), Expr(line_) {}
    Type getType() override { return Type::BLOCK; }
    virtual void print(int16_t level) override;
};
struct UnaryExpr : Expr {

    unique_ptr<Expr> expr;
    unique_ptr<token::Token> op;
    UnaryExpr(unique_ptr<Expr> &&e, unique_ptr<token::Token> &&o,
              uint64_t line_)
        : Expr(line_), expr(move(e)), op(move(o)) {}
    Type getType() override { return Type::UNARY_EXPR; }
    virtual void print(int16_t level) override;
};
struct BinaryExpr : Expr {
    unique_ptr<Expr> left;
    unique_ptr<Expr> right;
    unique_ptr<token::Token> op;
    BinaryExpr(unique_ptr<Expr> &&l, unique_ptr<Expr> &&r,
               unique_ptr<token::Token> o, uint64_t line_)
        : Expr(line_), left(move(l)), right(move(r)), op(move(o)) {}

    Type getType() override { return Type::BINARY_EXPR; }
    virtual void print(int16_t level) override;
};
struct Load : Expr {
    shared_ptr<string> identifier;
    bool withVar;

    Load(const shared_ptr<string> &i, bool wv, uint64_t line_)
        : identifier(i), withVar(wv), Expr(line_) {}
    Type getType() override { return Type::LOAD; }
    virtual void print(int16_t level) override;
};

struct If : Expr {
    unique_ptr<Expr> condition;
    unique_ptr<Expr> body;
    unique_ptr<Expr> elseBody;

    If(unique_ptr<Expr> &&c, unique_ptr<Expr> &&b, unique_ptr<Expr> &&eb,
       uint64_t line_)
        : condition(move(c)), body(move(b)), elseBody(move(eb)), Expr(line_) {}

    virtual void print(int16_t level) override;
    virtual Type getType() override { return Type::IF; }
};
struct Loop : Expr {
    static void push(native_ptr<Loop> loop);
    static void pop();
    static native_ptr<Loop> top();
    static native_ptr<Loop> find(shared_ptr<string> tag);

    shared_ptr<string> tag;

    std::vector<Program::OPCodeReference> break_labels;
    std::vector<Program::OPCodeReference> continue_labels;

    Loop(shared_ptr<string> tag, uint64_t line_) : tag(tag), Expr(line_) {}

  private:
    static std::vector<native_ptr<Loop>> _M_loops;
};
struct While : Loop {
    unique_ptr<Expr> condition;
    unique_ptr<Expr> body;
    unique_ptr<Expr> elseBody;

    While(shared_ptr<string> tag_, unique_ptr<Expr> &&c, unique_ptr<Expr> &&b,
          unique_ptr<Expr> &&eb, uint64_t line_)
        : Loop(tag_, line_), condition(move(c)), body(move(b)),
          elseBody(move(eb)) {}

    virtual void print(int16_t level) override;
    virtual Type getType() override { return Type::WHILE; }
};
struct For : Loop {
    unique_ptr<Expr> initializer;
    unique_ptr<Expr> condition;
    unique_ptr<Expr> update;
    unique_ptr<Expr> body;
    unique_ptr<Expr> elseBody;

    For(shared_ptr<string> tag_, unique_ptr<Expr> &&i, unique_ptr<Expr> &&c,
        unique_ptr<Expr> &&u, unique_ptr<Expr> &&b, unique_ptr<Expr> &&eb,
        uint64_t line_)
        : initializer(move(i)), condition(move(c)), update(move(u)),
          body(move(b)), elseBody(move(eb)), Loop(tag_, line_) {}

    virtual void print(int16_t level) override;
    virtual Type getType() override { return Type::FOR; }
};
struct LoopController : Expr {
    native_ptr<Loop> loop;

    LoopController(shared_ptr<string> tag, uint64_t line_)
        : Expr(line_), loop(tag ? Loop::find(tag) : Loop::top()) {}
};
struct Break : LoopController {
    using LoopController::LoopController;

    virtual Type getType() override { return Type::BREAK; }
    virtual void print(int16_t level) override;
};
struct Continue : LoopController {
    using LoopController::LoopController;
    virtual Type getType() override { return Type::CONTINUE; }
    virtual void print(int16_t level) override;
};
struct Access : Expr {
    unique_ptr<Expr> target;
    unique_ptr<Expr> args;

    Access(unique_ptr<Expr> &&t, unique_ptr<Expr> &&a, uint64_t line_)
        : target(move(t)), args(move(a)), Expr(line_) {}
    virtual void print(int16_t level) override;
    virtual Type getType() override { return Type::ACCESS; }
};
struct Call : Expr {
    unique_ptr<Expr> target;
    unique_ptr<Expr> args;

    Call(unique_ptr<Expr> &&t, unique_ptr<Expr> &&a, uint64_t line_)
        : target(move(t)), args(move(a)), Expr(line_) {}
    virtual void print(int16_t level) override;
    virtual Type getType() override { return Type::CALL; }
};
struct Func : Expr {
    std::vector<shared_ptr<std::string>> args;
    unique_ptr<Expr> body;
    shared_ptr<string> name;

    Func(const std::vector<shared_ptr<std::string>> &a, unique_ptr<Expr> &&b,
         const shared_ptr<string> &n, uint64_t line_)
        : args(a), body(move(b)), name(n), Expr(line_) {}

    virtual void print(int16_t level) override;
    virtual Type getType() override { return Type::FUNC; }
};
struct Return : Expr {
    unique_ptr<Expr> value;

    Return(unique_ptr<Expr> &&v, uint64_t line_)
        : value(move(v)), Expr(line_) {}
    virtual void print(int16_t level) override;
    virtual Type getType() override { return Type::RETURN; }
};

} // namespace ast

} // namespace phi
