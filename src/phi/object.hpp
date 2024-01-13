#pragma once
#include <phi/typedef.hpp>

namespace phi
{
    class Variant;

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
        uinteger hash() { return uinteger(this); }

        //  TODO: Object::compare
        bool operator==(const Object &) const { return false; }
        bool operator==(const Variant &) const { return false; }
        bool operator!=(const Object &) const { return false; }
        bool operator!=(const Variant &) const { return false; }
        bool operator>(const Object &) const { return false; }
        bool operator>(const Variant &) const { return false; }
        bool operator>=(const Object &) const { return false; }
        bool operator>=(const Variant &) const { return false; }
        bool operator<(const Object &) const { return false; }
        bool operator<(const Variant &) const { return false; }
        bool operator<=(const Object &) const { return false; }
        bool operator<=(const Variant &) const { return false; }

        // TODO Object::convert
        operator integer() { return 0; }
        operator real() { return 0.0; }
        operator bool() { return true; }
        operator string() { return string(); }
    };
} // namespace phi
