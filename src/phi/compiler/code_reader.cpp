#include "code_reader.hpp"
#include "../exception.hpp"
#include "tokenizer.hpp"

namespace phi
{
    namespace compiler
    {
        char CodeReader::get()
        {
            char result = get_stream()->get();
            if ((result & 0xC0) != 0x80)
                m_tokenizer->nextColumn();
            if (result == '\n')
                m_tokenizer->nextLine();
            return result;
        }
        char CodeReader::peek()
        {
            return get_stream()->peek();
        }

        void CodeReader::put(const char &ch)
        {
            m_tokenizer->backward();
            get_stream()->putback(ch);
        }

        void CodeReader::unget()
        {
            m_tokenizer->backward();
            get_stream()->unget();
        }

        bool CodeReader::eof()
        {
            return get_stream()->eof();
        }

        void CodeReader::clear()
        {
            get_stream()->clear();
        }

        void CodeReader::reset()
        {
            get_stream()->seekg(std::ios_base::beg);
        }

        FileReader::FileReader(const std::string &filename)
        {
            open(filename);
        }

        void FileReader::open(const std::string &filename)
        {
            m_filename = filename;
            m_file.open(filename);
            if (!m_file.is_open())
                throw FileException(m_filename);
            if (m_tokenizer)
                m_tokenizer->position().filename = filename;
        }

        void FileReader::setOwner(Tokenizer *tokenizer)
        {
            m_tokenizer = tokenizer;
            m_tokenizer->position().filename = filename();
        }

        StringReader::StringReader(const std::string &s) : m_stream(s)
        {
            if (m_tokenizer)
                *m_tokenizer->position().filename = "chunk";
        }

        void StringReader::setOwner(Tokenizer *tokenizer)
        {
            m_tokenizer = tokenizer;
            char* buffer = new char[0xff];
            sprintf(buffer, "chunk: (%p)", this);
            m_tokenizer->position().filename = new std::string;
            *m_tokenizer->position().filename = buffer;
            delete[] buffer;
        }
    } // namespace compiler
} // namespace phi