#pragma once
#include <fstream>
#include <sstream>
#include <string>

namespace phi
{
    namespace compiler
    {
        class Tokenizer;

        class CodeReader
        {
        protected:
            virtual std::istream *get_stream() = 0;

            Tokenizer *m_tokenizer = nullptr;
        public:
            CodeReader() {}
            virtual ~CodeReader() {}

            char get();
            char peek();
            void put(const char &);
            void unget();
            bool eof();
            void clear();
            void reset();

            virtual void setOwner(Tokenizer *tokenizer) { m_tokenizer = tokenizer; }

            CodeReader &operator>>(char &ch)
            {
                ch = get();
                return *this;
            }

            CodeReader &operator<<(const char &ch)
            {
                put(ch);
                return *this;
            }
        };

        class FileReader : public CodeReader
        {
        private:
            std::ifstream m_file;
            std::string m_filename;

            virtual std::istream *get_stream() override { return &m_file; }

        public:
            FileReader(const std::string &);

            void open(const std::string &);
            virtual void setOwner(Tokenizer *tokenizer) override;

            std::string &filename() { return m_filename; }
            const std::string &filename() const { return m_filename; }
        };

        class StringReader : public CodeReader
        {
        private:
            std::istringstream m_stream;

            virtual std::istream *get_stream() override { return &m_stream; }

        public:
            StringReader(const std::string &);

            virtual void setOwner(Tokenizer *tokenizer) override;
        };
    } // namespace compiler
} // namespace phi
