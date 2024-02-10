#include "string.hpp"
#include "reflect_impl"

namespace phi
{
    namespace modules
    {
        static TypeModuleRegister __{MODULE_NAME, Variant::Type::STRING, CLASS_NAME::instance};
        
        bool CLASS_NAME::is_number(const string &str)
        {
            if (str.empty())
                return false;
            bool point = false;
            for (size_t i = 0; i < str.length(); ++i)
                if (str[i] == '.')
                    if (!point)
                        point = true;
                    else
                        return false;
                else if (!std::isdigit(str[i]))
                    return false;
            return true;
        }
    } // namespace modules
} // namespace phi
