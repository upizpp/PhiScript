#pragma once
#include <phi/typedef.hpp>

namespace phi
{
    class Object
    {
    private:
    public:
        Object() {}
        Object(const Object &) = default;

        virtual ~Object() {}

        // TODO: Object::toString()
        string toString() { return string(); }

        // TODO: Object::hash()
        uinteger hash() { return this; }

        //  TODO: Object::operator==(const Object&)
        bool operator==(const Object &) { return false; }

        // TODO Object::convert
        operator integer() {return 0;}
        operator real() {return 0.0}
        operator bool() { return true; }
        operator string() { return string(); }
    };
} // namespace phi
