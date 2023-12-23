#include "parser.hpp"
#include "analyzer.hpp"
#include <map>
#include <phi/exception.hpp>
#include <phi/function.hpp>
#include <phi/phi_entry.hpp>
#include <phi/phi_std.hpp>
#include <phi/runtime/op_code.hpp>
#include <stack>
#include <string>
#include <vector>

#include "ast/ast"

#define NewOC_P(pos, ...) OPCode{__VA_ARGS__}.position(pos)
#define NewOC(...) NewOC_P(token.position(), __VA_ARGS__)
#define POP_WHILE(condition)                  \
    while (!operators.empty() && (condition)) \
    {                                         \
        pushOperator(expr, operators);        \
    }

namespace phi
{
    namespace compiler
    {
        using namespace runtime;

        inline static OPCode getBinaryCode(Token &token)
        {
            switch (token.type())
            {
            case Token::Type::NUMBER:
            case Token::Type::BOOLEAN:
                return NewOC(parseNumber(token.content()));
            case Token::Type::STRING:
                return NewOC(Variant{token.content()});
            case Token::Type::OPERATOR:
                return NewOC(getBinaryCommand(token.content()));
            default:
                return OPCode();
            }
        }

        inline static OPCode getUnaryCode(Token &token)
        {
            switch (token.type())
            {
            case Token::Type::NUMBER:
            case Token::Type::BOOLEAN:
                return NewOC(Variant{parseNumber(token.content())});
            case Token::Type::STRING:
                return NewOC(Variant{token.content()});
            case Token::Type::OPERATOR:
                return NewOC(getUnaryCommand(token.content()));
            default:
                return OPCode();
            }
        }

        inline static void pushOperator(Expression &expression, std::stack<Token *> &m_operators)
        {
            auto op = m_operators.top();
            m_operators.pop();

            if (op->content() == ".")
            {
                expression.push(NewOC_P(op->position(), Variant{1}));
            }
            expression.push(getBinaryCode(*op));
        }

        inline static bool isLoading(const Token::Container::iterator &it, const Token::Container::iterator &end)
        {
            auto last = it;
            --last;
            if (last == end)
                return true;
            return !last->isPunctuationType();
        }

