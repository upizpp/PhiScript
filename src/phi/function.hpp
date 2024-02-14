#pragma once
#include <phi/class_db.hpp>
#include <phi/runtime/evaluator.hpp>
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
        Ref<Variant> _M_this;
		Ref<map<string, Ref<Variant>>> _M_exports;

    public:
        Method(const Method &method) = default;
        Method(const Ref<State> &method) : _M_method(method) {}
        Method(const Ref<State> &method, const Ref<vector<Ref<string>>> &&binds) : _M_method(method), _M_binds(binds) {}

        Ref<Variant> call(const array &args);
        std::pair<Ref<Variant>, Environment> call(const Environment &env, const array &args);
        Ref<Variant> operator()(const array &args) { return call(args); }

        State &getState() { return *_M_method; }
        const State &getState() const { return *_M_method; }

        void bind(const Ref<vector<Ref<string>>> &binds) { _M_binds = binds; }
        const vector<Ref<string>> &binds() const { return *_M_binds; }
        bool bound() const { return _M_binds != nullptr; }
        void setThis(Ref<Variant> new_this) { _M_this = new_this; }

		void exportValue(const string& name, Ref<Variant> value)
		{
			if (!_M_exports)
				_M_exports = new decltype(_M_exports)::value_t;
			(*_M_exports)[name] = value;
		}
    };

    class Function
    {
    private:
        Ref<Method> _M_method;
        Ref<callable_t> _M_callable;
        map<string, Ref<Variant>> _M_properties;
        map<uinteger, Ref<Variant>> _M_binds;

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

        callable_t &getCallable() { return *_M_callable; }
        const callable_t &getCallable() const { return *_M_callable; }
        Method &getMethod() { return *_M_method; }
        const Method &getMethod() const { return *_M_method; }
        void setThis(Ref<Variant> new_this)
        {
            if (isBuiltin())
                return;
            getMethod().setThis(new_this);
        }
        void bind(uinteger index, Ref<Variant> what) { _M_binds[index] = what; }
        void binds(const map<uinteger, Ref<Variant>> &binds) { _M_binds = binds; }
        const map<uinteger, Ref<Variant>> &binds() const { return _M_binds; }

        bool isBuiltin() const { return _M_callable; }

        VariantPacker access(const array &);
        bool hasProperty(const string &) const;
        void properties(const map<string, Ref<Variant>> &properties) { _M_properties = properties; }
        const map<string, Ref<Variant>> &properties() const { return _M_properties; }

        bool operator==(const Function &func) { return this == &func; }
        string toString()
        {
            std::ostringstream os;
            os << "function: " << this;
            return os.str();
        }
        operator string() { return toString(); }

        Function copy()
        {
            return *this;
        }
        Function deepCopy();
    };
} // namespace phi
