#include <phi/compiler/analyzer.hpp>
#include <phi/compiler/code_reader.hpp>
#include <phi/compiler/preprocessor.hpp>
#include <phi/compiler/tokenizer.hpp>
#include <phi/compiler/parser.hpp>
#include <phi/tree.hpp>
#include "test.hpp"

#ifdef DEBUG
#define PATH "D:/User File/Projects/VSCode/PhiScript/src/test/phi/compile_test.phi"
#else
#define PATH "./src/test/phi/compile_test.phi"
#endif

namespace test
{
    class CompileTest : public Test
    {
    private:
    public:
        virtual void _run()
        {
            using namespace phi;
            using namespace compiler;

            {
                GroupBegin("Compile");

                {
                    GroupBegin("Tokenize File");

                    Tokenizer tokenizer = Tokenizer(new FileReader{PATH});
                    Token::Container tokens = tokenizer.getTokens();

                    Preprocessor::handleString(&tokens);

                    for (auto &&token : tokens)
                    {
                        std::cout << token << std::endl;
                        LINFO(token.stringify());
                    }

                    puts("-----------------------");
                    
                    StaticAnalyzer::analyze(&tokens);

                    puts("-----------------------");

                    ast::Node node = Parser::parseStmts(&tokens);
                    print(node);
                    node.run();

                    GroupEnd();
                }

                GroupEnd();
            }
        }
    };

} // namespace test
