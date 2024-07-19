#include "parser.hpp"
#include <cstdlib>
#include <exception.hpp>
#include <follower.hpp>
#include <new>
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
Parser::node_t phi::Parser::parse() {
    uint64_t tline = getCurrentLine();
    return make_unique<Block>(sequence(), tline);
}
Parser::node_t Parser::body() {
    if (_M_look->tag == '{')
        return block();
    return stmt();
}
Parser::node_t Parser::block() {
    uint64_t tline = getCurrentLine();
    match('{');
    node_t res = sequence();
    match('}');
    return make_unique<Block>(std::move(res), tline);
}
Parser::node_t Parser::sequence() {
    if (_M_generator.eof() || _M_look->tag == '}')
        return nullptr;
    uint64_t tline = getCurrentLine();
    node_t tmp = stmt();
    return make_unique<Sequence>(std::move(tmp), sequence(), tline);
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
        return nullptr;
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
        uint64_t tline = getCurrentLine();
        token_t tok = std::move(_M_look);
        move();
        res = make_unique<BinaryExpr>(std::move(res), boolean(), std::move(tok),
                                      tline);
    }
    return res;
}
Parser::node_t Parser::boolean() {
    node_t res = bor();
    while (_M_look->tag == Tag::OR) {
        uint64_t tline = getCurrentLine();
        token_t tok = std::move(_M_look);
        move();
        res = make_unique<BinaryExpr>(std::move(res), bor(), std::move(tok),
                                      tline);
    }
    return res;
}
Parser::node_t Parser::bor() {
    node_t res = band();
    while (_M_look->tag == '|') {
        uint64_t tline = getCurrentLine();
        token_t tok = std::move(_M_look);
        move();
        res = make_unique<BinaryExpr>(std::move(res), band(), std::move(tok),
                                      tline);
    }
    return res;
}
Parser::node_t Parser::band() {
    node_t res = join();
    while (_M_look->tag == '&') {
        uint64_t tline = getCurrentLine();
        token_t tok = std::move(_M_look);
        move();
        res = make_unique<BinaryExpr>(std::move(res), join(), std::move(tok),
                                      tline);
    }
    return res;
}
Parser::node_t Parser::join() {
    node_t res = equality();
    while (_M_look->tag == Tag::AND) {
        uint64_t tline = getCurrentLine();
        token_t tok = std::move(_M_look);
        move();
        res = make_unique<BinaryExpr>(std::move(res), equality(),
                                      std::move(tok), tline);
    }
    return res;
}
Parser::node_t Parser::equality() {
    node_t res = rel();
    while (_M_look->tag == Tag::EQ || _M_look->tag == Tag::NE) {
        uint64_t tline = getCurrentLine();
        token_t tok = std::move(_M_look);
        move();
        res = make_unique<BinaryExpr>(std::move(res), rel(), std::move(tok),
                                      tline);
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
        uint64_t tline = getCurrentLine();
        token_t tok = std::move(_M_look);
        move();
        res = make_unique<BinaryExpr>(std::move(res), shift(), std::move(tok),
                                      tline);
        break;
    }
    return res;
}
Parser::node_t Parser::shift() {
    node_t res = as();
    while (_M_look->tag == Tag::LSHIFT || _M_look->tag == Tag::RSHIFT) {
        uint64_t tline = getCurrentLine();
        token_t tok = std::move(_M_look);
        move();
        res = make_unique<BinaryExpr>(std::move(res), as(), std::move(tok),
                                      tline);
    }
    return res;
}
Parser::node_t Parser::as() {
    node_t res = mdm();
    while (_M_look->tag == '+' || _M_look->tag == '-') {
        uint64_t tline = getCurrentLine();
        token_t tok = std::move(_M_look);
        move();
        res = make_unique<BinaryExpr>(std::move(res), mdm(), std::move(tok),
                                      tline);
    }
    return res;
}
Parser::node_t Parser::mdm() {
    node_t res = power();
    while (_M_look->tag == '*' || _M_look->tag == '/' || _M_look->tag == '%') {
        uint64_t tline = getCurrentLine();
        token_t tok = std::move(_M_look);
        move();
        res = make_unique<BinaryExpr>(std::move(res), power(), std::move(tok),
                                      tline);
    }
    return res;
}
Parser::node_t Parser::power() {
    node_t res = unary();
    while (_M_look->tag == Tag::POW) {
        uint64_t tline = getCurrentLine();
        token_t tok = std::move(_M_look);
        move();
        res = make_unique<BinaryExpr>(std::move(res), unary(), std::move(tok),
                                      tline);
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
        uint64_t tline = getCurrentLine();
        token_t tok = std::move(_M_look);
        move();
        return make_unique<UnaryExpr>(unary(), std::move(tok), tline);
    }
    return factor();
}
Parser::node_t Parser::factor() {
    node_t res;
    token_t tmp;
    // temp line
    uint64_t tline = getCurrentLine();
    switch (_M_look->tag) {
    case Tag::TRUE:
    case Tag::FALSE:
        tmp = std::move(_M_look);
        move();
        return make_unique<IntegerExpr>(tmp->tag == Tag::TRUE, tline);
    case Tag::INT:
        tmp = std::move(_M_look);
        move();
        return make_unique<IntegerExpr>(tmp->getInt(), tline);
    case Tag::REAL:
        tmp = std::move(_M_look);
        move();
        return make_unique<RealExpr>(tmp->getReal(), tline);
    case Tag::STRING:
        tmp = std::move(_M_look);
        move();
        return make_unique<StringExpr>(tmp->getStringPtr(), tline);
    case '(':
        match('(');
        res = expr();
        match(')');
        if (auto t = opt(res))
            return t;
        return res;
    case '<': {
        match('<');
        auto args = params((Tag)'>');
        match('>');
        match(Tag::ARROW);
        node_t body = stmt();
        if (body->getType() != Expr::Type::BLOCK)
            body = make_unique<Return>(std::move(body), tline);
        return make_unique<Func>(std::move(args), std::move(body), nullptr,
                                 tline);
    }
    case Tag::ID:
        tmp = std::move(_M_look);
        move();
        if (_M_look->tag == Tag::ARROW) {
            move();
            node_t body = stmt();
            if (body->getType() != Expr::Type::BLOCK)
                body = make_unique<Return>(std::move(body), tline);
            return make_unique<Func>(
                std::vector<shared_ptr<string>>{tmp->getStringPtr()},
                std::move(body), nullptr, tline);
        }
        res = make_unique<Load>(tmp->getStringPtr(), false, tline);
        if (auto t = opt(res))
            return t;
        return res;
    case Tag::VAR:
        match(Tag::VAR);
        tmp = std::move(_M_look);
        match(Tag::ID, tmp);
        move();
        return make_unique<Load>(tmp->getStringPtr(), true, tline);
    case Tag::IF: {
        match(Tag::IF);
        match('(');
        node_t cond = expr();
        match(')');
        node_t body = stmt();
        if (_M_look->tag != Tag::ELSE)
            return make_unique<If>(std::move(cond), std::move(body), nullptr,
                                   tline);
        match(Tag::ELSE);
        return make_unique<If>(std::move(cond), std::move(body), stmt(), tline);
    }
    case Tag::WHILE: {
        shared_ptr<string> tag;
        match(Tag::WHILE);
        if (_M_look->tag == ':') {
            match(':');
            if (_M_look->tag == Tag::ID)
                tag = std::move(_M_look)->getStringPtr();
            match(Tag::ID);
        }
        match('(');
        node_t cond = expr();
        match(')');
        native_ptr<While> loop = (native_ptr<While>)malloc(sizeof(While));
        new (&loop->tag) shared_ptr<string>;
        loop->tag = tag;
        Loop::push(loop);
        node_t body = stmt();
        Loop::pop();
        if (_M_look->tag != Tag::ELSE)
            return unique_ptr<While>(new (loop) While(
                tag, std::move(cond), std::move(body), nullptr, tline));
        match(Tag::ELSE);
        return unique_ptr<While>(new (loop) While(
            tag, std::move(cond), std::move(body), stmt(), tline));
    }
    case Tag::FOR: {
        shared_ptr<string> tag;
        match(Tag::FOR);
        if (_M_look->tag == ':') {
            match(':');
            if (_M_look->tag == Tag::ID)
                tag = std::move(_M_look)->getStringPtr();
            match(Tag::ID);
        }
        match('(');
        node_t initializer = expr();
        match(';');
        node_t condition = expr();
        match(';');
        node_t update = expr();
        match(')');
        native_ptr<For> loop = (native_ptr<For>)malloc(sizeof(For));
        new (&loop->tag) shared_ptr<string>;
        loop->tag = tag;
        Loop::push(loop);
        node_t body = stmt();
        Loop::pop();
        if (_M_look->tag != Tag::ELSE)
            return unique_ptr<For>(new (loop) For(
                tag, std::move(initializer), std::move(condition),
                std::move(update), std::move(body), nullptr, tline));
        match(Tag::ELSE);
        return unique_ptr<For>(
            new (loop) For(tag, std::move(initializer), std::move(condition),
                           std::move(update), std::move(body), stmt(), tline));
    }
    case Tag::BREAK: {
        match(Tag::BREAK);
        shared_ptr<string> tag;
        if (_M_look->tag == Tag::ID) {
            tag = _M_look->getStringPtr();
            match(Tag::ID);
        }
        return make_unique<Break>(tag, tline);
    }
    case Tag::CONTINUE: {
        match(Tag::CONTINUE);
        shared_ptr<string> tag;
        if (_M_look->tag == Tag::ID) {
            tag = _M_look->getStringPtr();
            match(Tag::ID);
        }
        return make_unique<Continue>(tag, tline);
    }
    case Tag::FN: {
        match(Tag::FN);
        shared_ptr<string> name;
        if (_M_look->tag == Tag::ID) {
            name = std::move(_M_look)->getStringPtr();
            move();
        }
        match('(');
        std::vector<shared_ptr<string>> args = params((Tag)')');
        match(')');
        node_t body = stmt();
        if (body->getType() != Expr::Type::BLOCK)
            body = make_unique<Return>(std::move(body), tline);
        return make_unique<Func>(std::move(args), std::move(body), name, tline);
    }
    case Tag::RETURN: {
        match(Tag::RETURN);
        return make_unique<Return>(expr(), tline);
    }
    }
    throw SyntaxException("Unexpected token '" + _M_look->toString() + '\'');
}
Parser::node_t Parser::opt(Parser::node_t &target) {
    node_t res;
    uint64_t tline = getCurrentLine();
    switch (_M_look->tag) {
    case '(':
    case Tag::ID:
        move();
        res = make_unique<Call>(std::move(target), args(), tline);
        match(')');
        if (auto tmp = opt(res))
            return tmp;
        return res;
    case '[':
        move();
        res = make_unique<Access>(std::move(target), args(), tline);
        match(']');
        if (auto tmp = opt(res))
            return tmp;
        return res;
    case '.':
        move();
        auto tmp = std::move(_M_look);
        match(Tag::ID, tmp);
        move();
        res = make_unique<Access>(
            std::move(target),
            make_unique<StringExpr>(tmp->getStringPtr(), tline), tline);
        if (auto tmp = opt(res))
            return tmp;
        return res;
    }
    return nullptr;
}
Parser::node_t Parser::args() {
    uint64_t tline = getCurrentLine();
    node_t res = expr();
    while (_M_look->tag == ',')
        res = make_unique<Sequence>(std::move(res), expr(), tline);
    return res;
}
std::vector<shared_ptr<string>> Parser::params(Tag end) {
    std::vector<shared_ptr<string>> args;
    if (_M_look->tag != end) {
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
    return args;
}
} // namespace phi
