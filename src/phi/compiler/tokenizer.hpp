#pragma once
#include "token.hpp"
#include <list>

namespace phi
{
    namespace compiler
    {
        class CodeReader;

        class Tokenizer
        {
        private:
            CodeReader *m_reader = nullptr;

            Position m_position;
            // the end of last line.
            integer m_eol = 1;

        public:
            Tokenizer(){};
            Tokenizer(CodeReader *reader);
            ~Tokenizer();

            Tokenizer &operator=(const Tokenizer &) = delete;

            void nextColumn();
            void nextLine();
            void backward();

            bool eof();

        protected:
            void skipWhitespace();

        public:
            Token getNextToken();
            Tokenizer &operator<<(Token &token)
            {
                token = getNextToken();
                return *this;
            }
            std::list<Token> getTokens();

            Position &position() { return m_position; }
            const Position &position() const { return m_position; }
        };
    } // namespace compiler
} // namespace phi
