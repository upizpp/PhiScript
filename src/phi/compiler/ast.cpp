#include "ast.hpp"
#include <phi/compiler/compiler.hpp>
#include <phi/compiler/generator.hpp>
#include <phi/exception.hpp>
#include <phi/function.hpp>

#define INIT string indent = string(level * 4, ' ');
#define OS cout << indent

namespace phi
{
	namespace ast
	{
		Borrower<Block> Block::top;

		arg_t Node::emit_label()
		{
			static arg_t label = 0;
			get_state().emitLabel(label);
			return label++;
		}

		State &Node::get_state()
		{
			return Generator::instance()->getState();
		}

		OPCodePacker Node::push(const OPCode &code)
		{
			State &state = get_state();
			if (code.opt() == OPCode::Command::CLEAR && state.top().opt() == OPCode::Command::CLEAR)
				return OPCodePacker();
			return get_state().push(code, line());
		}

		void Node::print(uinteger level)
		{
			INIT;
			OS << "Node";
		}

		void Func::print(uinteger level)
		{
			INIT;
			OS << (_M_name ? *_M_name : "Func") << '(';
			if (_M_binds)
			{
				bool first = true;
				for (auto &&item : *_M_binds)
				{
					if (first)
						first = false;
					else
						cout << ", ";
					cout << *item;
				}
			}
			cout << ")\n";
			_M_body->print(level + 1);
		}

		void Func::gen()
		{
			if (_M_name)
				push({OPCode::Command::ALLOCATE,
					  Generator::instance()->push(new Variant{*_M_name})});
			Function func = Compiler::load(_M_body, Compiler::globalOption ? *Compiler::globalOption : CompileOption{});
			func.getMethod().bind(std::move(_M_binds));
			push({OPCode::Command::LOAD_CONST, Generator::instance()->push(new Variant{func})});
			if (_M_name)
				push({OPCode::Command::ASSIGN});
		}

		void Sequence::print(uinteger level)
		{
			INIT;
			if (!_M_current)
				return;
			_M_current->print(level);
			OS << "\n";
			if (_M_next)
				_M_next->print(level);
		}

		void Comma::print(uinteger level)
		{
			static bool title = true;
			INIT;
			if (!_M_current)
				return;
			bool flag = false;
			if (title)
			{
				OS << "Comma\n";
				title = false;
				flag = true;
			}
			_M_current->print(level + flag);
			if (_M_next)
			{
				OS << "\n";
				_M_next->print(level + flag);
			}
			if (flag)
				title = true;
		}

		void Args::print(uinteger level)
		{
			static bool title = true;
			INIT;
			if (!_M_current)
				return;
			bool flag = false;
			if (title)
			{
				OS << "Args\n";
				title = false;
				flag = true;
			}
			_M_current->print(level + flag);
			if (_M_next)
			{
				OS << "\n";
				_M_next->print(level + flag);
			}
			if (flag)
				title = true;
		}

		void Call::print(uinteger level)
		{
			INIT;
			OS << "Call\n";
			_M_method->print(level + 1);
			if (_M_args)
			{
				cout << '\n';
				OS << '\n';
				_M_args->print(level + 1);
			}
		}

		void Access::print(uinteger level)
		{
			INIT;
			OS << "Access\n";
			_M_obj->print(level + 1);
			if (_M_args)
			{
				cout << '\n';
				OS << "    Args:\n";
				_M_args->print(level + 2);
			}
		}

		void Block::print(uinteger level)
		{
			INIT;
			if (_M_seq && !_M_seq->isNull())
			{
				OS << "{\n";
				_M_seq->print(level + 1);
				OS << "}";
			}
			else
				OS << "{}";
		}

		void Unary::print(uinteger level)
		{
			INIT;
			OS << "Unary(" << *opt() << ")\n";
			_M_operand->print(level + 1);
		}

		void Binary::print(uinteger level)
		{
			INIT;
			OS << "Binary(" << *opt() << ")\n";
			_M_left->print(level + 1);
			OS << '\n';
			_M_right->print(level + 1);
		}

		void Load::print(uinteger level)
		{
			INIT;
			OS << (_M_var ? "Allocate" : "Load") << "(" << identifier() << ")";
		}

		void Constant::print(uinteger level)
		{
			INIT;
			OS << "Constant(" << *opt() << ")";
		}

		Eval::Eval(Ref<token::Token> tok, Ref<Block> expr) : Stmt(tok),
															 _M_expr(expr), _M_block(Block::top)
		{
			if (!_M_block)
				throw CompileException("Eval cannot be used without a block.");
			Block::top->eval(this);
		}

