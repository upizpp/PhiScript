#include "string.hpp"
#include <algorithm>

namespace phi
{
    namespace algorithm
    {
        bool isInteger(const std::string &str)
        {
            if (str.empty())
                return false;
            if (str.find_first_not_of("0123456789-") != std::string::npos)
                return false;
            return true;
        }
        bool isNumber(const std::string &str)
        {
            if (str.empty())
                return false;
            if (str.find_first_not_of("0123456789.-") != std::string::npos)
                return false;
            if (std::count(str.begin(), str.end(), '.') > 1)
                return false;
            return true;
        }
    } // namespace algorithm
} // namespace phi
