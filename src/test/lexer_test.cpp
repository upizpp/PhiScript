
#include "test.hpp"
#include <compiler/lexer.hpp>
#include <path.hpp>

namespace phi {
namespace test {
struct LexerTest : Test {
    virtual void _run() override {
        {
            TESTSECTION("Lexer");
            Lexer lexer(new FileScanner(PATH "/src/test/lexer_test.txt"));

            ASSERT_FALSE(lexer.eof());
            while (!lexer.eof())
                printt(*lexer.next());
        }
    }
};
TEST(LexerTest);
} // namespace test
} // namespace phi