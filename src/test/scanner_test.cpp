
#include "test.hpp"
#include <compiler/scanner.hpp>
#include <path.hpp>

namespace phi {
namespace test {
struct ScannerTest : Test {
    virtual void _run() override {
        {
            TESTSECTION("StringScanner");
            StringScanner scanner("Hello World!");
            LISTEN_BEGIN();
            while (!scanner.eof())
                printts(scanner.get());
            LISTEN_END("Hello World!");
            scanner.reset();
            ASSERT_EQUAL(scanner.peek(), 'H');
            ASSERT_EQUAL(scanner.get(), 'H');
            ASSERT_EQUAL(scanner.peek(), 'e');
            scanner.unget();
            ASSERT_EQUAL(scanner.peek(), 'H');
        }
        {
            TESTSECTION("FileScanner");
            FileScanner scanner(PATH "/src/test/letters.txt");
            LISTEN_BEGIN();
            while (!scanner.eof())
                printts(scanner.get());
            LISTEN_END("The Quick Brown Fox Jumps Over the Lazy Dog");
            scanner.reset();
            ASSERT_EQUAL(scanner.peek(), 'T');
            ASSERT_EQUAL(scanner.get(), 'T');
            ASSERT_EQUAL(scanner.peek(), 'h');
            scanner.unget();
            ASSERT_EQUAL(scanner.peek(), 'T');
        }
    }
};
TEST(ScannerTest);
} // namespace test
} // namespace phi