#pragma once
#include <phi/typedef.hpp>
#include <stack>

namespace phi
{
    struct ProgramPosition
    {
        string func;
        uinteger line;

        ProgramPosition(string f, uinteger l) : func(f), line(l) {}
    };

    class ProgramFollower
    {
    private:
        std::stack<ProgramPosition> _M_calling;
        uinteger _M_line;

    public:
        void callBegin(const ProgramPosition &position)
        {
            _M_calling.push(position);
        }
        void callEnd()
        {
            _M_calling.pop();
        }
        void line(uinteger line) { _M_line = line; }
        uinteger line() const { return _M_line; }
        decltype(_M_calling) getCallStack()
        {
            return _M_calling;
        }
    };
} // namespace phi
