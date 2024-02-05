#include "parser.hpp"
#include <new>
#include <phi/exception.hpp>

#define THROW                                     \
	if (_M_look == _M_tokens->back())             \
		throw SyntaxException("Unexpected EOF."); \
	else                                          \
		throw SyntaxException("Unexpected token '" + _M_look->stringify() + "'.");
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
		else if (_M_look == _M_tokens->back())
			throw SyntaxException(
				"Unexpected EOF at line " + std::to_string(_M_look->line()) +
				", expected '" + token::toString(tag) + '\'' + ".");
		else
			throw SyntaxException(
				"Unexpected token '" + _M_look->stringify() +
				"' at line " + std::to_string(_M_look->line()) +
				", expected '" + token::toString(tag) + '\'' + ".");
	}

	void Parser::match(std::set<token::tag_t> tags)
	{
		if (tags.find(_M_look->tag()) != tags.end())
			move();
		else if (_M_look == _M_tokens->back())
			throw SyntaxException(
				"Unexpected EOF at line " + std::to_string(_M_look->line()) + ".");
		else
			throw SyntaxException(
				"Unexpected token '" + _M_look->stringify() +
				"' at line " + std::to_string(_M_look->line()) + ".");
	}

	void Parser::move()
	{
		++_M_it;
		_M_look = _M_it != _M_tokens->end() ? *_M_it : nullptr;
		// throw SyntaxException("Unexpected end of program.");
	}

	Parser::node_t Parser::parse(token::tokens &tokens)
	{
		if (tokens.empty())
			return new Block;
		tokens.insert(tokens.begin(), (new Token('{'))->line(tokens.front()->line()));
		tokens.insert(tokens.end(), (new Token('}'))->line(tokens.back()->line()));
		return program(tokens);
	}

	Parser::node_t Parser::program(const tokens &tokens)
	{
		if (tokens.empty())
			return new Block;
		_M_tokens = &tokens;
		_M_it = tokens.begin();
		_M_look = *_M_it;

		return block();
	}

	Parser::node_t Parser::body()
	{
		if (_M_look->tag() != '{')
		{
			auto res = new Block;
			auto tmp = Block::top;
			Block::top = res;
			auto expr_ = expr();
			auto eval_map = std::move(res->evalMap());
			res = new (res) Block{new Sequence{expr_, nullptr}, _M_look};
			res->evalMap(std::move(eval_map));
			Block::top = tmp;
			return res;
		}
		return block();
	}

	Parser::node_t Parser::block()
	{
		auto res = new Block;
		auto tmp = Block::top;
		Block::top = res;

		token_t tok = _M_look;
		match('{');
		node_t seq = sequence();
		auto eval_map = std::move(res->evalMap());
		res = new (res) Block{seq, tok};
		res->evalMap(std::move(eval_map));
		match('}');

		Block::top = tmp;

		return res;
	}
	Parser::node_t Parser::sequence()
	{
		if (_M_look->tag() == '}')
			return new Sequence;
		if (_M_look->tag() == ';')
			return (move(), sequence());
		node_t x = expr();
		CHECK_NONE;
		return (Node *)(new Sequence(x, sequence()));
	}
	Parser::node_t Parser::expr()
	{
		switch (_M_look->tag())
		{
		case '{':
			return block();
		case ';':
		case ')':
			return nullptr;
		}
		return assign();
	}
	Parser::node_t Parser::exprNoComma()
	{
		switch (_M_look->tag())
		{
		case '{':
			return block();
		case ';':
		case ')':
			return nullptr;
		}
		return assignNoComma();
	}
	Parser::node_t Parser::assign()
	{
		node_t x = comma(false);
		while (_M_look->tag() == '=')
		{
			token_t tok = _M_look;
			move();
			x = (Node *)(new Binary(tok, x, comma(false)));
		}
		return x;
	}
	Parser::node_t Parser::assignNoComma()
	{
		node_t x = boolean();
		while (_M_look->tag() == '=')
		{
			token_t tok = _M_look;
			move();
			x = (Node *)(new Binary(tok, x, boolean()));
		}
		return x;
	}
	Parser::node_t Parser::comma(bool required)
	{
		node_t node = boolean();
		if (_M_look->tag() != ',' && !required)
			return node;
		node_t x = (Node *)(new Comma{_M_look, node, nullptr});
		while (_M_look->tag() == ',')
		{
			token_t tok = _M_look;
			move();
			x = (Node *)(new Comma{tok, x, new Comma{_M_look, boolean(), nullptr}});
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
			x = (Node *)(new Binary(tok, x, bor()));
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
			x = (Node *)(new Binary(tok, x, band()));
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
			x = (Node *)(new Binary(tok, x, join()));
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
			x = (Node *)(new Binary(tok, x, equality()));
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
			x = (Node *)(new Binary(tok, x, rel()));
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
			return (Node *)(new Binary(tok, x, shift()));
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
			x = (Node *)(new Binary(tok, x, as()));
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
			x = (Node *)(new Binary(tok, x, mdm()));
		}
		return x;
	}
	Parser::node_t Parser::mdm()
	{
		node_t x = power();
		while (_M_look->tag() == '*' || _M_look->tag() == '/' || _M_look->tag() == '%')
		{
			token_t tok = _M_look;
			move();
			x = (Node *)(new Binary(tok, x, power()));
		}
		return x;
	}
	Parser::node_t Parser::power()
	{
		node_t x = unary();
		while (_M_look->tag() == Tag::POW)
		{
			token_t tok = _M_look;
			move();
			x = (Node *)(new Binary(tok, x, unary()));
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
			return (Node *)(new Unary(tok, unary()));
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
			x = expr();
			match(')');
			return x;
		case '[':
		{
			tok = _M_look;
			move();
			Ref<Args> members = _M_look->tag() != ']' ? args() : nullptr;
			match(']');
			return new Array{tok, members};
		}
		case '{':
		{
			tok = _M_look;
			move();
			Ref<Args> members = _M_look->tag() != '}' ? pairs() : nullptr;
			match('}');
			return new Dictionary{tok, members};
			
		}
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
			return new Eval{tok, expr()};
		case Tag::IMPORT:
		{
			tok = _M_look;
			move();
			token_t module_name = _M_look;
			token_t import_name = module_name;
			match({Tag::ID, Tag::STRING});
			if (_M_look->tag() == Tag::AS)
			{
				move();
				import_name = _M_look;
				match({Tag::ID, Tag::STRING});
			}
			return new Import{tok, module_name, import_name};
		}
		case Tag::DELETE:
			tok = _M_look;
			move();
			return new Delete{tok, expr()};
		case Tag::IF:
		{
			If *node = new If;

			tok = _M_look;
			move();
			bool likely = _M_look->tag() == Tag::UNLIKELY ? false : true;
			if (_M_look->tag() == Tag::LIKELY || _M_look->tag() == Tag::UNLIKELY)
				move();
			match('(');
			x = expr();
			match(')');
			node_t body = this->body();
			if (_M_look->tag() != Tag::ELSE)
				return new (node) If(tok, likely, x, body);
			move();
			node_t else_body = this->body();
			return new (node) IfElse(tok, likely, x, body, else_body);
		}
		case Tag::WHILE:
		{
			While *node = new While;
			Loop::pushLoop(node);

			tok = _M_look;
			move();
			string name;
			if (_M_look->tag() == ':')
			{
				move();
				token_t tmp = _M_look;
				match({Tag::ID, Tag::STRING});
				name = ((Ref<Word>)tmp)->value();
			}
			node->name(name);
			match('(');
			x = expr();
			match(')');
			node_t body = this->body();
			Loop::pop();
			if (_M_look->tag() != Tag::ELSE)
				return &(new (node) While{tok, x, body})->name(name);
			move();
			node_t else_body = this->body();
			return &(new (node) WhileElse{tok, x, body, else_body})->name(name);
		}
		case Tag::FOR:
		{
			For *node = new For;
			Loop::pushLoop(node);

			tok = _M_look;
			move();
			string name;
			if (_M_look->tag() == ':')
			{
				move();
				token_t tmp = _M_look;
				match({Tag::ID, Tag::STRING});
				name = ((Ref<Word>)tmp)->value();
			}
			node->name(name);
			match('(');
			node_t init = expr();
			match(';');
			node_t cond = expr();
			match(';');
			node_t update = expr();
			match(')');
			node_t body = this->body();
			Loop::pop();
			if (_M_look->tag() != Tag::ELSE)
				return &(new (node) For{tok, init, cond, update, body})->name(name);
			move();
			node_t else_body = this->body();
			return &(new (node) ForElse{tok, init, cond, update, body, else_body})->name(name);
		}
		case Tag::BREAK:
		{
			tok = _M_look;
			move();
			string name;
			if (_M_look->tag() == Tag::ID || _M_look->tag() == Tag::STRING)
			{
				name = ((Ref<Word>)_M_look)->value();
				move();
			}
			return new Break{tok, name};
		}
		case Tag::CONTINUE:
		{
			tok = _M_look;
			move();
			string name;
			if (_M_look->tag() == Tag::ID || _M_look->tag() == Tag::STRING)
			{
				name = ((Ref<Word>)_M_look)->value();
				move();
			}
			return new Continue{tok, name};
		}
		}
		THROW
	}

    Parser::node_t Parser::args()
    {
        node_t node = boolean();
		node_t x = (Node *)(new Args{_M_look, node, nullptr});
		while (_M_look->tag() == ',')
		{
			token_t tok = _M_look;
			move();
			x = (Node *)(new Args{tok, x, new Args{_M_look, boolean(), nullptr}});
		}
		return x;
    }

