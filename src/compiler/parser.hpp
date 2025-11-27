#pragma once
#include "ast.hpp"
#include "token.hpp"
#include <exception.hpp>

namespace phi {
struct Parser {
    Parser(TokenGenerator &generator) : _M_generator(generator) { read(); }

    using node_t = shared_ptr<ast::Node>;
    using token_t = unique_ptr<Token>;
    node_t parse();

  private:
    TokenGenerator &_M_generator;

    token_t _M_sentinel;

    inline void read() { _M_sentinel = _M_generator.next(); }
    inline void read(const Token::Tag &expectation) {
        if (_M_sentinel->tag() != expectation)
            throw SyntaxException("Expected %s, but got %s.",
                                  stringify(expectation).c_str(),
                                  _M_sentinel->stringify().c_str());
        read();
    }
    inline void read(const Token &expectation) {
        if (*_M_sentinel != expectation)
            throw SyntaxException("Expected %s, but got %s.",
                                  expectation.stringify().c_str(),
                                  _M_sentinel->stringify().c_str());
        read();
    }

    inline token_t read_cached() {
        auto tmp = std::move(_M_sentinel);
        _M_sentinel = _M_generator.next();
        return tmp;
    }
    inline token_t read_cached(const Token::Tag &expectation) {
        if (_M_sentinel->tag() != expectation)
            throw SyntaxException("Expected %s, but got %s.",
                                  stringify(expectation).c_str(),
                                  _M_sentinel->stringify().c_str());
        return read_cached();
    }
    inline token_t read_cached(const Token &expectation) {
        if (*_M_sentinel != expectation)
            throw SyntaxException("Expected %s, but got %s.",
                                  expectation.stringify().c_str(),
                                  _M_sentinel->stringify().c_str());
        return read_cached();
    }

    node_t block(bool flag = false);
    node_t seq(char eof);
    node_t stmt();
    node_t expr();
    node_t assign();
    node_t boolean();
    node_t bor();
    node_t band();
    node_t join();
    node_t equality();
    node_t rel();
    node_t shift();
    node_t as();  // addition and subtraction
    node_t mdm(); // Multiplication and division and mod
    node_t power();
    node_t unary();
    node_t factor();
};

} // namespace phi
