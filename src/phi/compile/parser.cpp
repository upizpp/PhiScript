#include "parser.hpp"
#include <exception.hpp>
#include <vector>

namespace phi {
using namespace token;
using namespace ast;
void Parser::move() { _M_look = _M_generator.next(); }
void Parser::match(uint16_t tag) {
    match(tag, _M_look);
    move();
}
void Parser::match(uint16_t tag, token_t &tok) {
    if (tok->tag != tag)
        if (_M_generator.eof())
            throw SyntaxException("Unexpected EOF, expected '" +
                                  Token::tagToString((Tag)tag) + '\'');
        else
            throw SyntaxException("Unexpected token '" + _M_look->toString() +
                                  ", expected '" +
                                  Token::tagToString((Tag)tag) + '\'');
}
Parser::Parser(TokenGenerator &generator) : _M_generator(generator) { move(); }
Parser::node_t phi::Parser::parse() { return make_unique<Block>(sequence()); }
Parser::node_t Parser::body() {
    if (_M_look->tag == '{')
        return block();
    return stmt();
}
Parser::node_t Parser::block() {
    match('{');
    node_t res = sequence();
    match('}');
    return make_unique<Block>(std::move(res));
}
Parser::node_t Parser::sequence() {
    if (_M_generator.eof() || _M_look->tag == '}')
        return nullptr;
    node_t tmp = stmt();
    return make_unique<Sequence>(std::move(tmp), sequence());
}
Parser::node_t Parser::stmt() {
    if (_M_look->tag == '{')
        return block();
    node_t res = expr();
    match(';');
    return res;
}
Parser::node_t Parser::expr() {
    switch (_M_look->tag) {
    case ';':
    case Tag::PASS:
        move();
        return nullptr;
    case '{':
        return block();
    default:
        return assign();
    }
}
Parser::node_t Parser::assign() {
    node_t res = boolean();
    while (_M_look->tag == '=') {
        token_t tok = std::move(_M_look);
        move();
        res =
            make_unique<BinaryExpr>(std::move(res), boolean(), std::move(tok));
    }
    return res;
}
Parser::node_t Parser::boolean() {
    node_t res = bor();
    while (_M_look->tag == Tag::OR) {
        token_t tok = std::move(_M_look);
        move();
        res = make_unique<BinaryExpr>(std::move(res), bor(), std::move(tok));
    }
    return res;
}
Parser::node_t Parser::bor() {
    node_t res = band();
    while (_M_look->tag == '|') {
        token_t tok = std::move(_M_look);
        move();
        res = make_unique<BinaryExpr>(std::move(res), band(), std::move(tok));
    }
    return res;
}
Parser::node_t Parser::band() {
    node_t res = join();
    while (_M_look->tag == '&') {
        token_t tok = std::move(_M_look);
        move();
        res = make_unique<BinaryExpr>(std::move(res), join(), std::move(tok));
    }
    return res;
}
Parser::node_t Parser::join() {
    node_t res = equality();
    while (_M_look->tag == Tag::AND) {
        token_t tok = std::move(_M_look);
        move();
        res =
            make_unique<BinaryExpr>(std::move(res), equality(), std::move(tok));
    }
    return res;
}
Parser::node_t Parser::equality() {
    node_t res = rel();
    while (_M_look->tag == Tag::EQ || _M_look->tag == Tag::NE) {
        token_t tok = std::move(_M_look);
        move();
        res = make_unique<BinaryExpr>(std::move(res), rel(), std::move(tok));
    }
    return res;
}
Parser::node_t Parser::rel() {
    node_t res = shift();
    switch (_M_look->tag) {
    case '<':
    case '>':
    case Tag::LE:
    case Tag::GE:
        token_t tok = std::move(_M_look);
        move();
        res = make_unique<BinaryExpr>(std::move(res), shift(), std::move(tok));
        break;
    }
    return res;
}
Parser::node_t Parser::shift() {
    node_t res = as();
    while (_M_look->tag == Tag::LSHIFT || _M_look->tag == Tag::RSHIFT) {
        token_t tok = std::move(_M_look);
        move();
        res = make_unique<BinaryExpr>(std::move(res), as(), std::move(tok));
    }
    return res;
}
Parser::node_t Parser::as() {
    node_t res = mdm();
    while (_M_look->tag == '+' || _M_look->tag == '-') {
        token_t tok = std::move(_M_look);
        move();
        res = make_unique<BinaryExpr>(std::move(res), mdm(), std::move(tok));
    }
    return res;
}
Parser::node_t Parser::mdm() {
    node_t res = power();
    while (_M_look->tag == '*' || _M_look->tag == '/' || _M_look->tag == '%') {
        token_t tok = std::move(_M_look);
        move();
        res = make_unique<BinaryExpr>(std::move(res), power(), std::move(tok));
    }
    return res;
}
Parser::node_t Parser::power() {
    node_t res = unary();
    while (_M_look->tag == Tag::POW) {
        token_t tok = std::move(_M_look);
        move();
        res = make_unique<BinaryExpr>(std::move(res), unary(), std::move(tok));
    }
    return res;
}
Parser::node_t Parser::unary() {
    switch (_M_look->tag) {
    case '-':
    case '!':
    case '~':
    case '@':
    case Tag::DCPY:
    case Tag::INC:
    case Tag::RED:
        token_t tok = std::move(_M_look);
        move();
        return make_unique<UnaryExpr>(unary(), std::move(tok));
    }
    return factor();
}
Parser::node_t Parser::factor() {
    node_t res;
    token_t tmp;
    switch (_M_look->tag) {
    case Tag::INT:
        tmp = std::move(_M_look);
        move();
        return make_unique<IntegerExpr>(tmp->getInt());
    case Tag::REAL:
        tmp = std::move(_M_look);
        move();
        return make_unique<RealExpr>(tmp->getReal());
    case Tag::STRING:
        tmp = std::move(_M_look);
        move();
        return make_unique<StringExpr>(tmp->getStringPtr());
    case '(':
        match('(');
        res = expr();
        match(')');
        if (auto t = opt(res))
            return t;
        return res;
    case Tag::ID:
        tmp = std::move(_M_look);
        move();
        res = make_unique<Load>(tmp->getStringPtr(), false);
        if (auto t = opt(res))
            return t;
        return res;
    case Tag::VAR:
        match(Tag::VAR);
        tmp = std::move(_M_look);
        match(Tag::ID, tmp);
        move();
        return make_unique<Load>(tmp->getStringPtr(), true);
    case Tag::IF: {
        match(Tag::IF);
        match('(');
        node_t cond = expr();
        match(')');
        node_t body = stmt();
        if (_M_look->tag != Tag::ELSE)
            return make_unique<If>(std::move(cond), std::move(body), nullptr);
        match(Tag::ELSE);
        return make_unique<If>(std::move(cond), std::move(body), stmt());
    }
    case Tag::WHILE: {
        match(Tag::WHILE);
        match('(');
        node_t cond = expr();
        match(')');
        node_t body = stmt();
        if (_M_look->tag != Tag::ELSE)
            return make_unique<While>(std::move(cond), std::move(body),
                                      nullptr);
        match(Tag::ELSE);
        return make_unique<While>(std::move(cond), std::move(body), stmt());
    }
    case Tag::FOR: {
        match(Tag::FOR);
        match('(');
        node_t initializer = expr();
        match(';');
        node_t condition = expr();
        match(';');
        node_t update = expr();
        match(')');
        node_t body = stmt();
        if (_M_look->tag != Tag::ELSE)
            return make_unique<For>(std::move(initializer),
                                    std::move(condition), std::move(update),
                                    std::move(body), nullptr);
        match(Tag::ELSE);
        return make_unique<For>(std::move(initializer), std::move(condition),
                                std::move(update), std::move(body), stmt());
    }
    case Tag::FN: {
        match(Tag::FN);
        shared_ptr<string> name;
        if (_M_look->tag == Tag::ID) {
            name = std::move(_M_look)->getStringPtr();
            move();
        }
        match('(');
        std::vector<shared_ptr<string>> args;
        if (_M_look->tag != ')') {
            auto tmp = std::move(_M_look);
            match(Tag::ID, tmp);
            args.push_back(tmp->getStringPtr());
            move();
        }
        while (_M_look->tag == ',') {
            move();
            auto tmp = std::move(_M_look);
            match(Tag::ID, tmp);
            args.push_back(tmp->getStringPtr());
            move();
        }
        match(')');
        return make_unique<Func>(std::move(args), stmt(), name);
    }
    }
    throw SyntaxException("Unexpected token '" + _M_look->toString() + '\'');
}
Parser::node_t Parser::opt(Parser::node_t &target) {
    node_t res;
    switch (_M_look->tag) {
    case '(':
    case Tag::ID:
        move();
        res = make_unique<Call>(std::move(target), args());
        match(')');
        if (auto tmp = opt(res))
            return tmp;
        return res;
    case '[':
        move();
        res = make_unique<Access>(std::move(target), args());
        match(']');
        if (auto tmp = opt(res))
            return tmp;
        return res;
    case '.':
        move();
        auto tmp = std::move(_M_look);
        match(Tag::ID, tmp);
        move();
        res = make_unique<Access>(std::move(target),
                                  make_unique<StringExpr>(tmp->getStringPtr()));
        if (auto tmp = opt(res))
            return tmp;
        return res;
    }
    return nullptr;
}
Parser::node_t Parser::args() {
    node_t res = expr();
    while (_M_look->tag == ',')
        res = make_unique<Sequence>(std::move(res), expr());
    return res;
}
} // namespace phi