		void Eval::print(uinteger level)
		{
			INIT;
			OS << "Eval\n";
			_M_expr->print(level + 1);
		}

		void If::print(uinteger level)
		{
			INIT;
			OS << "if " << (_M_likely ? "likely" : "unlikely") << '\n';
			_M_test->print(level);
			cout << '\n';
			_M_body->print(level + 1);
		}

		void IfElse::print(uinteger level)
		{
			INIT;
			If::print(level);
			cout << '\n';
			OS << "else\n";
			_M_else->print(level + 1);
		}

		void While::print(uinteger level)
		{
			INIT;
			OS << "while: " << name() << '\n';
			_M_test->print(level);
			cout << '\n';
			_M_body->print(level + 1);
		}

		void WhileElse::print(uinteger level)
		{
			INIT;
			While::print(level);
			cout << '\n';
			OS << "else\n";
			_M_else->print(level + 1);
		}

		void For::print(uinteger level)
		{
			INIT;
			OS << "for: " << name() << '\n';
			if (_M_init)
			{
				_M_init->print(level);
				cout << '\n';
			}
			if (_M_test)
			{
				_M_test->print(level);
				cout << '\n';
			}
			if (_M_update)
			{
				_M_update->print(level);
				cout << '\n';
			}
			if (_M_body)
				_M_body->print(level + 1);
		}

		void ForElse::print(uinteger level)
		{
			INIT;
			For::print(level);
			cout << '\n';
			OS << "else\n";
			_M_else->print(level + 1);
		}

		void Return::print(uinteger level)
		{
			INIT;
			OS << "return\n";
			_M_operand->print(level + 1);
		}

		void Delete::print(uinteger level)
		{
			INIT;
			OS << "delete\n";
			_M_operand->print(level + 1);
		}

		void Import::print(uinteger level)
		{
			INIT;
			OS << "import " << module() << " as " << import();
		}

		void Break::print(uinteger level)
		{
			INIT;
			OS << "break " << _M_loop->name();
		}

		void Continue::print(uinteger level)
		{
			INIT;
			OS << "continue " << _M_loop->name();
		}

		void Array::print(uinteger level)
		{
			INIT;
			OS << "array\n";
			_M_members->print(level + 1);
		}

		void Dictionary::print(uinteger level)
		{
			INIT;
			OS << "dictionary\n";
			_M_members->print(level + 1);
		}

		void Stmt::genStmt(Ref<Stmt> node)
		{
			if (!node)
				return;
			node->gen();
			push({OPCode::Command::CLEAR});
		}

		void Array::gen()
		{
			push({OPCode::Command::ARGS});
			if (_M_members)
				_M_members->gen();
			push({OPCode::Command::MAKE_ARRAY});
		}

		void Dictionary::gen()
		{
			push({OPCode::Command::ARGS});
			if (_M_members)
				_M_members->gen();
			push({OPCode::Command::MAKE_DICT});
		}

		void Call::gen()
		{
			push({OPCode::Command::ARGS});
			if (_M_args)
				_M_args->gen();
			_M_method->gen();
			push({OPCode::Command::CALL});
		}

		void Access::gen()
		{
			push({OPCode::Command::ARGS});
			if (_M_args)
				_M_args->gen();
			_M_obj->gen();
			push({OPCode::Command::ACCESS});
		}

		void Block::gen()
		{
			if (!_M_seq)
				return;
			push(OPCode{OPCode::Command::PUSH_ENV});
			_M_seq->gen();
			push(OPCode{OPCode::Command::POP_ENV});
			if (!_M_eval_map)
				return;
			list<OPCodePacker> goto_codes;
			goto_codes.push_back(push({OPCode::Command::GOTO}));
			for (auto &&pair : *_M_eval_map)
			{
				pair.second = emit_label();
				push({OPCode::Command::POP_ENV});
				pair.first->genExpr();
				goto_codes.push_back(push({OPCode::Command::GOTO}));
			}
			arg_t exit = emit_label();
			for (OPCodePacker &goto_code : goto_codes)
				goto_code->value(exit);
		}

		void Sequence::gen()
		{
			if (!_M_current)
				return;
			_M_current->gen();
			if (_M_next)
			{
				push({OPCode::Command::CLEAR});
				_M_next->gen();
			}
		}

