#pragma once
#include "variant.hpp"
#include <phi/phi_std.hpp>
#include <functional>
#include <map>

namespace phi
{
    class Object
    {
    private:
        std::function<integer(const Object *)> m_hasher;
        std::function<string(const Object *)> m_to_string;
        std::function<bool(const Object *)> m_to_bool;
        std::map<string, Variant> m_properties;

    public:
        integer hash() const;
        string toString() const;
        bool toBool() const;

        virtual Variant call(const array &args);
        Variant operator()(const array &args) { return call(args); }

        Object() = default;
        Object(const Object &) = default;
        Object(Object &&) = default;
        ~Object() = default;

        Variant &getProperty(const string &key) { return m_properties[key]; }
        const Variant &getProperty(const string &key) const { return m_properties.find(key)->second; }
        void setProperty(const string &key, Variant value) { m_properties[key] = std::move(value); }
        bool hasProperty(const string& key) const {return m_properties.find(key) != m_properties.end();}

        Ref<Variant> access(const array &args, Flag = FLAG_NONE);

        Variant &operator[](const string &key) { return *access({new Variant{key}}); }

        Object &operator=(const Object &);

        // TODO Object compare
        bool operator<(const Variant &) const { return false; }
        // TODO Object compare
        bool operator>(const Variant &) const { return false; }
        // TODO Object compare
        bool operator<=(const Variant &) const { return false; }
        // TODO Object compare
        bool operator>=(const Variant &) const { return false; }
        // TODO Object compare
        bool operator==(const Variant &) const { return false; }
        // TODO Object compare
        bool operator!=(const Variant &value) const { 
            if (value.type() == Variant::Type::NIL)
                return true;
            return false; 
        }
    };
} // namespace phi
