#pragma once
#include <phi/compiler/token.hpp>
#include <phi/runtime/state.hpp>
#include <phi/typedef.hpp>

namespace phi
{
	namespace ast
	{
		class Node
		{
		private:
			uinteger _M_line;

		protected:
			arg_t emit_label();
			State &get_state();
			OPCodePacker push(const OPCode &);

		public:
			Node() : _M_line(0) {}
			explicit Node(uinteger line) : _M_line(line) {}

			uinteger line() const { return _M_line; }
			void line(uinteger line) { _M_line = line; }

			virtual void print(uinteger level = 0);
			virtual void gen() = 0;
		};

		class Expr : public Node
		{
		private:
			Ref<token::Token> _M_operator;

		public:
			using Node::Node;
			explicit Expr(Ref<token::Token> opt) : Node(opt->line()), _M_operator(opt) {}

			Ref<token::Token> opt() { return _M_operator; }
			const Ref<token::Token> opt() const { return _M_operator; }
		};

		class Constant : public Expr
		{
		public:
			using Expr::Expr;
			virtual void print(uinteger level = 0) override;
			virtual void gen() override;
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

			Sequence *link(Ref<Sequence> next)
			{
				_M_next = next;
				return (Sequence *)next.data();
			}

			bool isNull() const { return !_M_current; }

			virtual void print(uinteger level = 0) override;
			virtual void gen() override;
		};

		class Eval;

		class Block : public Expr
		{
		private:
			Ref<Sequence> _M_seq;
			Owner<map<Eval *, arg_t>> _M_eval_map;

		public:
			static Borrower<Block> top;

			using Expr::Expr;
			Block(Ref<Sequence> seq, Ref<token::Token> opt) : Expr(opt), _M_seq(seq) {}

			virtual void print(uinteger level = 0) override;
			virtual void gen() override;

			Owner<map<Eval *, arg_t>> &evalMap() { return _M_eval_map; }
			void evalMap(Owner<map<Eval *, arg_t>> m) { _M_eval_map = std::move(m); }
			void eval(Eval *eval)
			{
				if (!_M_eval_map)
					_M_eval_map.reset(new map<Eval *, arg_t>);
				_M_eval_map->insert({eval, ULONG_LONG_MAX});
			}
			arg_t getLabel(Eval *eval) const
			{
				return _M_eval_map->at(eval);
			}
		};

		class Comma : public Expr
		{
		private:
			Ref<Expr> _M_current;
			Ref<Comma> _M_next;

		public:
			Comma(Ref<token::Token> opt, Ref<Expr> current, Ref<Comma> next) : Expr(opt), _M_current(current), _M_next(next) {}

			virtual void print(uinteger level = 0) override;
			virtual void gen() override;
		};
		

		class Args : public Expr
		{
		private:
			Ref<Expr> _M_current;
			Ref<Args> _M_next;

		public:
			Args(Ref<token::Token> opt, Ref<Expr> current, Ref<Args> next) : Expr(opt), _M_current(current), _M_next(next) {}

			virtual void print(uinteger level = 0) override;
			virtual void gen() override;
		};

		class Call : public Expr
		{
		private:
			Ref<Expr> _M_method;
			Ref<Args> _M_args;

		public:
			Call(Ref<token::Token> opt, Ref<Expr> method, Ref<Args> args) : Expr(opt), _M_method(method), _M_args(args) {}

			virtual void print(uinteger level = 0) override;
			virtual void gen() override;
		};

		class Access : public Expr
		{
		private:
			Ref<Expr> _M_obj;
			Ref<Args> _M_args;

		public:
			Access(Ref<token::Token> opt, Ref<Expr> obj, Ref<Args> args) : Expr(opt), _M_obj(obj), _M_args(args) {}

			virtual void print(uinteger level = 0) override;
			virtual void gen() override;
		};

		class Unary : public Expr
		{
		private:
			Ref<Expr> _M_operand;

		public:
			Unary(Ref<token::Token> token, Ref<Expr> operand) : Expr(token), _M_operand(operand) {}

			Ref<Expr> operand() { return _M_operand; }

			virtual void print(uinteger level = 0) override;
			virtual void gen() override;
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
			virtual void gen() override;
		};

		class Load : public Expr
		{
		private:
			bool _M_var;

		public:
			Load(Ref<token::Token> token, bool var) : Expr(token), _M_var(var) {}

			const string &identifier() const { return (Ref<token::Word>(opt()))->value(); }

			virtual void print(uinteger level = 0) override;
			virtual void gen() override;
		};

		class Array: public Expr
		{
		private:
			Ref<Args> _M_members;
		public:
			Array(Ref<token::Token> tok, Ref<Args> members) : Expr(tok), _M_members(members) {}

			virtual void print(uinteger level = 0) override;
			virtual void gen() override;
		};

		class Dictionary: public Expr
		{
		private:
			Ref<Args> _M_members;
		public:
			Dictionary(Ref<token::Token> tok, Ref<Args> members) : Expr(tok), _M_members(members) {}

			virtual void print(uinteger level = 0) override;
			virtual void gen() override;
		};

		class Stmt : public Expr
		{
		private:
			arg_t _M_exit;

		public:
			using Expr::Expr;
			Stmt() = default;