        Expression phi::compiler::Parser::parseExpr(const Token::Container::iterator &begin, const Token::Container::iterator &end)
        {
            std::stack<OPCode::Command> args_flag;
            std::stack<Token *> operators;
            std::stack<OPCode> to_push;
            std::stack<integer> call_bracket_flag;
            std::stack<OPCode::Command> middle_bracket_command;
            // Used to record whether it is in the dictionary and whether the corresponding ':' character appears
            std::stack<bool> dictionary_marker;
            std::map<Token::KeywordType, bool> kw_used;

            auto use_kw = [&kw_used](const Token::KeywordType &kw)
            {
                bool temp = kw_used[kw];
                kw_used[kw] = false;
                return temp;
            };

            Expression expr;

            for (auto it = begin; it != end; ++it)
            {
                Token &token = *it;
                PhiSetPosition(token.position());
                switch (token.type())
                {
                case Token::Type::KEYWORD:
                {
                    if (kw_used[token.keywordType()])
                        throw SyntaxException("Unexpected keyword '%s'.", token.content().c_str());
                    switch (token.keywordType())
                    {
                    case Token::KeywordType::FUNCTION:
                    {
                        auto border = ParserImpl::getPair(end, ParserImpl::findToken(end, it, "{"), "{", "}");
                        ++border;
                        ParserImpl parser;
                        Function func = parser.parseFunction(it, border);
                        if (func.hasName())
                        {
                            expr.push(NewOC(new Variant{func.name()}));
                            expr.push(NewOC(OPCode::Command::ALLOCATE));
                            expr.push(NewOC(new Variant{func}));
                            expr.push(NewOC(OPCode::Command::ASSIGN));
                        }
                        else
                            expr.push(NewOC(new Variant{func}));
                        expr.push(NewOC(OPCode::Command::FN_LOCATE));
                        it = border;
                        break;
                    }
                    default:
                        kw_used[token.keywordType()] = true;
                        break;
                    }
                    break;
                }
                case Token::Type::SYMBOL:
                {
                    if (kw_used[Token::KeywordType::VAR])
                        throw SyntaxException("Unexpected keyword 'var'.");
                    switch (token.content()[0])
                    {
                    case '(':
                        // call
                        if (isLoading(it, end))
                        {
                            auto next = it;
                            ++next;
                            args_flag.push(OPCode::Command::CALL);
                            expr.push((NewOC(OPCode::Command::ARGS)));
                            call_bracket_flag.push(0);
                            operators.push(&token);
                        }
                        // change priority
                        else
                        {
                            if (!call_bracket_flag.empty())
                                ++call_bracket_flag.top();
                            operators.push(&token);
                        }
                        break;
                    case ')':
                    {
                        POP_WHILE(operators.top()->type() != Token::Type::SYMBOL);
                        if (!operators.empty())
                            operators.pop();
                        if (!call_bracket_flag.empty())
                        {
                            --call_bracket_flag.top();
                            if (call_bracket_flag.top() < 0)
                            {
                                call_bracket_flag.pop();
                                expr.push(NewOC(OPCode::Command::CALL));
                                args_flag.pop();
                            }
                        }
                        break;
                    }
                    case '[':
                    {
                        // access
                        if (isLoading(it, end))
                        {
                            args_flag.push(OPCode::Command::ACCESS);
                            middle_bracket_command.push(OPCode::Command::ACCESS);
                        }
                        // array
                        else
                        {
                            args_flag.push(OPCode::Command::MAKE_ARRAY);
                            middle_bracket_command.push(OPCode::Command::MAKE_ARRAY);
                        }
                        auto next = it;
                        ++next;
                        expr.push((NewOC(OPCode::Command::ARGS)));
                        operators.push(&token);
                        break;
                    }
                    case ']':
                    {
                        POP_WHILE(operators.top()->type() != Token::Type::SYMBOL);
                        if (!operators.empty())
                            operators.pop();
                        expr.push(NewOC(middle_bracket_command.top()));
                        args_flag.pop();
                        middle_bracket_command.pop();
                        break;
                    }
                    case '{':
                    {
                        auto next = it;
                        ++next;
                        expr.push((NewOC(OPCode::Command::ARGS)));
                        args_flag.push(OPCode::Command::MAKE_DICTIONARY);
                        operators.push(&token);
                        dictionary_marker.push(false);
                        break;
                    }
                    case '}':
                    {
                        POP_WHILE(operators.top()->type() != Token::Type::SYMBOL);
                        if (!operators.empty())
                            operators.pop();
                        args_flag.pop();
                        expr.push(NewOC(OPCode::Command::MAKE_DICTIONARY));
                        break;
                    }
                    case ':':
                    {
                        if (dictionary_marker.empty())
                            throw SyntaxException("Unknown token ':' in the expression.");
                        if (dictionary_marker.top())
                            throw SyntaxException("Token ':' has already appeared once in a key-value pair.");
                        dictionary_marker.top() = true;
                        break;
                    }
                    case ',':
                    {
                        if (args_flag.top() == OPCode::Command::MAKE_DICTIONARY)
                            dictionary_marker.top() = false;
                        break;
                    }
                    }
                    break;
                }
                case Token::Type::OPERATOR:
                {
                    if (kw_used[Token::KeywordType::VAR])
                        throw SyntaxException("Unexpected keyword 'var'.");
                    if (it->content() == ".")
                    {
                        ++it;
                        if (it == end || it->type() != Token::Type::IDENTIFIER)
                            throw SyntaxException("Identifier expected after '.' .");
                        expr.push((NewOC(OPCode::Command::ARGS)));
                        expr.push(NewOC(Variant{it->content()}));
                        expr.push(NewOC(OPCode::Command::ACCESS));
                        break;
                    }
                    auto front = it;
                    front--;
                    auto back = it;
                    back++;
                    bool front_condition = front != end ? front->isPunctuationType() : true;
                    bool back_condition = back != end ? !back->isPunctuationType() : false;
                    if (
                        (
                            !isBinary(token.content()) && isUnary(token.content())) ||
                        (isBinary(token.content()) && isUnary(token.content()) &&
                         front_condition &&
                         back_condition))
                    {
                        to_push.push(getUnaryCode(token));
                        goto loop_end;
                    }

                    POP_WHILE(token.priority() <= operators.top()->priority() &&
                              operators.top()->type() != Token::Type::SYMBOL);
                    operators.push(&token);
                    break;
                }
                case Token::Type::IDENTIFIER:
                {
                    if (it != begin)
                    {
                        auto l_it = it;
                        --l_it;
                        if (l_it->content() == ".")
                            expr.push(NewOC(Variant{token.content()}));
                        else
                            goto load_variable;
                    }
                    else
                    {
                    load_variable:;
                        expr.push(NewOC(Variant{token.content()}));
                        expr.push(NewOC(use_kw(Token::KeywordType::VAR) ? OPCode::Command::ALLOCATE : OPCode::Command::LOAD));
                    }
                    break;
                }
                case Token::Type::BOOLEAN:
                case Token::Type::NUMBER:
                case Token::Type::STRING:
                {
                    if (kw_used[Token::KeywordType::VAR])
                        throw SyntaxException("Unexpected keyword 'var'.");
                    expr.push(getBinaryCode(token));
                    break;
                }

                default:
                    break;
                }

                while (!to_push.empty())
                {
                    expr.push(to_push.top());
                    to_push.pop();
                }

            loop_end:;
            }

            while (!operators.empty())
                pushOperator(expr, operators);

            // TODO Static Analyze
            // StaticAnalyzer::analyze(&expr);

            return expr;
        }

