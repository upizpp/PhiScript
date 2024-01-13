#include <iostream>
#include <phi/variant.hpp>

int main()
{
    using namespace phi;
    Variant test{"Hello, world!"};
    std::cout << test;
}