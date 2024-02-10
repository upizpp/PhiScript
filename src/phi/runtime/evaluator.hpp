#pragma once
#include <phi/object.hpp>
#include <phi/runtime/state.hpp>
#include <stack>

namespace phi
{
    class Environment;
    using Env = Environment;

    class VariantPacker
    {
    public:
        using variable_t = Borrower<Ref<Variant>>;

    private:
        Ref<Variant> _M_data;
        variable_t _M_source;
        Ref<const string> _M_name;

    public:
        VariantPacker() = default;
        VariantPacker(const VariantPacker &) = default;
        VariantPacker(Ref<Variant> data) : _M_data(data) {}
        VariantPacker(variable_t source) : _M_source(source), _M_data(*source) {}
        VariantPacker(Ref<Variant> data, const string &name) : _M_data(data), _M_name(new string{name}) {}
        VariantPacker(variable_t source, const string &name) : _M_source(source), _M_data(*source), _M_name(new string(name)) {}
        // for ARGS
        explicit VariantPacker(bool) : _M_data(nullptr), _M_source(nullptr){};

        Variant &data() { return *_M_data; }
        const Variant &data() const { return *_M_data; }

        void name(const string &name) { _M_name = new string{name}; }
        const string &name() const { return *_M_name; }
        string nameSafely() const { return hasName() ? *_M_name : "Unknown"; }

        Ref<Variant> pointer() { return _M_data; }
        const Ref<Variant> pointer() const { return _M_data; }

        bool isArgs() const { return _M_source == nullptr && _M_data == nullptr; }
        bool isVariable() const { return _M_source != nullptr; }
        bool hasName() const { return _M_name != nullptr; }
        void redirectTo(Ref<Variant> variable)
        {
            *_M_source = variable;
        };

        bool isNull() const { return _M_data == nullptr; }

        void free(Env &);

        VariantPacker &assign(const VariantPacker &);
        VariantPacker &assign(const Ref<Variant> &);
        VariantPacker &operator=(const VariantPacker &other) { return assign(other); }
        VariantPacker &operator=(const Ref<Variant> &other) { return assign(other); }

        Variant *operator->() { return _M_data.data(); }
        const Variant *operator->() const { return _M_data.data(); }
        Variant &operator*() { return *_M_data; }
        const Variant &operator*() const { return *_M_data; }
    };

    class Environment
    {
    public:
        using value_stack = std::stack<VariantPacker>;

    private:
        value_stack _M_stack;
        map<string, Ref<Variant>> _M_locals;

    public:
        value_stack &stack() { return _M_stack; }
        const value_stack &stack() const { return _M_stack; }

        VariantPacker allocate(const string &);
        void setLocal(const string &, Ref<Variant>);
        VariantPacker load(const string &);
        bool has(const string &) const;

        void free(const string &);
    };

    class Evaluator
    {
    public:
    private:
        Borrower<const State> _M_state;
        list<Environment> _M_envs;

        uinteger _M_stream;

    public:
        Evaluator() = default;
        Evaluator(const State &state) : _M_state(&state) {}

        void setup(const State &);
        Ref<Variant> eval();

        inline Env &pushEnv()
        {
            _M_envs.push_back(Env());
            return _M_envs.back();
        }
        inline void popEnv()
        {
            _M_envs.pop_back();
        }

    private:
        Ref<Variant> handle(const OPCode &);

        inline void clear_stack()
        {
            Env::value_stack &stack = get_stack();
            while (!stack.empty())
                stack.pop();
        }

        inline void clear()
        {
            _M_envs.clear();
        }

        inline void stream_goto(uinteger where) { _M_stream = where - 1; }
        inline Env &get_env() { return _M_envs.back(); }
        inline Env::value_stack &get_stack() { return get_env().stack(); }
        inline VariantPacker push(const VariantPacker &value)
        {
            get_stack().push(value);
            return top();
        }
        inline VariantPacker &top() { return get_stack().top(); }
        inline VariantPacker pop()
        {
            VariantPacker tmp = top();
            get_stack().pop();
            return tmp;
        }
        inline VariantPacker allocate(const string &name) { return get_env().allocate(name); }
        VariantPacker load(const string &name, bool throws = true);
    };
} // namespace phi
