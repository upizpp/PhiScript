#pragma once
#include <phi/class_db.hpp>
#include <phi/runtime/state.hpp>
#include <phi/typedef.hpp>

namespace phi
{
    class Method
    {
    private:
        Ref<State> _M_method;
        Ref<vector<Ref<string>>> _M_binds;
        // closure
        Ref<map<Ref<string>, Ref<Variant>>> _M_links;
        Ref<Variant> _M_this;

    public:
        Method(const Method &method) = default;
        Method(const Ref<State> &method) : _M_method(method) {}
        Method(const Ref<State> &method, const Ref<vector<Ref<string>>> &&binds) : _M_method(method), _M_binds(binds) {}

        Ref<Variant> call(const array &args);
        Ref<Variant> operator()(const array &args) { return call(args); }

        void bind(const Ref<vector<Ref<string>>> &binds) { _M_binds = binds; }
        void setThis(Ref<Variant> new_this) { _M_this = new_this; }
    };

    class Function
    {
    private:
        Ref<Method> _M_method;
        Ref<callable_t> _M_callable;
        map<string, Ref<Variant>> _M_properties;

    public:
        Function() {}
        Function(const callable_t &callable) : _M_callable(new callable_t{callable}) {}
        Function(const Ref<callable_t> &callable) : _M_callable(callable) {}
        Function(const Method &method) : _M_method(new Method{method}) {}
        Function(const Ref<Method> &method) : _M_method(method) {}
        Function(const Function &) = default;
        ~Function() {}

        Ref<Variant> call(const array &args = {});
        Ref<Variant> operator()(const array &args = {}) { return call(args); }

        Method &getMethod() { return *_M_method; }

        bool isBuiltin() { return _M_callable; }

        Ref<Variant>& access(const array &);

        bool operator==(const Function &func) { return this == &func; }

        string toString()
        {
            std::ostringstream os;
            os << "function: " << this;
            return os.str();
        }
        operator string() { return toString(); }
    };
} // namespace phi
