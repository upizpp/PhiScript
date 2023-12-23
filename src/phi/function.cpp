#include "function.hpp"
#include <algorithm>
#include <sstream>

namespace phi
{
    Variant Function::call(array &args)
    {
        using runtime::Block;
        //TODO function call
    }

    string Function::stringify() const
    {
        std::ostringstream ss;
        if (hasName())
            ss << name();
        ss << "(";
        bool first = true;
        for (auto &arg : m_args)
        {
            if (!first)
                ss << ", ";
            else
                first = false;
            ss << arg;
        }
        ss << ")";
        return ss.str();
    }
} // namespace phi