			arg_t exit() const { return _M_exit; }
			void exit(arg_t v) { _M_exit = v; }

			void genStmt(Ref<Stmt>);
		};

		class Delete : public Stmt
		{
		private:
			Ref<Expr> _M_operand;

		public:
			Delete(Ref<token::Token> token, Ref<Expr> operand) : Stmt(token), _M_operand(operand) {}

			Ref<Expr> operand() { return _M_operand; }

			virtual void print(uinteger level = 0);
			virtual void gen() override;
		};

		class Eval : public Stmt
		{
		private:
			Ref<Expr> _M_expr;
			Ref<Block> _M_block;

		public:
			Eval(Ref<token::Token> tok, Ref<Block> block);

			virtual void print(uinteger level = 0) override;
			virtual void gen() override;

			void genExpr() { _M_expr->gen(); }
		};

		class Import : public Stmt
		{
		private:
			Ref<token::Word> _M_module_name;
			Ref<token::Word> _M_import_name;

		public:
			Import(Ref<token::Token> tok, Ref<token::Word> module_name) : Stmt(tok), _M_module_name(module_name), _M_import_name(module_name) {}
			Import(Ref<token::Token> tok, Ref<token::Word> module_name, Ref<token::Word> import_name) : Stmt(tok), _M_module_name(module_name), _M_import_name(import_name) {}

			const string &module() const { return _M_module_name->value(); }
			const string &import() const { return _M_import_name->value(); }

			virtual void print(uinteger level = 0) override;
			virtual void gen() override;
		};

		class If : public Stmt
		{
		protected:
			Ref<Expr> _M_test;
			Ref<Expr> _M_body;
			bool _M_likely;

		public:
			using Stmt::Stmt;
			If(Ref<token::Token> tok, bool likely, Ref<Expr> condition, Ref<Expr> body) : Stmt(tok),
																						  _M_likely(likely), _M_test(condition), _M_body(body) {}

			virtual void print(uinteger level = 0) override;
			virtual void gen() override;
		};

		class IfElse : public If
		{
		private:
			Ref<Expr> _M_else;

		public:
			IfElse(Ref<token::Token> tok, bool likely, Ref<Expr> condition, Ref<Expr> body, Ref<Expr> else_) : If(tok, likely, condition, body),
																											   _M_else(else_) {}

			virtual void print(uinteger level = 0) override;
			virtual void gen() override;
		};

		class Loop : public Stmt
		{
		public:
			using loops_t = std::list<Borrower<Loop>>;

		private:
			static loops_t _M_loops;

			string _M_name;
			arg_t _M_entry;

		public:
			using Stmt::Stmt;

			static void pushLoop(Borrower<Loop>);
			static void pop();
			static Borrower<Loop> find(const string &);

			const string &name() const { return _M_name; }
			Loop &name(const string &name)
			{
				_M_name = name;
				return *this;
			}

			arg_t entry() const { return _M_entry; }
			void entry(arg_t v) { _M_entry = v; }
		};

		class Controller : public Stmt
		{
		protected:
			Ref<Loop> _M_loop;

		public:
			Controller(Ref<token::Token> tok, const string &name);
		};

		class Break : public Controller
		{
		public:
			using Controller::Controller;

			virtual void print(uinteger level = 0) override;
			virtual void gen() override;
		};

		class Continue : public Controller
		{
		public:
			using Controller::Controller;

			virtual void print(uinteger level = 0) override;
			virtual void gen() override;
		};

		class While : public Loop
		{
		protected:
			Ref<Expr> _M_test;
			Ref<Expr> _M_body;

		public:
			using Loop::Loop;
			While(Ref<token::Token> tok, Ref<Expr> condition, Ref<Expr> body) : Loop(tok),
																				_M_test(condition), _M_body(body) {}

			virtual void print(uinteger level = 0) override;
			virtual void gen() override;
		};

		class WhileElse : public While
		{
		private:
			Ref<Expr> _M_else;

		public:
			WhileElse(Ref<token::Token> tok, Ref<Expr> condition, Ref<Expr> body, Ref<Expr> else_) : While(tok, condition, body),
																									 _M_else(else_) {}

			virtual void print(uinteger level = 0) override;
			virtual void gen() override;
		};

		class For : public Loop
		{
		protected:
			Ref<Expr> _M_init;
			Ref<Expr> _M_test;
			Ref<Expr> _M_update;
			Ref<Expr> _M_body;

		public:
			using Loop::Loop;
			For(Ref<token::Token> tok, Ref<Expr> init, Ref<Expr> test, Ref<Expr> update, Ref<Expr> body) : Loop(tok),
																										   _M_init(init), _M_test(test),
																										   _M_update(update), _M_body(body) {}

			virtual void print(uinteger level = 0) override;
			virtual void gen() override;
		};

		class ForElse : public For
		{
		private:
			Ref<Expr> _M_else;

		public:
			ForElse(Ref<token::Token> tok, Ref<Expr> init, Ref<Expr> condition, Ref<Expr> update, Ref<Expr> body, Ref<Expr> else_) : For(tok, init, condition, update, body),
																																	 _M_else(else_) {}

			virtual void print(uinteger level = 0) override;
			virtual void gen() override;
		};
	} // namespace ast
} // namespace phi
