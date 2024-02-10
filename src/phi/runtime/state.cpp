#include "state.hpp"

namespace phi
{
    Borrower<const State> State::printInstance;
    Borrower<State::gcp_t> State::_M_globalPool;

    void State::print() const
    {
        printInstance = this;
        if (!_M_codes.empty())
        {
            cout << "Codes:\n";
            for (uinteger i = 0; i < _M_codes.size(); ++i)
                cout << _M_codes[i] << " at line " << _M_lines[i] << '\n';
        }
        if (!_M_labels.empty())
        {
            cout << "Labels:\n";
            for (auto &&label : _M_labels)
                cout << "L" << label.first << ": " << label.second << '\n';
        }
        cout << endl;
    }
} // namespace phi
