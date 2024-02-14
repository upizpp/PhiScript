#include "string.hpp"
#include "reflect_impl"
#include <phi/exception.hpp>

namespace phi
{
    namespace modules
    {
        static TypeModuleRegister __{MODULE_NAME, Variant::Type::STRING, CLASS_NAME::instance};

        bool CLASS_NAME::is_empty(const string &base)
        {
            return base.empty();
        }

        bool CLASS_NAME::is_number(const string &base)
        {
            if (base.empty())
                return false;
            bool point = false;
            for (size_t i = 0; i < base.length(); ++i)
                if (base[i] == '.')
                    if (!point)
                        point = true;
                    else
                        return false;
                else if (!std::isdigit(base[i]))
                    return false;
            return true;
        }

        string CLASS_NAME::get_slice(const string &base, const string &delimiter, integer index)
        {
            if (index < 0)
                index = get_slice_count(base, delimiter) + index;
            size_t current = -delimiter.size();
            size_t temp;
            size_t n = 0;
            while (true)
            {
                temp = current + delimiter.size();
                current = base.find(delimiter, current + delimiter.size());
                if (current == string::npos)
                    throw RuntimeException("Slice not found while calling StringLib::get_slice(\"%s\", \"%s\", %d).", base.c_str(), delimiter.c_str(), index);
                if (n == index)
                    return base.substr(temp, current - temp);
                ++n;
            }
            return "";
        }

        integer CLASS_NAME::get_slice_count(const string &base, const string &delimiter)
        {
            size_t count = 0;
            size_t current = -delimiter.size();
            while (true)
            {
                current = base.find(delimiter, current + delimiter.size());
                if (current == string::npos)
                    break;
                ++count;
            }
            return count;
        }

        vector<string> CLASS_NAME::split(const string &base, const string &delimiter, Optional<bool> allow_empty)
        {
            OPT_DEFAULT(allow_empty, false);

            vector<string> result;
            size_t current = -delimiter.size();
            size_t temp;

            while (true)
            {
                temp = current + delimiter.size();
                current = base.find(delimiter, current + delimiter.size());
                string to_push;
                if (current == string::npos)
                    to_push = base.substr(temp);
                else
                    to_push = base.substr(temp, current - temp);
                if (!*allow_empty && to_push.empty())
                    continue;
                result.push_back(to_push);
                if (current == string::npos)
                    break;
            }

            return result;
        }

        string CLASS_NAME::absolute(const string &relative, const string &base)
        {
            return "";
        }
    } // namespace modules
} // namespace phi
