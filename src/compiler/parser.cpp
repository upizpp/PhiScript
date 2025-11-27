#include "parser.hpp"

#define READ_CACHED(expected) tok = std::move(read_cached(expected));

namespace phi {
using namespace ast;
Parser::node_t Parser::parse() { return block(true); }
Parser::node_t Parser::block(bool flag) {
    if (!flag)
        read('{');
    node_t res = make_shared<ast::Block>(seq('}'));
    if (!flag)
        read('}');
    return (node_t)res;
}
Parser::node_t Parser::seq(char eof) {
    shared_ptr<ast::Sequence> front = make_shared<ast::Sequence>();
    shared_ptr<ast::Sequence> tmp = front;
    while (1) {
        tmp->expr = stmt();
        if (_M_generator.eof() || _M_sentinel->tag() == eof)
            break;
        tmp->next = make_shared<ast::Sequence>();
        tmp = tmp->next;
    }
    return node_t(front);
}
Parser::node_t Parser::stmt() {
    switch (_M_sentinel->tag()) {
    case '{':
        return block();
    }
    node_t res = expr();
    read(';');
    return res;
}
Parser::node_t Parser::expr() {
    switch (_M_sentinel->tag()) {
    case ';':
        return nullptr;
    }
    return assign();
}
Parser::node_t Parser::assign() {
    node_t x = boolean();
    while (_M_sentinel->tag() == '=') {
        token_t tok = std::move(_M_sentinel);
        read();
        x = (node_t)(new Binary{std::move(tok), x, boolean()});
    }
    return x;
}
Parser::node_t Parser::boolean() {
    node_t x = bor();
    while (_M_sentinel->tag() == Token::Tag::OR) {
        token_t tok = std::move(_M_sentinel);
        read();
        x = (node_t)(new Binary(std::move(tok), x, bor()));
    }
    return x;
}
Parser::node_t Parser::bor() {
    node_t x = band();
    while (_M_sentinel->tag() == '|') {
        token_t tok = std::move(_M_sentinel);
        read();
        x = (node_t)(new Binary(std::move(tok), x, band()));
    }
    return x;
}
Parser::node_t Parser::band() {
    node_t x = join();
    while (_M_sentinel->tag() == '&') {
        token_t tok = std::move(_M_sentinel);
        read();
        x = (node_t)(new Binary(std::move(tok), x, join()));
    }
    return x;
}
Parser::node_t Parser::join() {
    node_t x = equality();
    while (_M_sentinel->tag() == Token::Tag::AND) {
        token_t tok = std::move(_M_sentinel);
        read();
        x = (node_t)(new Binary(std::move(tok), x, equality()));
    }
    return x;
}
Parser::node_t Parser::equality() {
    node_t x = rel();
    while (_M_sentinel->tag() == Token::Tag::EQ ||
           _M_sentinel->tag() == Token::Tag::NE) {
        token_t tok = std::move(_M_sentinel);
        read();
        x = (node_t)(new Binary(std::move(tok), x, rel()));
    }
    return x;
}
Parser::node_t Parser::rel() {
    node_t x = shift();
    switch (_M_sentinel->tag()) {
    case '<':
    case '>':
    case Token::Tag::LE:
    case Token::Tag::GE:
        token_t tok = std::move(_M_sentinel);
        read();
        return (node_t)(new Binary(std::move(tok), x, shift()));
    }
    return x;
}
Parser::node_t Parser::shift() {
    node_t x = as();
    while (_M_sentinel->tag() == Token::Tag::LSHIFT ||
           _M_sentinel->tag() == Token::Tag::RSHIFT) {
        token_t tok = std::move(_M_sentinel);
        read();
        x = (node_t)(new Binary(std::move(tok), x, as()));
    }
    return x;
}
Parser::node_t Parser::as() {
    node_t x = mdm();
    while (_M_sentinel->tag() == '+' || _M_sentinel->tag() == '-') {
        token_t tok = std::move(_M_sentinel);
        read();
        x = (node_t)(new Binary(std::move(tok), x, mdm()));
    }
    return x;
}
Parser::node_t Parser::mdm() {
    node_t x = power();
    while (_M_sentinel->tag() == '*' || _M_sentinel->tag() == '/' ||
           _M_sentinel->tag() == '%') {
        token_t tok = std::move(_M_sentinel);
        read();
        x = (node_t)(new Binary(std::move(tok), x, power()));
    }
    return x;
}
Parser::node_t Parser::power() {
    node_t x = unary();
    while (_M_sentinel->tag() == Token::Tag::POW) {
        token_t tok = std::move(_M_sentinel);
        read();
        x = (node_t)(new Binary(std::move(tok), x, unary()));
    }
    return x;
}
Parser::node_t Parser::unary() {
    switch (_M_sentinel->tag()) {
    case '-':
    case '!':
    case '~':
    case '@':
    case Token::Tag::DCP:
    case Token::Tag::INC:
    case Token::Tag::DEC:
        token_t tok = std::move(_M_sentinel);
        read();
        return (node_t)(new Unary(std::move(tok), unary()));
    }
    return factor();
}
Parser::node_t Parser::factor() {
    node_t x;
    token_t tok;
    switch (_M_sentinel->tag()) {
    case '(':
        read();
        x = expr();
        read(')');
        return x;
    case Token::Tag::INT:
    case Token::Tag::REAL:
    case Token::Tag::STRING:
        READ_CACHED();
        return (node_t)(new Constant(std::move(tok)));
    case Token::Tag::IDENTIFIER:
        READ_CACHED();
        return (node_t)(new Load(std::move(tok), false));
    case Token::Tag::VAR:
        read();
        READ_CACHED(Token::Tag::IDENTIFIER);
        return (node_t)(new Load(std::move(tok), true));
    }
    throw SyntaxException("Unexpected token.");
    return nullptr;
}
} // namespace phi
