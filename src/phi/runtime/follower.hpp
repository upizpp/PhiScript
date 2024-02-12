#pragma once
#include <phi/typedef.hpp>
#include <stack>

namespace phi
{
    struct ProgramPosition
    {
        string func;
        uinteger line;
        Ref<string> chunk;

        ProgramPosition() = default;
        ProgramPosition(const ProgramPosition&) = default;
        ProgramPosition(string f, uinteger l, const Ref<string> &c) : func(f), line(l), chunk(c) {}

        string toString() const
        {
            return "in \"" + *chunk + "\" at line " + std::to_string(line) + (!func.empty() ? (":" + func) : "");
        }
    };

    inline std::ostream& operator<<(std::ostream& os, const ProgramPosition &position)
    {
        return os << position.toString();
    }

    class ProgramFollower
    {
    private:
        std::stack<ProgramPosition> _M_calling;
        ProgramPosition _M_current;

    public:
        void callBegin(const ProgramPosition &position)
        {
            _M_calling.push(position);
        }
        void callEnd()
        {
            _M_calling.pop();
        }
        const ProgramPosition& position() const
        {
            return _M_current;
        }
        void position(const ProgramPosition &position)
        {
            _M_current = position;
        }
        decltype(_M_calling) getCallStack()
        {
            return _M_calling;
        }
    };
} // namespace phi