		void Load::gen()
		{
			push({_M_var ? OPCode::Command::ALLOCATE : OPCode::Command::LOAD,
				  Generator::instance()->push(new Variant{((Ref<token::Word>)opt())->value()})});
		}

		void Unary::gen()
		{
			_M_operand->gen();
			push({OPCode::unaryToCommand(opt()->tag())});
		}

		void Binary::gen()
		{
			_M_left->gen();
			_M_right->gen();
			push({OPCode::binaryToCommand(opt()->tag())});
		}

		void Constant::gen()
		{
			Ref<Variant> constant;
			Ref<token::Token> data = opt();
			switch (data->tag())
			{
			case token::Tag::TRUE:
			case token::Tag::FALSE:
				constant = new Variant{((Ref<token::Word>)data)->value() == "true" ? true : false};
				break;
			case token::Tag::INT:
				constant = new Variant{((Ref<token::Integer>)data)->value()};
				break;
			case token::Tag::REAL:
				constant = new Variant{((Ref<token::Real>)data)->value()};
				break;
			case token::Tag::ID:
			case token::Tag::STRING:
				constant = new Variant{((Ref<token::Word>)data)->value()};
				break;
			}
			push({OPCode::Command::LOAD_CONST, Generator::instance()->push(constant)});
		}

		void Eval::gen()
		{
			OPCodePacker *goto_code = new OPCodePacker{push({OPCode::Command::GOTO})};
			Generator::instance()->addTask([this, goto_code]()
										   {
			(*goto_code)->value(this->_M_block->getLabel(this));
			delete goto_code; });
		}

		void If::gen()
		{
			if (_M_likely)
			{
				/*
					eval x
					ifFalse x goto L0
					body
					L0:
				*/

				_M_test->gen();
				OPCodePacker if_code = push({OPCode::Command::IFFALSE});

				_M_body->gen();
				arg_t L0 = emit_label();
				if_code->value(L0);
				exit(L0);
			}
			else
			{
				/*
					eval x
					ifTrue x goto L0
					goto L1
					L0: body
					L1:
				*/

				_M_test->gen();
				OPCodePacker if_code = push({OPCode::Command::IFTRUE});

				OPCodePacker goto_code = push({OPCode::Command::GOTO});
				if_code->value(emit_label()); // L0
				_M_body->gen();
				arg_t L1 = emit_label();
				goto_code->value(L1);
				exit(L1);
			}
		}

		void IfElse::gen()
		{
			if (_M_likely)
			{
				/*
					eval x
					ifFalse x goto L0
					body
					goto L1
					L0: else_body
					L1:
				*/

				_M_test->gen();
				OPCodePacker if_code = push({OPCode::Command::IFFALSE});

				_M_body->gen();
				OPCodePacker goto_code = push({OPCode::Command::GOTO});
				if_code->value(emit_label()); // L0
				_M_else->gen();
				arg_t L1 = emit_label();
				goto_code->value(L1);
				exit(L1);
			}
			else
			{
				/*
					eval x
					ifTrue x goto L0
					else_body
					goto L1
					L0: body
					L1:
				*/

				_M_test->gen();
				OPCodePacker if_code = push({OPCode::Command::IFTRUE});

				_M_else->gen();
				OPCodePacker goto_code = push({OPCode::Command::GOTO});
				if_code->value(emit_label()); // L0
				_M_body->gen();
				arg_t L1 = emit_label();
				goto_code->value(L1);
				exit(L1);
			}
		}

		void While::gen()
		{
			/*
				L0: eval x
				ifFalse x goto L1
				body
				goto L0
				L1:
			*/
			arg_t L0 = emit_label();
			_M_test->gen();
			OPCodePacker if_code = push({OPCode::IFFALSE});

			_M_body->gen();
			entry(L0);
			push({OPCode::GOTO, L0});
			arg_t L1 = emit_label();
			exit(L1);
			if_code->value(L1);
		}

		void WhileElse::gen()
		{
			/*
				eval x
				ifTrue x goto L1
				else_body
				goto L2
				L0: eval x
				ifFalse x goto L2
				L1: body
				goto L0
				L2:
			*/
			_M_test->gen();
			OPCodePacker if_L1 = push({OPCode::Command::IFTRUE});
			Stmt::genStmt(_M_else);

			OPCodePacker goto_L2 = push({OPCode::Command::GOTO});
			arg_t L0 = emit_label();

			_M_test->gen();
			OPCodePacker if_L2 = push({OPCode::Command::IFFALSE});

			if_L1->value(emit_label()); // L1
			_M_body->gen();
			entry(L0);
			push({OPCode::Command::GOTO, L0});
			arg_t L2 = emit_label();
			goto_L2->value(L2);
			if_L2->value(L2);
			exit(L2);
		}