#define IS_OPT(tok) ((tok)->tag() == '.' || (tok)->tag() == '(' || (tok)->tag() == '[')

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
			x = new Access{tok, obj, new Args{symbol, new Constant(symbol), nullptr}};
			break;
		}
		case '[':
		{
			token_t tok = _M_look;
			move();
			Ref<Args> args = _M_look->tag() != ']' ? Parser::args() : nullptr;
			match(']');
			x = new Access{tok, obj, args};
			break;
		}
		case '(':
		{
			token_t tok = _M_look;
			move();
			Ref<Args> args = _M_look->tag() != ')' ? Parser::args() : nullptr;
			match(')');
			x = new Call{tok, obj, args};
			break;
		}
		}
		return IS_OPT(_M_look) ? opt(x) : x;
	}

    Parser::node_t Parser::pair(node_t x)
    {
		if (x)
			x = new Args{_M_look, x, new Args{_M_look, expr(), nullptr}};
		else
			x = new Args{_M_look, expr(), nullptr};
		match(':'),
		x = new Args{_M_look, x, new Args{_M_look, exprNoComma(), nullptr}};
        return x;
    }
    Parser::node_t Parser::pairs()
    {
        Ref<Args> x = new Args{_M_look, pair(nullptr), nullptr};
		while (_M_look->tag() == ',')
		{
			token_t tok = _M_look;
			move();
			x = pair(x);
		}
		return x;
		
    }
} // namespace phi
