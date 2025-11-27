
#include "test.hpp"
#include <compiler/lexer.hpp>
#include <compiler/preprocessor.hpp>
#include <compiler/scanner.hpp>
#include <path.hpp>

namespace phi {
namespace test {
struct PreprocessorTest : Test {
    virtual void _run() override {
        {
            TESTSECTION("Lexer");
            Lexer lexer(
                new FileScanner(PATH "/src/test/preprocessor_test.txt"));
            Preprocessor preprocessor(lexer);

            ASSERT_FALSE(preprocessor.eof());
            while (!preprocessor.eof())
                printl(preprocessor.line, *preprocessor.next());
        }
    }
};
TEST(PreprocessorTest);
} // namespace test
} // namespace phi