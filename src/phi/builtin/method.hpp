#pragma once
#include <functional>
#include <phi/object.hpp>
#include <phi/variant.hpp>

namespace phi
{
    class Method : public Object
    {
    public:
        using Function = std::function<Variant(array &)>;

    private:
        Function m_func;
        std::map<uinteger, Ref<Variant>> m_bind;

    public:
        Method() = default;
        Method(const Function &func) : m_func(func) {}
        Method(const Method &method) = default;
        Method(Method &&method) = default;

        Method &operator=(const Method &method) = default;
        Method &operator=(Method &&method) = default;

        void bind(uinteger index, const Ref<Variant> &value) { m_bind[index] = value; }

        virtual Variant call(array args) const
        {
            for (auto &&pair : m_bind)
            {
                args.insert(args.begin() + pair.first, pair.second);
            }
            
            return m_func(args);
        }

        Variant operator()(const array &args) const
        {
            return call(args);
        }
    };
} // namespace phi
