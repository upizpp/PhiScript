#include "parser.hpp"
#include <exception.hpp>

namespace phi {
using namespace token;
using namespace ast;
Parser::node_t phi::Parser::parse() { return node_t(); }
void Parser::move() { _M_look = _M_generator.next(); }
bool Parser::match(token::Tag tag) {
    if (_M_look->tag == tag)
        move();
    else if (_M_generator.eof())
        throw SyntaxException("Unexpected EOF, expected '" +
                              Token::tagToString(tag) + '\'');
    else
        throw SyntaxException("Unexpected token '" + _M_look->toString() +
                              ", expected '" + Token::tagToString(tag) +
                              '\'');
}
} // namespace phi
