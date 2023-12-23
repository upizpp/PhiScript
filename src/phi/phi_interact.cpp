#include "phi_interact.hpp"
#include <iostream>
#include <phi/compiler/compiler.hpp>
#include <phi/runtime/block.hpp>
#include <sstream>
#include <phi/phi_entry.hpp>
#include <phi/exception.hpp>

static std::string &trim(std::string &s)
{
    if (s.empty())
        return s;
    s.erase(0, s.find_first_not_of(" \t\n"));
    s.erase(s.find_last_not_of(" \t\n") + 1);
    return s;
}

namespace phi
{
    void interact(const std::string &path)
    {
        puts("PhiScript - Author: upizpp");
        puts("Compile Time: " __DATE__);
        Ref<runtime::Block> block = new runtime::Block();
        while (std::cin)
        {
            std::string code;
            std::cout << "> ";
            std::getline(std::cin, code);
            trim(code);

            using std::endl;
            std::ostringstream os;
            bool error = false;
            try
            {
                compiler::Compiler::compile(code, block.borrow()).run();
            }
            catch (const phi::Exception &exception)
            {
                os << "Program threw an phi::exception:" << endl;
                os << exception.getClassName() << ":"
                   << "\n"
                   << exception.what() << endl;
                os << "Position: " << PhiGetPosition() << endl;
                error = true;
            }
            catch (const std::exception &exception)
            {
                os << "Program threw an std::exception:" << endl;
                os << exception.what() << endl;
                os << "Position: " << PhiGetPosition() << endl;
                error = true;
            }
            if (error)
                std::cout << os.str();
            os.str("");
        }
    }
} // namespace phi
