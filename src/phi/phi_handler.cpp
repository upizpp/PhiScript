#include "phi_handler.hpp"
#include <iomanip>
#include <phi/compiler/compiler.hpp>
#include <phi/runtime/follower.hpp>
#include <phi/serialize.hpp>
#include <fstream>
#include <phi/version>
#include <phi/pout.hpp>

#ifdef _WIN32
#   define CLEAR "cls"
#else
#   define CLEAR "clear"
#endif

namespace phi
{
    void handleInput(const string &code)
    {
        if (code.empty())
            return;
        if (code == "exit")
            exit(EXIT_SUCCESS);
        if (code == "clear")
        {
            system(CLEAR);
            return;
        }
        static Environment env;
        tryRun([&]
               {
            try
            {
                Compiler compiler{new StringScanner{"return " + code}};
                auto pair = compiler.load().getMethod().call(env, {});
                cout << *pair.first << endl;
                env = std::move(pair.second);
            }
            catch (...)
            {
                Compiler compiler{new StringScanner{code}};
                auto pair = compiler.load().getMethod().call(env, {});
                cout << *pair.first << endl;
                env = std::move(pair.second);
            } });
    }

    void interact()
    {
        cout << std::left << std::setw(16) << "Phi " PHI_VERSION " " << std::right << std::setw(16) << __DATE__ " build" << endl;
        string input;
        cout << "> ";
        while (std::getline(cin, input))
        {
            handleInput(input);
            cout << "> ";
        }
    }
    Ref<Variant> doFile(const string &path, const array &args)
    {
        if (Pout::isPout(path))
            return Pout::load(path).call(args);
        Compiler compiler{new FileScanner{path}};
        return compiler.load().call(args);
    }
    void compileTo(Scanner *scanner, const string &dest)
    {
        Compiler compiler{scanner};
        Function func = compiler.load();
        vector<byte> bytes = Pout::dump(func);
        std::ofstream os(dest, std::ios::binary);
        os.write((const char*)bytes.data(), bytes.size());
    }
    void tryRun(const std::function<void()> what)
    {
        try
        {
            what();
        }
        catch (const std::exception &e)
        {
            std::cerr << e.what() << '\n';
            Generator::clearInstance();
            abort();
        }
        catch (const RuntimeException &e)
        {
            std::cerr << e.className() << " " << Singleton<ProgramFollower>::instance()->position() << endl;
            std::cerr << e.what() << endl;
            auto stack = Singleton<ProgramFollower>::instance()->getCallStack();
            if (!stack.empty())
            {
                std::cerr << "Call Stack:" << endl;
                while (!stack.empty())
                {
                    std::cerr << "<- " << stack.top() << endl;
                    stack.pop();
                }
            }
            Generator::clearInstance();
        }
        catch (const Exception &e)
        {
            std::cerr << e.className() << ":\n";
            std::cerr << e.what() << endl;
            Generator::clearInstance();
        }
    }
} // namespace phi