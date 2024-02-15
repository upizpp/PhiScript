#include <phi/phi_handler.hpp>

#ifdef _WIN32
#include <windows.h>
#endif

int main(int argc, char **args) {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
#endif

    using namespace phi;
    tryRun([]() {
        doFile("D:\\User "
               "File\\Projects\\VSCode\\PhiScript\\src\\phi_script\\test.phi",
               {});
    });
    puts("Finished");
}