#include <iostream>

#include <phi/phi_entry.hpp>

#ifdef TEST
#include "test/compile_test.hpp"
typedef test::CompileTest Tester;
#endif

int main(int argc, char **args)
{
    phi::PhiInitialize();

#ifdef TEST
    Tester test;
    test.Run();
    phi::PhiExit();
#else
    return phi::PhiEntry(argc, args);
#endif
    return 0;
}