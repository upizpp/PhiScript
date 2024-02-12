#include <iomanip>
#include <iostream>
#include <phi/serialize.hpp>
#include <phi/deserialize.hpp>
#include <phi/compiler/compiler.hpp>
#include <fstream>

#ifdef _WIN32
#include <windows.h>
#endif

int main(int argc, char **args)
{
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
#endif

    using namespace phi;
    Compiler compiler(new FileScanner{"src/phi_script/test.phi"});
    Function f = compiler.load();
    auto bytes = Serialize<Function>()(f);
    std::ofstream fs("./test.phiout", std::ios::binary);
    vector<byte> bytes_vec{bytes.begin(), bytes.end()};
    fs.write((char*)bytes_vec.data(), bytes_vec.size());
    puts("FINISH!");
}