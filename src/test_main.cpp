#include <iostream>
#include <phi/compiler/lexer.hpp>
#include <phi/compiler/preprocessor.hpp>

int main()
{
    using namespace phi;
    Lexer lexer(new FileScanner("D:\\User File\\Projects\\VSCode\\PhiScript\\src\\phi_script\\test.phi"));
    Preprocessor preprocessor(lexer.getTokens());
    for (auto token : preprocessor.getTokens())
    {
        std::cout << *token << std::endl;
    }
    puts("FINISH!");
}