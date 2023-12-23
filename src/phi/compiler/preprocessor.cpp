#include "preprocessor.hpp"
#include <algorithm>

namespace phi
{
    namespace compiler
    {
        void Preprocessor::handleString(Token::Container *tokens)
        {
            Token::Container::iterator iterator = tokens->begin();
            while (true)
            {
                iterator = std::adjacent_find(iterator, tokens->end(), [](const Token &left, const Token &right) {
                    return left.type() == Token::Type::STRING && right.type() == Token::Type::STRING;
                });
                if (iterator == tokens->end())
                {
                    break;
                }
                Token::Container::iterator next = iterator;
                ++next;
                Token new_token{iterator->position(), iterator->content() + next->content(), Token::Type::STRING};
                tokens->insert(iterator, new_token);
                Token::Container::iterator temp = iterator;
                --temp;
                ++next;
                tokens->erase(iterator, next);
                iterator = temp;
            }
        }

        void Preprocessor::replaceToken(Token::Container *tokens, Token from, Token to)
        {
            Token::Container::iterator iterator;
            while ((iterator = std::find(tokens->begin(), tokens->end(), from)) != tokens->end())
            {
                *iterator = to;
            }
        }

    } // namespace compiler

} // namespace phi