		void For::gen()
		{
			/*
				push_env
				init
				L0: eval test
				ifFalse test goto L1
				body
				update
				goto L0
				L1: pop_env
			*/
			// TODO endless loop optimization
			push({OPCode::Command::PUSH_ENV});
			Stmt::genStmt(_M_init);

			arg_t L0 = emit_label();
			entry(L0);
			OPCodePacker if_code;
			if (_M_test)
			{
				_M_test->gen();
				if_code = push({OPCode::Command::IFFALSE});
			}
			_M_body->gen();

			Stmt::genStmt(_M_update);

			push({OPCode::Command::GOTO, L0});
			arg_t L1 = emit_label();
			exit(L1);
			if (_M_test)
				if_code->value(L1); // L1
			push({OPCode::Command::POP_ENV});
		}

		void ForElse::gen()
		{
			/*
				push_env
				init
				eval test
				ifTrue test goto L0
				else_body
				goto L2
				L1: eval test
				ifFalse test goto L2
				L0: body
				update
				goto L1
				L2: pop_env
			*/
			push({OPCode::Command::PUSH_ENV});
			Stmt::genStmt(_M_init);

			_M_test->gen();
			OPCodePacker if_L0 = push({OPCode::Command::IFTRUE});
			Stmt::genStmt(_M_else);
			OPCodePacker goto_L2 = push({OPCode::Command::GOTO});
			arg_t L1 = emit_label();
			entry(L1);
			_M_test->gen();
			OPCodePacker if_L2 = push({OPCode::Command::IFFALSE});
			if_L0->value(emit_label()); // L0
			_M_body->gen();
			Stmt::genStmt(_M_update);

			push({OPCode::Command::GOTO, L1});
			arg_t L2 = emit_label();
			goto_L2->value(L2);
			if_L2->value(L2);
			exit(L2);
			push({OPCode::Command::POP_ENV});
		}

		void Break::gen()
		{
			OPCodePacker *goto_code = new OPCodePacker{push({OPCode::Command::GOTO})};
			Generator::instance()->addTask([goto_code, this]()
										   {
			(*goto_code)->value(this->_M_loop->exit());
			delete goto_code; });
		}

		void Continue::gen()
		{
			OPCodePacker *goto_code = new OPCodePacker{push({OPCode::Command::GOTO})};
			Generator::instance()->addTask([goto_code, this]()
										   {
			(*goto_code)->value(this->_M_loop->entry());
			delete goto_code; });
		}

		void Delete::gen()
		{
			_M_operand->gen();
			push({OPCode::Command::DEL});
		}

		void Return::gen()
		{
			_M_operand->gen();
			push({OPCode::Command::RETURN});
		}

		void Comma::gen()
		{
			if (!_M_current)
				return;
			_M_current->gen();
			if (_M_next)
			{
				push({OPCode::Command::CLEAR});
				_M_next->gen();
			}
		}

		void Args::gen()
		{
			if (!_M_current)
				return;
			_M_current->gen();
			if (_M_next)
				_M_next->gen();
		}

		void Import::gen()
		{

			push({OPCode::Command::ALLOCATE,
				  Generator::instance()->push(new Variant{((Ref<token::Word>)opt())->value()})});
			push({OPCode::Command::IMPORT, Generator::instance()->push(new Variant{_M_module_name->value()})});
			push({OPCode::Command::ASSIGN});
		}

		Loop::loops_t Loop::_M_loops;
		void Loop::pushLoop(Borrower<Loop> loop)
		{
			_M_loops.push_back(loop);
		}

		void Loop::pop()
		{
			_M_loops.pop_back();
		}

		Borrower<Loop> Loop::find(const string &name)
		{
			if (_M_loops.empty())
				return nullptr;
			if (name.empty())
				return _M_loops.back();
			loops_t::iterator it = _M_loops.end();
			while (--it != _M_loops.end())
				if ((*it)->name() == name)
					return *it;
			return nullptr;
		}

		Controller::Controller(Ref<token::Token> tok, const string &name) : Stmt(tok)
		{
			Borrower<Loop> loop = Loop::find(name);
			if (!loop)
				throw CompileException("Cannot find the loop(%s).", name.c_str());
			_M_loop = loop;
		}
	} // namespace ast
} // namespace phi