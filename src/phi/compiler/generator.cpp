#include "generator.hpp"

namespace phi
{
	Borrower<Generator> Generator::_M_current;

    Generator::~Generator()
    {
        if (_M_current == this)
            _M_current = nullptr;
    }

    void Generator::run_tasks()
    {
		while (!_M_tasks.empty())
		{
			_M_tasks.front()();
			_M_tasks.pop();
		}
	}
	Ref<State> Generator::gen(Ref<ast::Node> ast)
	{
		Borrower<Generator> temp = _M_current;
		_M_current = this;
		_M_state.reset(new State);
		_M_GCP.reset(new State::gcp_t);
		ast->gen();
		_M_state->setGCP(_M_GCP.release());
		run_tasks();
        if (temp)
		    _M_current = temp;
		return _M_state;
	}
	arg_t Generator::push(Ref<Variant> constant)
	{
		static arg_t index = 0;
		for (auto&& pair : *_M_GCP)
			if (pair.second->equals(*constant))
				return pair.first;
		_M_GCP->insert({ index, constant });
		return index++;
	}
	void Generator::addTask(const task_t& task)
	{
		_M_tasks.push(task);
	}
} // namespace phi