        Token::Container::iterator ParserImpl::getPair(const Token::Container::iterator &end_it, Token::Container::iterator from, const std::string &begin, const std::string &end)
        {
            uinteger count = 0;
            do
            {
                if (from->content() == begin)
                    ++count;
                if (from->content() == end)
                    --count;
                ++from;
            } while (from != end_it && count != 0);
            --from;
            return from;
        }

        Function ParserImpl::parseFunction(const Token::Container::iterator &begin, const Token::Container::iterator &end, Ref<runtime::Block> block)
        {
            // TODO parseFunction
            if (begin->keywordType() != Token::KeywordType::FUNCTION)
                throw SyntaxException("Unknown function syntax.");
            Function func;
            this->m_iterator = begin;
            this->m_end = end;
            m_itNext();
            if (m_iterator->type() == Token::Type::IDENTIFIER)
                func.name(m_iterator->content());
            else
                --m_iterator;
            m_match("(");
            m_itNext();
            std::vector<std::string> args;
            if (m_iterator->content() == ")")
            {
                m_match("{");
            }
            else
            {
                while (true)
                {
                    if (m_iterator->type() != Token::Type::IDENTIFIER)
                        throw SyntaxException("Unknown argument for the function, identifier expected.");
                    if (std::find(args.begin(), args.end(), m_iterator->content()) != args.end())
                        throw SyntaxException("Duplicate argument for the function. (%s)", m_iterator->content().c_str());
                    args.push_back(m_iterator->content());
                    m_itNext();
                    if (m_iterator->content() == ",")
                        m_itNext();
                    else if (m_iterator->content() == ")")
                    {
                        m_match("{");
                        break;
                    }
                }
            }
            func.setArgs(args);
            auto border = getPair(end, m_iterator, "{", "}");
            m_itNext();
            func.setNode(parseStmts(new ast::Node, m_iterator, border));

            return func;
        }

        Token::Container::iterator ParserImpl::getUntil(const Token::Container::iterator &end, Token::Container::iterator from, const std::string &content)
        {
            while (from != end && from->content() != content)
            {
                ++from;
                if (from->content() == "{")
                {
                    from = getPair(end, from, "{", "}");
                    ++from;
                }
            }
            if (from == end)
                throw SyntaxException("Expected to find the token '%s' but not found.", content.c_str());
            ++from;
            return from;
        }

        Token::Container::iterator ParserImpl::findToken(const Token::Container::iterator &end, Token::Container::iterator from, const std::string &content)
        {
            while (from != end && from->content() != content)
                ++from;
            if (from == end)
                throw SyntaxException("Expected to find the token '%s' but not found.", content.c_str());
            return from;
        }

        Token::Container::iterator ParserImpl::m_itNext()
        {
            ++m_iterator;
            if (m_iterator == m_end)
                throw SyntaxException("Unexpected statement(It should not end here).");
            return m_iterator;
        };

