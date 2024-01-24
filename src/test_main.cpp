#include <iostream>
#include <phi/compiler/lexer.hpp>

int main()
{
    using namespace phi;
    Lexer lexer(new FileScanner("D:\\User File\\Projects\\VSCode\\PhiScript\\src\\phi_script\\test.phi"));
    while (!lexer.eof())
    {
        std::cout << *lexer.getNextToken() << std::endl;
    }
    puts("FINISH!");
}