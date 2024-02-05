#include <iostream>
#include <phi/compiler/lexer.hpp>
#include <phi/compiler/parser.hpp>
#include <phi/compiler/preprocessor.hpp>
#include <phi/compiler/generator.hpp>
#include <phi/compiler/optimizer.hpp>
#include <iomanip>

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
        if (!tree)
            goto end;
        puts("Abstract Syntax Tree:");
        tree->print();
        std::cout << std::endl << std::endl;
        Generator generator;
        Ref<State> state = generator.gen(tree);
        puts("OPCodes:");
        Optimizer::optimizeSimply(*state);
        auto& codes = state->getCodes();
        for (uinteger i = 0; i < codes.size(); i++)
            std::cout << std::left << std::setw(8) << std::to_string(i) + ":" << codes[i] << std::endl;
        auto& labels = state->labels();
        if (!labels.empty())
        {
            puts("Labels:");
            for (auto&& pair : labels)
                std::cout << "L" << pair.first << ": " << pair.second << std::endl;
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
        abort();
    }
    catch (const Exception& e)
    {
        std::cerr << e.className() << ":\n";
        std::cerr << e.what() << '\n';
        abort();
    }
    end: puts("FINISH!");
}