        Token::Container::iterator ParserImpl::m_match(const std::string &expected)
        {
            ++m_iterator;
            if (m_iterator == m_end)
                throw SyntaxException("Unexpected statement(It should not end here).");
            if (m_iterator->content() != expected)
                throw SyntaxException("Unknown syntax. (%s expected)", expected.c_str());
            return m_iterator;
        };
        Token::Container::iterator ParserImpl::m_match(const Token::Type &type)
        {
            ++m_iterator;
            if (m_iterator == m_end)
                throw SyntaxException("Unexpected statement(It should not end here).");
            if (m_iterator->type() != type)
                throw SyntaxException("Unknown syntax. (%s expected)", Token::stringifyType(type).c_str());
            return m_iterator;
        };
        Token::Container::iterator ParserImpl::m_match(const std::set<Token::Type> &types)
        {
            ++m_iterator;
            if (m_iterator == m_end)
                throw SyntaxException("Unexpected statement(It should not end here).");
            if (types.find(m_iterator->type()) == types.end())
                throw SyntaxException("Unknown syntax.");
            return m_iterator;
        };
        ast::Node *ParserImpl::m_readStatements(ast::Node *node)
        {
            if (m_iterator->content() == "{")
            {
                auto border = getPair(m_end, m_iterator, "{", "}");
                m_itNext();
                parseStmts(node, m_iterator, border);
                m_iterator = border;
                ++m_iterator;
                return node;
            }
            if (isStructuredKeyword(m_iterator->content()))
            {
                auto condition_begin = m_iterator;
                ++condition_begin;
                auto condition_end = getPair(m_end, condition_begin, "(", ")");
                ++condition_end;
                Token::Container::iterator border;
                Token::Container::iterator begin;
                begin = condition_end;
                if (condition_end->content() == "{")
                {
                    ++begin;
                    border = getPair(m_end, begin, "{", "}");
                }
                else
                {
                    border = findToken(m_end, begin, ";");
                }
                ++border;
                while (border != m_end && isStructuredKeyword(border->content()))
                {
                    ++border;
                    if (border->content() == "{")
                    {
                        border = ++getPair(m_end, findToken(m_end, border, "{"), "{", "}");
                    }
                    else
                    {
                        border = findToken(m_end, border, ";");
                        ++border;
                    }
                }

                parseStmts(node, m_iterator, border);
                m_iterator = border;
                if (m_iterator == m_end)
                    return node;
                return node;
            }
            {
                auto border = getUntil(m_end, m_iterator, ";");
                parseStmts(node, m_iterator, border);
                m_iterator = border;
                return node;
            }
        };

