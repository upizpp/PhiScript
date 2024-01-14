#include <iostream>
#include <phi/class_db.hpp>
#include <phi/object.hpp>
#include <phi/variant.hpp>

using namespace phi;

class Test : public phi::Object
{
    PhiClass(Test, Object) public : integer value = 0;
    string message = "???";

    Variant f()
    {
        puts("WORKING!");
        return Variant();
    }

    Variant add(int a, int b)
    {
        std::cout << a << " + " << b << " = " << a + b << std::endl;
        return Variant();
    }
};

void Test::static_register()
{
    METHOD_R(f, func);
    METHOD(add);
#undef CLASS
#define CLASS Test
    PROPERTY(value);
    PROPERTY(message);
}

int main()
{
    using namespace phi;
    Test obj;
    ClassDB::call(&obj, "func");
    ClassDB::call(&obj, "add", {Variant{4}, Variant{3}});
    std::cout << obj.value << std::endl;
    ClassDB::set(&obj, "value", Variant{10});
    std::cout << ClassDB::get(&obj, "value") << std::endl;
    std::cout << ClassDB::get(&obj, "message") << std::endl;
    ClassDB::set(&obj, "message", Variant{"Hello world!"});
    std::cout << ClassDB::get(&obj, "message") << std::endl;
    std::cout << obj.getClass() << std::endl;
}