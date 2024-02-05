#include "evaluator.hpp"

namespace phi
{
    void Evaluator::setup(const State &state)
    {
        _M_state.reset(&state);
        try
        {
            eval();
        }
        catch (...)
        {
            _M_state.release();
            throw;
        }
        _M_state.release();
    }
    void Evaluator::eval()
    {
        const vector<OPCode> &codes = _M_state->getCodes();
        for (_M_stream = 0; _M_stream < codes.size(); ++_M_stream)
        {
            handle(codes[_M_stream]);
        }
    }
    void Evaluator::handle(const OPCode &code)
    {
        switch (code.opt())
        {
        case OPCode::Command::PUSH_VAL:
        {
            get_stack().push(_M_state->lookup(code.value()));
            break;
        }
        case OPCode::Command::POP_VAL:
        {
            get_stack().pop();
            break;
        }
        case OPCode::Command::PUSH_ENV:
        {
            _M_envs.push_back(value_stack{});
            break;
        }
        case OPCode::Command::POP_ENV:
        {
            _M_envs.pop_back();
            break;
        }
        case OPCode::Command::CLEAR:
        {
            value_stack &stack = get_stack();
            while (!stack.empty())
                stack.pop();
            break;
        }
        case OPCode::Command::GOTO:
        {
            stream_goto(code.value());
            break;
        }
        case OPCode::Command::IFTRUE:
        {
            if (*pop())
                stream_goto(code.value());
            break;
        }
        case OPCode::Command::IFFALSE:
        {
            if (!*pop())
                stream_goto(code.value());
            break;
        }
        default:
            break;
        }
    }
} // namespace phi