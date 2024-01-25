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
                "Unexpected token " + string(*_M_look) +
                " at line " + std::to_string(_M_look->line()) +
                ", expected " + std::to_string(tag));
    }

    void Parser::move()
    {
        ++_M_it;
        _M_look = _M_it != _M_tokens->end() ? *_M_it : nullptr;
        // throw SyntaxException("Unexpected end of program.");
    }

    Parser::node Parser::parse(token::tokens &tokens)
    {
        if (tokens.empty())
            return new Block;
        tokens.insert(tokens.begin(), new Token('{'));
        tokens.insert(tokens.end(), new Token('}'));
        return program(tokens);
    }

    Parser::node Parser::program(const tokens &tokens)
    {
        if (tokens.empty())
            return new Block;
        _M_tokens = &tokens;
        _M_it = tokens.begin();
        _M_look = *_M_it;

        return block();
    }

    Parser::node Parser::block()
    {
        match('{');
        auto block = new Block{(Ref<Expr>)sequence()};
        match('}');
        return block;
    }
    Parser::node Parser::sequence()
    {
        if (_M_look->tag() == '}')
            return nullptr;
        if (_M_look->tag() == ';')
            return (move(), sequence());
        node x = expr();
        CHECK_NONE;
        return (Node *)(new Sequence(x, sequence()));
    }
    Parser::node Parser::expr()
    {
        switch (_M_look->tag())
        {
        case '{':
            return block();
        case ';':
            return nullptr;
        }
        return assign();
    }
    Parser::node Parser::assign()
    {
        node x = boolean();
        while (_M_look->tag() == '=')
        {
            Ref<Token> tok = _M_look;
            move();
            x = (Node *)(new Binary(tok, x, boolean()));
        }
        return x;
    }
    Parser::node Parser::boolean()
    {
        node x = join();
        while (_M_look->tag() == Tag::OR)
        {
            Ref<Token> tok = _M_look;
            move();
            x = (Node *)(new Binary(tok, x, join()));
        }
        return x;
    }
    Parser::node Parser::join()
    {
        node x = equality();
        while (_M_look->tag() == Tag::AND)
        {
            Ref<Token> tok = _M_look;
            move();
            x = (Node *)(new Binary(tok, x, equality()));
        }
        return x;
    }
    Parser::node Parser::equality()
    {
        node x = rel();
        while (_M_look->tag() == Tag::EQ || _M_look->tag() == Tag::NE)
        {
            Ref<Token> tok = _M_look;
            move();
            x = (Node *)(new Binary(tok, x, rel()));
        }
        return x;
    }
    Parser::node Parser::rel()
    {
        node x = as();
        switch (_M_look->tag())
        {
        case '<':
        case '>':
        case Tag::LE:
        case Tag::GE:
            Ref<Token> tok = _M_look;
            move();
            return (Node *)(new Binary(tok, x, as()));
        }
        return x;
    }
    Parser::node Parser::as()
    {
        node x = md();
        while (_M_look->tag() == '+' || _M_look->tag() == '-')
        {
            Ref<Token> tok = _M_look;
            move();
            x = (Node *)(new Binary(tok, x, md()));
        }
        return x;
    }
    Parser::node Parser::md()
    {
        node x = unary();
        while (_M_look->tag() == '*' || _M_look->tag() == '/')
        {
            Ref<Token> tok = _M_look;
            move();
            x = (Node *)(new Binary(tok, x, unary()));
        }
        return x;
    }
    Parser::node Parser::unary()
    {
        switch (_M_look->tag())
        {
        case '-':
        case '!':
        case '~':
        case Tag::INC:
        case Tag::RED:
            Ref<Token> tok = _M_look;
            move();
            return (Node *)(new Unary(tok, unary()));
        }
        return factor();
    }
    Parser::node Parser::factor()
    {
        node x;
        Ref<Token> tok;
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
                // TODO Call
                break;
            case '[':
            case '.':
                // TODO Access
                break;
            default:
                return new Load(tok, false);
            }
        case Tag::VAR:
            move();
            tok = _M_look;
            move();
            return new Load(tok, true);
        case Tag::EVAL:
            move();
            return new Eval(Ref<Expr>(expr()));
        case Tag::IF:
        {
            move();
            match('(');
            x = expr();
            match(')');
            node body = expr();
            if (_M_look->tag() != Tag::ELSE)
                return new If(x, body);
            move();
            node else_body = expr();
            return new IfElse(x, body, else_body);
        }
        case Tag::WHILE:
        {
            move();
            match('(');
            x = expr();
            match(')');
            node body = expr();
            if (_M_look->tag() != Tag::ELSE)
                return new While(x, body);
            move();
            node else_body = expr();
            return new WhileElse(x, body, else_body);
        }
        case Tag::FOR:
        {
            move();
            match('(');
            node init = expr();
            match(';');
            node cond = expr();
            match(';');
            node update = expr();
            match(')');
            node body = expr();
            if (_M_look->tag() != Tag::ELSE)
                return new For(init, cond, update, body);
            move();
            node else_body = expr();
            return new ForElse(init, cond, update, body, else_body);
        }
        }
        THROW;
    }
    Parser::node Parser::access()
    {
        return node();
    }
} // namespace phi
