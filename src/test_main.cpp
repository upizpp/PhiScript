#include <iomanip>
#include <iostream>
#include <phi/compiler/compiler.hpp>
#include <phi/runtime/evaluator.hpp>
#include <phi/function.hpp>
#include <phi/class_db.hpp>
#include <phi/runtime/builtin/global.hpp>

int main(int argc, char** args)
{
    using namespace phi;
    try
    {
        Compiler compiler(new FileScanner("D:\\User File\\Projects\\VSCode\\PhiScript\\src\\phi_script\\test.phi"));

        // compiler.parse()->print();

        Function func = compiler.load();
        
        array parsed_args{(size_t)argc, nullptr};
        Function test{ClassDB::toCallable([](RestParameters what){
            for (auto &&item : what)
                cout << (string)*item << " ";
            cout << endl;
        })};
        setGlobal("print", new Variant{test});
        for (uinteger i = 0; i < argc; ++i)
            parsed_args[i] = new Variant{args[i]};
        
        Ref<Variant> res = func({new Variant(argc), new Variant(parsed_args)});
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