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
        virtual string getClass() const
        {
            return "Object";
        }
        virtual string getParentClass() const
        {
            return "";
        }

        static void static_register();

        void set(const string &, Ref<Variant>);
        Ref<Variant> get(const string &);

        bool hasProperty(const string &) const;

        Ref<Variant> call(const array &);
        Ref<Variant> call(const string &, const array & = {});

        VariantPacker access(const array &args);

        Variant convert(Variant::Type);

        string toString();
        uinteger hash();

        bool operator==(const Object &obj) const
        {
            Variant temp{std::move(Owner<Object>{const_cast<Object*>(&obj)})};
            bool res = this->operator==(temp);
            temp.release();
            return res;
        }
        bool operator==(const Variant &) const;
        bool operator!=(const Object &obj) const
        {
            Variant temp{std::move(Owner<Object>{const_cast<Object*>(&obj)})};
            bool res = this->operator!=(temp);
            temp.release();
            return res;
        }
        bool operator!=(const Variant &) const;
        bool operator>(const Object &obj) const
        {
            Variant temp{std::move(Owner<Object>{const_cast<Object*>(&obj)})};
            bool res = this->operator>(temp);
            temp.release();
            return res;
        }
        bool operator>(const Variant &) const;
        bool operator>=(const Object &obj) const
        {
            Variant temp{std::move(Owner<Object>{const_cast<Object*>(&obj)})};
            bool res = this->operator>=(temp);
            temp.release();
            return res;
        }
        bool operator>=(const Variant &) const;
        bool operator<(const Object &obj) const
        {
            Variant temp{std::move(Owner<Object>{const_cast<Object*>(&obj)})};
            bool res = this->operator<(temp);
            temp.release();
            return res;
        }
        bool operator<(const Variant &) const;
        bool operator<=(const Object &obj) const
        {
            Variant temp{std::move(Owner<Object>{const_cast<Object*>(&obj)})};
            bool res = this->operator<=(temp);
            temp.release();
            return res;
        }
        bool operator<=(const Variant &) const;

        Variant operator+(const Variant &) const;
        Variant operator-(const Variant &) const;
        Variant operator*(const Variant &) const;
        Variant operator/(const Variant &) const;
        Variant operator%(const Variant &) const;
        Variant operator&(const Variant &) const;
        Variant operator|(const Variant &) const;
        Variant operator^(const Variant &) const;
        Variant operator&&(const Variant &) const;
        Variant operator||(const Variant &) const;
        Variant operator<<(const Variant &) const;
        Variant operator>>(const Variant &) const;
        Variant operator!() const;
        Variant operator-() const;
        Variant operator~() const;
        Variant operator++();
        Variant operator--();

        operator integer();
        operator real();
        operator bool();
        operator string();
    };
} // namespace phi
