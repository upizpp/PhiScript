#include <iostream>
#include <phi/compiler/scanner.hpp>

int main()
{
    using namespace phi;
    puts("FileScanner:");
    {
        FileScanner scanner("./config.json");
        while (!scanner.eof())
            std::cout << scanner.get();
    }
    std::cout << std::endl;
    puts("StringScanner:");
    {
        StringScanner scanner("Hello world!");
        while (!scanner.eof())
            std::cout << scanner.get();
    }
}