#include "parser.hpp"
#include <phi/exception.hpp>

#define THROW throw SyntaxException("Unexpected token " + _M_look->toString() + ".")
#define CHECK_NONE \
    if (!x)        \
    THROW

namespace phi
{
	using namespace token;
	using namespace ast;

	void Parser::match(token::tag_t tag)
	{
		if (_M_look->tag() == tag)
			move();
		else
			throw SyntaxException(
				"Unexpected token " + _M_look->toString() +
				" at line " + std::to_string(_M_look->line()) +
				", expected '" + token::toString(tag) + '\'');
	}

	void Parser::move()
	{
		++_M_it;
		_M_look = _M_it != _M_tokens->end() ? *_M_it : nullptr;
		// throw SyntaxException("Unexpected end of program.");
	}

	Parser::node_t Parser::parse(token::tokens& tokens)
	{
		if (tokens.empty())
			return new Block;
		tokens.insert(tokens.begin(), new Token('{'));
		tokens.insert(tokens.end(), new Token('}'));
		return program(tokens);
	}

	Parser::node_t Parser::program(const tokens& tokens)
	{
		if (tokens.empty())
			return new Block;
		_M_tokens = &tokens;
		_M_it = tokens.begin();
		_M_look = *_M_it;

		return block();
	}

	Parser::node_t Parser::block()
	{
		token_t tok = _M_look;
		match('{');
		auto block = new Block{ sequence(), tok };
		match('}');
		return block;
	}
	Parser::node_t Parser::sequence()
	{
		if (_M_look->tag() == '}')
			return new Sequence;
		if (_M_look->tag() == ';')
			return (move(), sequence());
		node_t x = expr();
		CHECK_NONE;
		return (Node*)(new Sequence(x, sequence()));
	}
	Parser::node_t Parser::expr()
	{
		switch (_M_look->tag())
		{
		case '{':
			return block();
		case ';':
			return nullptr;
		}
		return comma();
	}
	Parser::node_t Parser::comma()
	{
		node_t x = (Node*)(new Comma{_M_look, assign(), nullptr});
		while (_M_look->tag() == ',')
		{
			token_t tok = _M_look;
			move();
			x = (Node*)(new Comma{tok, x, assign()});
		}
		return x;
	}
	Parser::node_t Parser::assign()
	{
		node_t x = boolean();
		while (_M_look->tag() == '=')
		{
			token_t tok = _M_look;
			move();
			x = (Node*)(new Binary(tok, x, boolean()));
		}
		return x;
	}
	Parser::node_t Parser::boolean()
	{
		node_t x = bor();
		while (_M_look->tag() == Tag::OR)
		{
			token_t tok = _M_look;
			move();
			x = (Node*)(new Binary(tok, x, bor()));
		}
		return x;
	}
	Parser::node_t Parser::bor()
	{
		node_t x = band();
		while (_M_look->tag() == '|')
		{
			token_t tok = _M_look;
			move();
			x = (Node*)(new Binary(tok, x, band()));
		}
		return x;
	}
	Parser::node_t Parser::band()
	{
		node_t x = join();
		while (_M_look->tag() == '&')
		{
			token_t tok = _M_look;
			move();
			x = (Node*)(new Binary(tok, x, join()));
		}
		return x;
	}
	Parser::node_t Parser::join()
	{
		node_t x = equality();
		while (_M_look->tag() == Tag::AND)
		{
			token_t tok = _M_look;
			move();
			x = (Node*)(new Binary(tok, x, equality()));
		}
		return x;
	}
	Parser::node_t Parser::equality()
	{
		node_t x = rel();
		while (_M_look->tag() == Tag::EQ || _M_look->tag() == Tag::NE)
		{
			token_t tok = _M_look;
			move();
			x = (Node*)(new Binary(tok, x, rel()));
		}
		return x;
	}
	Parser::node_t Parser::rel()
	{
		node_t x = shift();
		switch (_M_look->tag())
		{
		case '<':
		case '>':
		case Tag::LE:
		case Tag::GE:
			token_t tok = _M_look;
			move();
			return (Node*)(new Binary(tok, x, shift()));
		}
		return x;
	}
	Parser::node_t Parser::shift()
	{
		node_t x = as();
		while (_M_look->tag() == Tag::LSHIFT || _M_look->tag() == Tag::RSHIFT)
		{
			token_t tok = _M_look;
			move();
			x = (Node*)(new Binary(tok, x, as()));
		}
		return x;
	}
	Parser::node_t Parser::as()
	{
		node_t x = mdm();
		while (_M_look->tag() == '+' || _M_look->tag() == '-')
		{
			token_t tok = _M_look;
			move();
			x = (Node*)(new Binary(tok, x, mdm()));
		}
		return x;
	}
	Parser::node_t Parser::mdm()
	{
		node_t x = unary();
		while (_M_look->tag() == '*' || _M_look->tag() == '/' || _M_look->tag() == '%')
		{
			token_t tok = _M_look;
			move();
			x = (Node*)(new Binary(tok, x, unary()));
		}
		return x;
	}
	Parser::node_t Parser::unary()
	{
		switch (_M_look->tag())
		{
		case '-':
		case '!':
		case '~':
		case Tag::INC:
		case Tag::RED:
			token_t tok = _M_look;
			move();
			return (Node*)(new Unary(tok, unary()));
		}
		return factor();
	}
	Parser::node_t Parser::factor()
	{
		node_t x;
		token_t tok;
		switch (_M_look->tag())
		{
		case '(':
			move();
			x = assign();
			match(')');
			return x;
		case Tag::INT:
		case Tag::REAL:
		case Tag::TRUE:
		case Tag::FALSE:
		case Tag::STRING:
			x = new Constant(_M_look);
			move();
			return x;
		case Tag::ID:
			tok = _M_look;
			move();
			switch (_M_look->tag())
			{
			case '(':
			case '[':
			case '.':
				return opt(new Load(tok, false));
			default:
				return new Load(tok, false);
			}
		case Tag::VAR:
			move();
			tok = _M_look;
			move();
			return new Load(tok, true);
		case Tag::EVAL:
			tok = _M_look;
			move();
			return new Eval{ tok, expr() };
		case Tag::DELETE:
			tok = _M_look;
			move();
			return new Delete{tok, expr()};
		case Tag::IF:
		{
			tok = _M_look;
			move();
			match('(');
			x = expr();
			match(')');
			node_t body = expr();
			if (_M_look->tag() != Tag::ELSE)
				return new If(tok, x, body);
			move();
			node_t else_body = expr();
			return new IfElse(tok, x, body, else_body);
		}
		case Tag::WHILE:
		{
			tok = _M_look;
			move();
			match('(');
			x = expr();
			match(')');
			node_t body = expr();
			if (_M_look->tag() != Tag::ELSE)
				return new While(tok, x, body);
			move();
			node_t else_body = expr();
			return new WhileElse(tok, x, body, else_body);
		}
		case Tag::FOR:
		{
			tok = _M_look;
			move();
			match('(');
			node_t init = expr();
			match(';');
			node_t cond = expr();
			match(';');
			node_t update = expr();
			match(')');
			node_t body = expr();
			if (_M_look->tag() != Tag::ELSE)
				return new For(tok, init, cond, update, body);
			move();
			node_t else_body = expr();
			return new ForElse(tok, init, cond, update, body, else_body);
		}
		}
		THROW;
	}

#define IS_OPT(tok)  ((tok)->tag() == '.' || (tok)->tag() == '(' || (tok)->tag() == '[')

	Parser::node_t Parser::opt(node_t obj)
	{
		node_t x;
		switch (_M_look->tag())
		{
		case '.':
		{
			token_t tok = _M_look;
			move();
			token_t symbol = _M_look;
			match(Tag::ID);
			x = new Access{ tok, obj, new Comma{symbol, new Constant(symbol), nullptr} };
			break;
		}
		case '[':
		{
			token_t tok = _M_look;
			move();
			Ref<Comma> args = _M_look->tag() != ']' ? comma() : nullptr;
			match(']');
			x = new Access{ tok, obj, args };
			break;
		}
		case '(':
		{
			token_t tok = _M_look;
			move();
			Ref<Comma> args = _M_look->tag() != ')' ? comma() : nullptr;
			match(')');
			x = new Call{ tok, obj, args };
			break;
		}
		}
		return IS_OPT(_M_look) ? opt(x) : x;
	}
} // namespace phi
