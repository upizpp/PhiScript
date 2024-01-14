#include <iostream>
#include <phi/variant.hpp>

int main()
{
    using namespace phi;
    Variant a{"Hello"};
    Variant b{"World"};
    std::cout << (a + " " + b);
}