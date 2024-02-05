#include <iostream>
#include <phi/compiler/compiler.hpp>
#include <iomanip>

int main()
{
    using namespace phi;
    try
    {
        Compiler compiler(new FileScanner("D:\\User File\\Projects\\VSCode\\PhiScript\\src\\phi_script\\test.phi"));
        Ref<State> state = compiler.compile();
        puts("OPCodes:");
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
        Generator::clearInstance();
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
        Generator::clearInstance();
        abort();
    }
    catch (const Exception& e)
    {
        std::cerr << e.className() << ":\n";
        std::cerr << e.what() << '\n';
        Generator::clearInstance();
        abort();
    }
    end: puts("FINISH!");
}