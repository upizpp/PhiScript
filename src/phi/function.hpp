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
        vector<Ref<string>> _M_binds;

    public:
        Method(const Ref<State>& method, const vector<Ref<string>>& binds): _M_method(method), _M_binds(binds) {}

        Ref<Variant> call(array &args);
    };

    class Function
    {
    private:
        Ref<Method> _M_method;
        Ref<callable_t> _M_callable;

    public:
        Function() {}
        Function(const Function &) = default;
        ~Function() {}

        Ref<Variant> call(array &args);

        bool isBuiltin() { return _M_callable; }

        bool operator==(const Function &func) { return this == &func; }

        // TODO: Function::toString()
        string toString()
        {
            return string();
        }
        operator string() { return string(); }
    };
} // namespace phi
