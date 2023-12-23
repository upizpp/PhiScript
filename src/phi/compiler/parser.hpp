#pragma once
#include "ast/node.hpp"
#include <phi/runtime/expression.hpp>
#include "token.hpp"
#include <phi/function.hpp>

namespace phi
{
    namespace compiler
    {
        class Parser
        {
        private:
        public:
            static runtime::Expression parseExpr(const Token::Container::iterator& begin, const Token::Container::iterator& end);
            static ast::Node parseStmts(Token::Container *tokens, Ref<runtime::Block> = nullptr);
        };

        class ParserImpl
        {
        private:
            Token::Container::iterator m_iterator;
            Token::Container::iterator m_end;
            Token::Container::iterator m_itNext();
            Token::Container::iterator m_match(const std::string &expected);
            Token::Container::iterator m_match(const Token::Type &type);
            Token::Container::iterator m_match(const std::set<Token::Type> &types);
            ast::Node *m_readStatements(ast::Node *node);
        public:
            static Token::Container::iterator getPair(const Token::Container::iterator &end_it, Token::Container::iterator from, const std::string &begin, const std::string &end);
            static Token::Container::iterator getUntil(const Token::Container::iterator &end, Token::Container::iterator from, const std::string &content);
            static Token::Container::iterator findToken(const Token::Container::iterator &end, Token::Container::iterator from, const std::string &content);

            ast::Node *parseStmts(ast::Node *root, const Token::Container::iterator &begin, const Token::Container::iterator &end, Ref<runtime::Block> block = nullptr);
            Function parseFunction(const Token::Container::iterator &begin, const Token::Container::iterator &end, Ref<runtime::Block> block = nullptr);
        };
    } // namespace compiler
} // namespace phi
