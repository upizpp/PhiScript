#pragma once
#include <phi/typedef.hpp>
#include <phi/variant.hpp>

namespace phi
{
    class Object
    {
    private:
        map<string, Ref<Variant>> _M_properties;
    public:
        Object() {}
        Object(const Object &) = default;

        virtual ~Object() {}

        static string className()
        {
            return "Object";
        }
        static string parentClassName()
        {
            return "";
        }
        virtual string getClass()
        {
            return "Object";
        }
        virtual string getParentClass()
        {
            return "";
        }

        static void static_register();

        void set(const string&, Ref<Variant>);
        Ref<Variant> get(const string&);

        // TODO Object::call
        Ref<Variant> call(const array &) { return Variant::Null; }

        VariantPacker access(const array &args);

        // TODO Object::convert
        Variant convert(Variant::Type) { return *Variant::Null; }

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

        // TODO: Object::calculate
        Variant operator+(const Variant &) const { return Variant(); }
        Variant operator-(const Variant &) const { return Variant(); }
        Variant operator*(const Variant &) const { return Variant(); }
        Variant operator/(const Variant &) const { return Variant(); }
        Variant operator%(const Variant &) const { return Variant(); }
        Variant operator&(const Variant &) const { return Variant(); }
        Variant operator|(const Variant &) const { return Variant(); }
        Variant operator^(const Variant &) const { return Variant(); }
        Variant operator&&(const Variant &) const { return Variant(); }
        Variant operator||(const Variant &) const { return Variant(); }
        Variant operator<<(const Variant &) const { return Variant(); }
        Variant operator>>(const Variant &) const { return Variant(); }
        Variant operator!() const { return Variant(); }
        Variant operator-() const { return Variant(); }
        Variant operator~() const { return Variant(); }
        Variant operator++() { return Variant(); }
        Variant operator--() { return Variant(); }

        // TODO Object::convert
        operator integer() { return 0; }
        operator real() { return 0.0; }
        operator bool() { return true; }
        operator string() { return string(); }
    };
} // namespace phi
