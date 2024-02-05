#pragma once
#include <functional>
#include <phi/compiler/ast.hpp>
#include <phi/runtime/state.hpp>
#include <queue>

namespace phi
{
    class Generator
    {
    public:
        using task_t = std::function<void()>;

    private:
        static Borrower<Generator> _M_current;

        Borrower<State> _M_state;
        Borrower<State::gcp_t> _M_GCP;
        std::queue<task_t> _M_tasks;

        void run_tasks();
    public:
        static Borrower<Generator> instance() { return _M_current; }

        ~Generator();

        Ref<State> gen(Ref<ast::Node>);
        State &getState() { return *_M_state; }
        arg_t push(Ref<Variant> constant);

        void addTask(const task_t&);
    };
} // namespace phi
