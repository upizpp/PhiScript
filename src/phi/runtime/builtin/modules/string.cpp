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

        array CLASS_NAME::split(const string &s, const string &delimiter)
        {
            array tokens;
            size_t start = 0;

            while ((start = s.find(delimiter)) != string::npos)
            {
                if (start > 0)
                    tokens.push_back(new Variant{s.substr(0, start)});
                start += delimiter.length();
                while (start < s.length() && s[start] == delimiter[0])
                    start += delimiter.length();
            }

            if (start < s.length())
                tokens.push_back(new Variant{s.substr(start)});

            return tokens;
        }

        string CLASS_NAME::absolute(const string &relative, const string &base)
        {
            return "";
        }
    } // namespace modules
} // namespace phi
