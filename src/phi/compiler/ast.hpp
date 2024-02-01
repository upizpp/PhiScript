#pragma once
#include <phi/compiler/token.hpp>
#include <phi/typedef.hpp>

namespace phi
{
    namespace ast
    {
        class Node
        {
        private:
            uinteger _M_line;

        public:
            Node() : _M_line(0) {}
            explicit Node(uinteger line) : _M_line(line) {}

            uinteger line() const { return _M_line; }
            void line(uinteger line) { _M_line = line; }

            virtual void print(uinteger level = 0);
        };

        class Expr : public Node
        {
        private:
            Ref<token::Token> _M_operator;

        public:
            using Node::Node;
            explicit Expr(Ref<token::Token> op) : Node(op->line()), _M_operator(op) {}

            Ref<token::Token> op() { return _M_operator; }
            const Ref<token::Token> op() const { return _M_operator; }

            virtual void print(uinteger level = 0) override;
        };

        class Constant : public Expr
        {
        public:
            using Expr::Expr;

            virtual void print(uinteger level = 0) override;
        };

        class Sequence : public Node
        {
        private:
            Ref<Expr> _M_current;
            Ref<Expr> _M_next;

        public:
            Sequence() {}
            Sequence(Ref<Expr> current, Ref<Expr> next) : _M_current(current), _M_next(next) {}

            Ref<Expr> current() { return _M_current; }
            Ref<Expr> next() { return _M_next; }

            bool isNull() const { return !_M_current; }

            virtual void print(uinteger level = 0) override;
        };

        class Block : public Expr
        {
        private:
            Ref<Sequence> _M_seq;

        public:
            using Expr::Expr;
            explicit Block(Ref<Sequence> seq, Ref<token::Token> op) : Expr(op), _M_seq(seq) {}

            virtual void print(uinteger level = 0) override;
        };

        class Unary : public Expr
        {
        private:
            Ref<Expr> _M_operand;

        public:
            using Expr::Expr;
            Unary(Ref<token::Token> token, Ref<Expr> operand) : Expr(token), _M_operand(operand) {}

            Ref<Expr> operand() { return _M_operand; }

            virtual void print(uinteger level = 0);
        };

        class Binary : public Expr
        {
        private:
            Ref<Expr> _M_left;
            Ref<Expr> _M_right;

        public:
            using Expr::Expr;
            Binary(Ref<token::Token> token, Ref<Expr> left, Ref<Expr> right) : Expr(token), _M_left(left), _M_right(right) {}

            Ref<Expr> left() { return _M_left; }
            Ref<Expr> right() { return _M_right; }

            virtual void print(uinteger level = 0) override;
        };

        class Load : public Expr
        {
        private:
            bool _M_var;

        public:
            using Expr::Expr;
            Load(Ref<token::Token> token, bool var) : Expr(token), _M_var(var) {}

            const string &identifier() const { return (Ref<token::Word>(op()))->value(); }

            virtual void print(uinteger level = 0) override;
        };

        class Stmt : public Expr
        {
        public:
            using Expr::Expr;
        };

        class Eval : public Stmt
        {
        private:
            Ref<Expr> _M_expr;

        public:
            using Stmt::Stmt;
            explicit Eval(Ref<Expr> expr) : Stmt(Ref<token::Token>(token::Word::get("eval"))),
                                            _M_expr(expr) {}

            virtual void print(uinteger level = 0) override;
        };

        class If : public Stmt
        {
        protected:
            Ref<Expr> _M_condition;
            Ref<Expr> _M_body;

        public:
            using Stmt::Stmt;
            If(Ref<Expr> condition, Ref<Expr> body) : Stmt(Ref<token::Token>(token::Word::get("if"))),
                                                      _M_condition(condition), _M_body(body) {}

            virtual void print(uinteger level = 0) override;
        };

        class IfElse : public If
        {
        private:
            Ref<Expr> _M_else;

        public:
            using If::If;
            IfElse(Ref<Expr> condition, Ref<Expr> body, Ref<Expr> else_) : If(condition, body),
                                                                           _M_else(else_) {}

            virtual void print(uinteger level = 0) override;
        };

        class While : public Stmt
        {
        protected:
            Ref<Expr> _M_condition;
            Ref<Expr> _M_body;

        public:
            using Stmt::Stmt;
            While(Ref<Expr> condition, Ref<Expr> body) : Stmt(Ref<token::Token>(token::Word::get("while"))),
                                                         _M_condition(condition), _M_body(body) {}

            virtual void print(uinteger level = 0) override;
        };

        class WhileElse : public While
        {
        private:
            Ref<Expr> _M_else;

        public:
            using While::While;
            WhileElse(Ref<Expr> condition, Ref<Expr> body, Ref<Expr> else_) : While(condition, body),
                                                                              _M_else(else_) {}

            virtual void print(uinteger level = 0) override;
        };

        class For : public Stmt
        {
        protected:
            Ref<Expr> _M_init;
            Ref<Expr> _M_condition;
            Ref<Expr> _M_update;
            Ref<Expr> _M_body;

        public:
            using Stmt::Stmt;
            For(Ref<Expr> init, Ref<Expr> condition, Ref<Expr> update, Ref<Expr> body) : Stmt(Ref<token::Token>(token::Word::get("for"))),
                                                                                         _M_init(init), _M_condition(condition),
                                                                                         _M_update(update), _M_body(body) {}

            virtual void print(uinteger level = 0) override;
        };

        class ForElse : public For
        {
        private:
            Ref<Expr> _M_else;

        public:
            using For::For;
            ForElse(Ref<Expr> init, Ref<Expr> condition, Ref<Expr> update, Ref<Expr> body, Ref<Expr> else_) : For(init, condition, update, body),
                                                                                                              _M_else(else_) {}

            virtual void print(uinteger level = 0) override;
        };
    } // namespace ast
} // namespace phi
