#pragma once
#include <phi/runtime/state.hpp>
#include <stack>

namespace phi
{
    class Environment;
    using Env = Environment;

    class VariantPacker
    {
    public:
        using source_t = Borrower<std::pair<const string, Ref<Variant>>>;

    private:
        Ref<Variant> _M_data;
        source_t _M_source;

    public:
        VariantPacker(const VariantPacker &) = default;
        VariantPacker(const Variant &data) : _M_data(new Variant{data}) {}
        VariantPacker(Ref<Variant> data) : _M_data(data) {}
        VariantPacker(source_t source) : _M_source(source), _M_data(source->second) {}
        explicit VariantPacker(bool) : _M_data(nullptr), _M_source(nullptr){};

        Variant &data() { return *_M_data; }
        const Variant &data() const { return *_M_data; }

        const string &name() const { return _M_source->first; }

        Ref<Variant> pointer() { return _M_data; }
        const Ref<Variant> pointer() const { return _M_data; }

        bool isArgs() const { return _M_source == nullptr && _M_data == nullptr; }
        bool isVariable() const { return _M_source != nullptr; }
        void redirectTo(Ref<Variant> variable) { _M_source->second = variable; };

        void free(Env &);

        VariantPacker &assign(const VariantPacker &);
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
        VariantPacker load(const string &name);
    };
} // namespace phi
