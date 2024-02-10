#include <iomanip>
#include <iostream>
#include <phi/class_db.hpp>
#include <phi/compiler/compiler.hpp>
#include <phi/function.hpp>
#include <phi/runtime/builtin/global.hpp>
#include <phi/runtime/evaluator.hpp>
#include <phi/runtime/follower.hpp>

#ifdef _WIN32
#include <windows.h>
#endif

int main(int argc, char **args)
{
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
#endif

    using namespace phi;
    try
    {
        Compiler compiler(new FileScanner("D:\\User File\\Projects\\VSCode\\PhiScript\\src\\phi_script\\test.phi"));

        // compiler.parse()->print(), cout << endl;

        Function func = compiler.load();
        
        // func.getMethod().getState().print();

        array parsed_args{(size_t)argc, nullptr};
        Function print{ClassDB::toCallable([](RestParameters what)
                                           {
            for (auto &&item : what)
                cout << (string)*item << " ";
            cout << endl; })};
        Function id{ClassDB::toCallable([](Ref<Variant> value)
                                        { return new Variant{(integer)value.data()}; })};
        setGlobal("print", new Variant{print});
        setGlobal("id", new Variant{id});
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
    catch (const RuntimeException &e)
    {
        std::cerr << e.className() << " at line " << Singleton<ProgramFollower>::instance()->line() << ":\n";
        std::cerr << e.what() << endl;
        std::cerr << "Call Stack:" << endl;
        auto stack = Singleton<ProgramFollower>::instance()->getCallStack();
        while (!stack.empty())
        {
            std::cerr << "<- " << std::left << std::setw(32) << stack.top().func << " at line " << stack.top().line << endl;
            stack.pop();
        }
        Generator::clearInstance();
        abort();
    }
    catch (const Exception &e)
    {
        std::cerr << e.className() << ":\n";
        std::cerr << e.what() << endl;
        Generator::clearInstance();
        abort();
    }
end:
    puts("FINISH!");
}