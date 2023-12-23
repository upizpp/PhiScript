#include <phi/compiler/code_reader.hpp>
#include <phi/compiler/tokenizer.hpp>
#include "test.hpp"
#include <phi/singleton.hpp>
#include <phi/logger.hpp>

#ifdef FC
#   define PATH "D:/User File/Projects/VSCode/PhiScript/src/test/phi/tokenize_test.phi"
#else
#   define PATH "./src/test/phi/tokenize_test.phi"
#endif

namespace test
{
    class TokenizerTest : public Test
    {
    private:
    public:
        virtual void _run()
        {
            using namespace phi;
            using namespace compiler;

            {
                GroupBegin("Tokenize String");

                Tokenizer tokenizer = Tokenizer(new StringReader{R"(123 456)"});
                while (!tokenizer.eof())
                {
                    std::cout << tokenizer.getNextToken() << std::endl;
                }

                GroupEnd();
            }

            {
                GroupBegin("Tokenize File");

                Tokenizer tokenizer = Tokenizer(new FileReader{PATH});
                // while (!tokenizer.eof())
                // {
                //     std::cout << tokenizer.getNextToken() << std::endl;
                // }
                for (auto &&token : tokenizer.getTokens())
                {
                    std::cout << token << std::endl;
                    LINFO(token.stringify());
                }

                GroupEnd();
            }
        }
    };

} // namespace test
