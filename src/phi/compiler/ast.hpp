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
			Ref<Sequence> _M_next;

		public:
			Sequence() {}
			Sequence(Ref<Expr> current, Ref<Sequence> next) : _M_current(current), _M_next(next) {}

			Ref<Expr> current() { return _M_current; }
			Ref<Sequence> next() { return _M_next; }

			Sequence* link(Ref<Sequence> next)
			{
				_M_next = next;
				return (Sequence*)next.data();
			}

			bool isNull() const { return !_M_current; }

			virtual void print(uinteger level = 0) override;
		};

		class Block : public Expr
		{
		private:
			Ref<Sequence> _M_seq;

		public:
			using Expr::Expr;
			Block(Ref<Sequence> seq, Ref<token::Token> op) : Expr(op), _M_seq(seq) {}

			virtual void print(uinteger level = 0) override;
		};

		class Comma : public Expr
		{
		private:
			Ref<Expr> _M_current;
			Ref<Expr> _M_next;
		public:
			Comma(Ref<token::Token> op, Ref<Expr> current, Ref<Expr> next): Expr(op), _M_current(current), _M_next(next) {}

			virtual void print(uinteger level = 0) override;
		};

		class Call : public Expr
		{
		private:
			Ref<Expr> _M_method;
			Ref<Comma> _M_args;
		public:
			Call(Ref<token::Token> op, Ref<Expr> method, Ref<Comma> args): Expr(op), _M_method(method), _M_args(args) {}
			
			virtual void print(uinteger level = 0) override;
		};

		class Access : public Expr
		{
		private:
			Ref<Expr> _M_obj;
			Ref<Comma> _M_args;
		public:
			Access(Ref<token::Token> op, Ref<Expr> obj, Ref<Comma> args) : Expr(op), _M_obj(obj), _M_args(args) {}

			virtual void print(uinteger level = 0) override;
		};

		class Unary : public Expr
		{
		private:
			Ref<Expr> _M_operand;

		public:
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
			Load(Ref<token::Token> token, bool var) : Expr(token), _M_var(var) {}

			const string& identifier() const { return (Ref<token::Word>(op()))->value(); }

			virtual void print(uinteger level = 0) override;
		};

		class Stmt : public Expr
		{
		public:
			using Expr::Expr;
		};

		class Delete : public Stmt
		{
		private:
			Ref<Expr> _M_operand;

		public:
			Delete(Ref<token::Token> token, Ref<Expr> operand) : Stmt(token), _M_operand(operand) {}

			Ref<Expr> operand() { return _M_operand; }

			virtual void print(uinteger level = 0);
		};

		class Eval : public Stmt
		{
		private:
			Ref<Expr> _M_expr;

		public:
			Eval(Ref<token::Token> tok, Ref<Expr> expr) : Stmt(tok), 
				_M_expr(expr) {}

			virtual void print(uinteger level = 0) override;
		};

		class Import : public Stmt
		{
		private:
			Ref<token::Word> _M_module_name;
			Ref<token::Word> _M_import_name;
		public:
			Import(Ref<token::Token> tok, Ref<token::Word> module_name) : Stmt(tok), _M_module_name(module_name), _M_import_name(module_name) {}
			Import(Ref<token::Token> tok, Ref<token::Word> module_name, Ref<token::Word> import_name): Stmt(tok), _M_module_name(module_name), _M_import_name(import_name) {}
			
			const string& module() const { return _M_module_name->value(); }
			const string& import() const { return _M_import_name->value(); }

			virtual void print(uinteger level = 0) override;
		};

		class If : public Stmt
		{
		protected:
			Ref<Expr> _M_condition;
			Ref<Expr> _M_body;

		public:
			If(Ref<token::Token> tok, Ref<Expr> condition, Ref<Expr> body) : Stmt(tok),
				_M_condition(condition), _M_body(body) {}

			virtual void print(uinteger level = 0) override;
		};

		class IfElse : public If
		{
		private:
			Ref<Expr> _M_else;

		public:
			IfElse(Ref<token::Token> tok, Ref<Expr> condition, Ref<Expr> body, Ref<Expr> else_) : If(tok, condition, body),
				_M_else(else_) {}

			virtual void print(uinteger level = 0) override;
		};

		class While : public Stmt
		{
		protected:
			Ref<Expr> _M_condition;
			Ref<Expr> _M_body;

		public:
			While(Ref<token::Token> tok, Ref<Expr> condition, Ref<Expr> body) : Stmt(tok),
				_M_condition(condition), _M_body(body) {}

			virtual void print(uinteger level = 0) override;
		};

		class WhileElse : public While
		{
		private:
			Ref<Expr> _M_else;

		public:
			WhileElse(Ref<token::Token> tok, Ref<Expr> condition, Ref<Expr> body, Ref<Expr> else_) : While(tok, condition, body),
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
			For(Ref<token::Token> tok, Ref<Expr> init, Ref<Expr> condition, Ref<Expr> update, Ref<Expr> body) : Stmt(tok),
				_M_init(init), _M_condition(condition),
				_M_update(update), _M_body(body) {}

			virtual void print(uinteger level = 0) override;
		};

		class ForElse : public For
		{
		private:
			Ref<Expr> _M_else;

		public:
			ForElse(Ref<token::Token> tok, Ref<Expr> init, Ref<Expr> condition, Ref<Expr> update, Ref<Expr> body, Ref<Expr> else_) : For(tok, init, condition, update, body),
				_M_else(else_) {}

			virtual void print(uinteger level = 0) override;
		};
	} // namespace ast
} // namespace phi
