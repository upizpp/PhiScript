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
        Owner<vector<Ref<string>>> _M_binds;

    public:
        Method(const Method &method) : _M_method(method._M_method), _M_binds(method._M_binds ? new vector<Ref<string>>{*method._M_binds} : nullptr) {}
        Method(const Ref<State> &method) : _M_method(method) {}
        Method(const Ref<State> &method, Owner<vector<Ref<string>>> &&binds) : _M_method(method), _M_binds(std::move(binds)) {}

        Ref<Variant> call(const array &args);
        Ref<Variant> operator()(const array &args) { return call(args); }

        void bind(Owner<vector<Ref<string>>> &&binds) { _M_binds = std::move(binds); }
    };

    class Function
    {
    private:
        Ref<Method> _M_method;
        Ref<callable_t> _M_callable;

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
