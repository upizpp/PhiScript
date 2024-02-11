#include <phi/phi_entry.hpp>

#ifdef _WIN32
#include <windows.h>
#endif

#include <iostream>

int main(int argc, char **args)
{
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
#endif
    return phi::main(argc, args);
}