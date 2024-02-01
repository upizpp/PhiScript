#include <iostream>
#include <phi/compiler/lexer.hpp>
#include <phi/compiler/parser.hpp>
#include <phi/compiler/preprocessor.hpp>

int main()
{
    using namespace phi;
    try
    {

        Lexer lexer(new FileScanner("D:\\User File\\Projects\\VSCode\\PhiScript\\src\\phi_script\\test.phi"));
        Preprocessor preprocessor(lexer.getTokens());
        Parser parser;
        auto& tokens = preprocessor.getTokens();
        auto tree = parser.parse(tokens);
        if (tree)
            tree->print();
        std::cout << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
        throw;
    }
    catch (const Exception& e)
    {
        std::cerr << e.className() << ":\n";
        std::cerr << e.what() << '\n';
        throw;
    }
    puts("FINISH!");
}