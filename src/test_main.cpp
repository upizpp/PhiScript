#include <iostream>
#include <phi/compiler/lexer.hpp>
#include <phi/compiler/preprocessor.hpp>

int main()
{
    using namespace phi;
    Lexer lexer(new FileScanner("src\\phi_script\\test.phi"));
    PreprocessRule rule("src\\phi_script\\test.pre");
    Preprocessor preprocessor(lexer.getTokens(), rule);
    for (auto token : preprocessor.getTokens())
    {
        std::cout << *token << std::endl;
    }
    puts("FINISH!");
}