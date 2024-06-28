#include <compile/lexer.hpp>
#include <compile/preprocessor.hpp>
#include <follower.hpp>
#include <iomanip>
#include <iostream>

int main() {
    std::ostringstream os;
    char c;
    while (std::cin.get(c))
        os << c;
    phi::string s = os.str();
    phi::StringScanner scanner(s);
    phi::Lexer lexer(scanner);
    phi::Preprocessor preprocessor(lexer);
    while (!preprocessor.eof()) {
        std::cout << std::left << std::setw(4)
                  << phi::ProgramFollower::get().line << *preprocessor.next()
                  << std::endl;
    }
    return EXIT_SUCCESS;
}