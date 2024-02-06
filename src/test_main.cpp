#include <iomanip>
#include <iostream>
#include <phi/compiler/compiler.hpp>
#include <phi/runtime/evaluator.hpp>

int main()
{
    using namespace phi;
    try
    {
        Compiler compiler(new FileScanner("D:\\User File\\Projects\\VSCode\\PhiScript\\src\\phi_script\\test.phi"));
        Ref<State> state = compiler.compile();
        puts("OPCodes:");
        auto &codes = state->getCodes();
        for (uinteger i = 0; i < codes.size(); i++)
        {
            cout << std::left << std::setw(8) << std::to_string(i) + ":" << codes[i];
            cout << "line:  " << state->line(i) << endl;
        }
        auto &labels = state->labels();
        if (!labels.empty())
        {
            puts("Labels:");
            for (auto &&pair : labels)
                cout << "L" << pair.first << ": " << pair.second << endl;
        }
        cout << endl;
        Evaluator evaluator(*state);
        Ref<Variant> res = evaluator.eval();
        if (res)
            cout << "result: " << *res << endl;
        Generator::clearInstance();
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
        Generator::clearInstance();
        abort();
    }
    catch (const Exception &e)
    {
        std::cerr << e.className() << ":\n";
        std::cerr << e.what() << '\n';
        Generator::clearInstance();
        abort();
    }
end:
    puts("FINISH!");
}