        ast::Node *ParserImpl::parseStmts(ast::Node *root, const Token::Container::iterator &begin, const Token::Container::iterator &end, Ref<runtime::Block> block)
        {
            root->block() = block != nullptr ? block : new Block;
            this->m_iterator = begin;
            this->m_end = end;

            while (m_iterator != end)
            {
                if (m_iterator->type() == Token::Type::KEYWORD)
                {
                    switch (m_iterator->keywordType())
                    {
                    case Token::KeywordType::IMPORT:
                    {
                        m_match({Token::Type::IDENTIFIER, Token::Type::STRING});
                        std::string module_name = m_iterator->content();
                        m_itNext();
                        std::string import_name;
                        if (m_iterator->keywordType() == Token::KeywordType::AS)
                            import_name = m_match(Token::Type::IDENTIFIER)->content();
                        else
                        {
                            --m_iterator;
                            import_name = module_name;
                        }
                        m_match(";");
                        ++m_iterator;
                        Ref<ast::ImportNode> import_node{new ast::ImportNode{import_name, module_name}};
                        import_node->block() = root->block();
                        import_node->block().detach();
                        root->addChild(import_node);
                        break;
                    }
                    case Token::KeywordType::IF:
                    {
                        m_match("(");
                        auto condition_border = getPair(end, m_iterator, "(", ")");
                        m_itNext();
                        Expression condition = Parser::parseExpr(m_iterator, condition_border);
                        m_iterator = condition_border;
                        m_itNext();
                        Ref<ast::IfNode> if_node = static_cast<ast::IfNode *>(m_readStatements(new ast::IfNode));
                        if_node->setCondition(std::move(condition));
                        if_node->block()->setParent(root->block());
                        root->addChild(if_node);
                        if (m_iterator == end)
                            goto break_loop;
                        if (m_iterator->keywordType() != Token::KeywordType::ELSE)
                            break;
                        m_itNext();
                        Owner<ast::ElseNode> else_node = static_cast<ast::ElseNode *>(m_readStatements(new ast::ElseNode));
                        else_node->block()->setParent(root->block());
                        if_node->addChild(else_node.asRef());
                        if_node->setElseNode(std::move(else_node));

                        break;
                    }
                    case Token::KeywordType::WHILE:
                    {
                        const std::string *label = nullptr;
                        m_itNext();
                        if (m_iterator->type() == Token::Type::SYMBOL && m_iterator->content() == ":")
                        {
                            m_match({Token::Type::IDENTIFIER, Token::Type::STRING});
                            label = &m_iterator->content();
                        }
                        else
                            --m_iterator;
                        m_match("(");
                        auto condition_border = getPair(end, m_iterator, "(", ")");
                        m_itNext();
                        Expression condition = Parser::parseExpr(m_iterator, condition_border);
                        m_iterator = condition_border;
                        m_itNext();
                        Ref<ast::WhileNode> while_node = static_cast<ast::WhileNode *>(m_readStatements(new ast::WhileNode));
                        while_node->setCondition(std::move(condition));
                        while_node->block()->setParent(root->block());
                        if (label)
                            while_node->label(*label);
                        root->addChild(while_node);
                        if (m_iterator == end)
                            goto break_loop;
                        if (m_iterator->keywordType() != Token::KeywordType::ELSE)
                            break;
                        m_itNext();
                        Owner<ast::ElseNode> else_node = static_cast<ast::ElseNode *>(m_readStatements(new ast::ElseNode));
                        else_node->block()->setParent(root->block());
                        while_node->addChild(else_node.asRef());
                        while_node->setElseNode(std::move(else_node));

                        break;
                    }
                    case Token::KeywordType::FOR:
                    {
                        const std::string *label = nullptr;
                        m_itNext();
                        if (m_iterator->type() == Token::Type::SYMBOL && m_iterator->content() == ":")
                        {
                            m_match({Token::Type::IDENTIFIER, Token::Type::STRING});
                            label = &m_iterator->content();
                        }
                        else
                            --m_iterator;
                        m_match("(");
                        auto border = getPair(end, m_iterator, "(", ")");
                        m_itNext();
                        auto entry_border = getUntil(border, m_iterator, ";");
                        auto condition_border = getUntil(border, entry_border, ";");
                        auto handler_border = border;
                        Expression entry = Parser::parseExpr(m_iterator, entry_border);
                        Expression condition = Parser::parseExpr(entry_border, condition_border);
                        Expression handler = Parser::parseExpr(condition_border, handler_border);
                        m_iterator = handler_border;
                        m_itNext();
                        Ref<ast::ForNode> for_node = static_cast<ast::ForNode *>(m_readStatements(new ast::ForNode));
                        for_node->setEntry(std::move(entry));
                        for_node->setCondition(std::move(condition));
                        for_node->setHandler(std::move(handler));
                        for_node->block()->setParent(root->block());
                        if (label)
                            for_node->label(*label);
                        root->addChild(for_node);

                        if (m_iterator == end)
                            goto break_loop;
                        if (m_iterator->keywordType() != Token::KeywordType::ELSE)
                            break;
                        m_itNext();
                        Owner<ast::ElseNode> else_node = static_cast<ast::ElseNode *>(m_readStatements(new ast::ElseNode));
                        else_node->block()->setParent(root->block());
                        for_node->addChild(else_node.asRef());
                        for_node->setElseNode(std::move(else_node));
                        break;
                    }
                    case Token::KeywordType::BREAK:
                    {
                        Ref<ast::BreakNode> break_node{new ast::BreakNode};
                        m_itNext();
                        if (m_iterator->type() == Token::Type::IDENTIFIER || m_iterator->type() == Token::Type::STRING)
                        {
                            break_node->label(m_iterator->content());
                            m_match(";");
                        }
                        else if (m_iterator->content() != ";")
                            throw SyntaxException("Invalid break statement.");
                        root->addChild(break_node);
                        ++m_iterator;
                        break;
                    }
                    case Token::KeywordType::CONTINUE:
                    {
                        Ref<ast::ContinueNode> continue_node{new ast::ContinueNode};
                        m_itNext();
                        if (m_iterator->type() == Token::Type::IDENTIFIER || m_iterator->type() == Token::Type::STRING)
                        {
                            continue_node->label(m_iterator->content());
                            m_match(";");
                        }
                        else if (m_iterator->content() != ";")
                            throw SyntaxException("Invalid break statement.");
                        root->addChild(continue_node);
                        ++m_iterator;
                        break;
                    }
                    case Token::KeywordType::RETURN:
                    {
                        m_itNext();
                        auto border = getUntil(end, m_iterator, ";");
                        Expression expr = Parser::parseExpr(m_iterator, border);
                        Ref<ast::ReturnNode> return_node{new ast::ReturnNode};
                        return_node->block() = root->block();
                        return_node->block().detach();
                        return_node->setExpression(expr);
                        root->addChild(return_node);

                        m_iterator = border;
                        break;
                    }
                    default:
                        goto parse_expr;
                    }
                    continue;
                break_loop:
                    break;
                }

                {
                parse_expr:
                    auto border = getUntil(end, m_iterator, ";");
                    Expression expr = Parser::parseExpr(m_iterator, border);
                    Ref<ast::Node> stmt{new ast::StatementNode(expr)};
                    stmt->block() = root->block();
                    stmt->block().detach();
                    root->addChild(stmt);

                    m_iterator = border;
                }
            }

            return root;
        }

        ast::Node Parser::parseStmts(Token::Container *tokens, Ref<runtime::Block> block)
        {
            ParserImpl parser;
            Owner<ast::Node> node = parser.parseStmts(new ast::Node, tokens->begin(), tokens->end(), block);
            node->correctTree();
            return *node;
        }
    } // namespace compiler

} // namespace phi