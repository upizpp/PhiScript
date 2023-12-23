#pragma once
#include <phi/builtin/method.hpp>
#include <phi/exception.hpp>
#include <phi/object.hpp>

namespace phi
{
    class Module : public Object
    {
    private:
        std::map<std::string, Method> m_methods;

    protected:
        void addMethod(const std::string &name, const Method &method)
        {
            m_methods[name] = method;
        }

    public:
        bool has(const std::string &key) { return m_methods.find(key) != m_methods.end() || hasProperty(key); }
        Variant call(const std::string &name, const array &args) { return m_methods[name](args); }
        Variant access(const std::string &key)
        {
            if (m_methods.find(key) != m_methods.end())
            {
                Variant result{&m_methods[key]};
                result.setFlag(FLAG_METHOD);
                return result;
            }
            else if (hasProperty(key))
            {
                return getProperty(key);
            }
            throw RuntimeException("No such a property in the module.");
        }
    };
} // namespace phi
