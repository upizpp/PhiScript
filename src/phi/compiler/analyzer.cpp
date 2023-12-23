#include "analyzer.hpp"
#include "../exception.hpp"
#include "../runtime/expression.hpp"
#include "../phi_entry.hpp"
#include "opcode_node.hpp"
#include <algorithm>
#include <map>
#include <string>
#include <vector>
#include <set>

namespace phi
{
    namespace compiler
    {
        typedef std::vector<std::pair<std::string, std::string>> PairsContainer;
        static void CheckPairs(Token::Container *tokens, const PairsContainer &pairs)
        {
            std::map<std::string, std::pair<integer, Token *>> map;

            for (auto &&pair : pairs)
            {
                map[pair.first].first = 0;
            }
            for (auto &&token : *tokens)
            {
                for (auto &&pair : pairs)
                {
                    if (token.content() == pair.first)
                    {
                        if (map[pair.first].first == 0)
                        {
                            map[pair.first].second = &token;
                        }
                        ++map[pair.first].first;
                    }
                    else if (token.content() == pair.second)
                    {
                        --map[pair.first].first;
                    }
                }
            }
            for (auto &item : map)
            {
                if (item.second.first != 0)
                {
                    PhiSetPosition(item.second.second->position());
                    throw SyntaxException("Broken pair '%s'.", item.first.c_str());
                }
            }
        }
        inline static void CheckContinuity(Token::Container *tokens, const Token::Type &type)
        {
            auto it = std::adjacent_find(tokens->begin(), tokens->end(),
                [&type](const Token &left, const Token &right)
                {
                    return left.type() == type && right.type() == type;
                });
            if (it != tokens->end())
            {
                PhiSetPosition(it->position());
                throw SyntaxException("Cannot have two consecutive tokens of type %s.", Token::stringifyType(it->type()).c_str());
            }
        }
        inline static void CheckContinuity(Token::Container *tokens, const std::set<Token::Type> & ltypes, const std::set<Token::Type> &rtypes)
        {
            auto it = std::adjacent_find(tokens->begin(), tokens->end(),
                [&ltypes, rtypes](const Token &left, const Token &right)
                {
                    return ltypes.find(left.type()) != ltypes.end() && rtypes.find(right.type()) != rtypes.end();
                });
            if (it != tokens->end())
            {
                PhiSetPosition(it->position());
                throw SyntaxException("Unknown expression.", Token::stringifyType(it->type()).c_str());
            }
        }
        template <typename Pred>
        inline static void CheckContinuity(Token::Container *tokens, const Pred &pred, const std::string &msg)
        {
            auto it = std::adjacent_find(tokens->begin(), tokens->end(), pred);
            if (it != tokens->end())
            {
                PhiSetPosition(it->position());
                throw SyntaxException(msg);
            }
        }

        void StaticAnalyzer::analyze(Token::Container *tokens)
        {
            CheckPairs(tokens, PairsContainer{
                                   {"(", ")"},
                                   {"[", "]"},
                                   {"{", "}"}
                }
            );
            CheckContinuity(tokens, {
                Token::Type::NUMBER, Token::Type::STRING, Token::Type::BOOLEAN, Token::Type::IDENTIFIER
                }, {
                    Token::Type::NUMBER, Token::Type::STRING, Token::Type::BOOLEAN, Token::Type::IDENTIFIER
                }
            );
            CheckContinuity(
                tokens, [](const Token &left, const Token &right)
                { 
                    return (
                        (
                            left.type() == Token::Type::IDENTIFIER && right.type() != Token::Type::SYMBOL &&
                            right.type() != Token::Type::OPERATOR && right.type() != Token::Type::IDENTIFIER &&
                            right.type() != Token::Type::KEYWORD
                        ) ||
                        (
                            right.type() == Token::Type::IDENTIFIER && left.type() != Token::Type::SYMBOL &&
                            left.type() != Token::Type::OPERATOR && left.type() != Token::Type::IDENTIFIER &&
                            left.type() != Token::Type::KEYWORD
                        )
                        );
                },
                "Unknown identifier syntax.");
        }

        void StaticAnalyzer::analyze(runtime::Expression *expr)
        {
            expr->toBack();
            OPCodeNode node(expr);
            node.analyze();
            // std::cout << std::endl;
            // node.print();
            // std::cout << std::endl;
        }
    } // namespace compiler
} // namespace phi
