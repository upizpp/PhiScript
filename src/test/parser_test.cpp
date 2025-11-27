#include "test.hpp"
#include <compiler/lexer.hpp>
#include <compiler/parser.hpp>
#include <compiler/preprocessor.hpp>
#include <path.hpp>

namespace phi {
namespace test {
struct ParserTest : Test {
    virtual void _run() override {
        {
            TESTSECTION("Parser");
            Lexer lexer(new FileScanner(PATH "/src/test/parser_test.txt"));
            Preprocessor preprocessor(lexer);
            Parser parser(preprocessor);
            Parser::node_t node = parser.parse();
            node->print();
            std::cout << '\n';
        }
    }
};
TEST(ParserTest);
} // namespace test
} // namespace phi