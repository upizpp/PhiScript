#include "phi_handler.hpp"
#include <fstream>
#include <iomanip>
#include <phi/compiler/compiler.hpp>
#include <phi/pout.hpp>
#include <phi/runtime/follower.hpp>
#include <phi/serialize.hpp>
#include <phi/version>

#ifdef _WIN32
#define CLEAR "cls"
#else
#define CLEAR "clear"
#endif

namespace phi {
    void handleInput(const string &code) {
        if (code.empty())
            return;
        if (code == "exit")
            exit(EXIT_SUCCESS);
        if (code == "clear") {
            system(CLEAR);
            return;
        }
        static Environment env;
        tryRun([&] {
            try {
                Compiler compiler{new StringScanner{"return " + code}};
                auto pair = compiler.load().getMethod().call(env, {});
                cout << *pair.first << endl;
                env = std::move(pair.second);
            } catch (...) {
                Compiler compiler{new StringScanner{code}};
                auto pair = compiler.load().getMethod().call(env, {});
                cout << *pair.first << endl;
                env = std::move(pair.second);
            }
        });
    }

    void interact() {
        cout << std::left << std::setw(16) << "Phi " PHI_VERSION " "
             << std::right << std::setw(16) << __DATE__ " build" << endl;
        string input;
        cout << "> ";
        while (std::getline(cin, input)) {
            handleInput(input);
            cout << "> ";
        }
    }
    Ref<Variant> doFile(const string &path, const array &args,
                        const string &name) {
        if (Pout::isPout(path))
            return Pout::load(path).call(args);
        Compiler compiler{new FileScanner{path}};
        Function func = compiler.load();
        func.setProperty("__name__", new Variant{name});
        return func.call(args);
    }
    void compileTo(Scanner *scanner, const string &dest) {
        Compiler compiler{scanner};
        Function func = compiler.load();
        vector<byte> bytes = Pout::dump(func);
        std::ofstream os(dest, std::ios::binary);
        os.write((const char *)bytes.data(), bytes.size());
    }
    void tryRun(const std::function<void()> what) {
        try {
            what();
        } catch (const std::exception &e) {
            std::cerr << Singleton<ProgramFollower>::instance()->position()
                      << endl;
            std::cerr << e.what() << endl;
            auto stack = Singleton<ProgramFollower>::instance()->getCallStack();
            if (!stack.empty()) {
                std::cerr << "Call Stack:" << endl;
                while (!stack.empty()) {
                    std::cerr << "<- " << stack.top() << endl;
                    stack.pop();
                }
            }
        } catch (const CompileException &e) {
            std::cerr << e.className() << " "
                      << Singleton<ProgramFollower>::instance()->position()
                      << endl;
            std::cerr << e.what() << endl;
        } catch (const RuntimeException &e) {
            std::cerr << e.className() << " "
                      << Singleton<ProgramFollower>::instance()->position()
                      << endl;
            std::cerr << e.what() << endl;
            auto stack = Singleton<ProgramFollower>::instance()->getCallStack();
            if (!stack.empty()) {
                std::cerr << "Call Stack:" << endl;
                while (!stack.empty()) {
                    std::cerr << "<- " << stack.top() << endl;
                    stack.pop();
                }
            }
        } catch (const Exception &e) {
            std::cerr << e.className() << " "
                      << Singleton<ProgramFollower>::instance()->position()
                      << endl;
            std::cerr << e.what() << endl;
            auto stack = Singleton<ProgramFollower>::instance()->getCallStack();
            if (!stack.empty()) {
                std::cerr << "Call Stack:" << endl;
                while (!stack.empty()) {
                    std::cerr << "<- " << stack.top() << endl;
                    stack.pop();
                }
            }
        }
        Generator::clearInstance();
    }
} // namespace phi
