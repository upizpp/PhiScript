#include "string.hpp"
#include "reflect_impl"
#include <phi/exception.hpp>

namespace phi {
    namespace modules {
        static TypeModuleRegister __{MODULE_NAME, Variant::Type::STRING,
                                     CLASS_NAME::instance};

        bool CLASS_NAME::is_empty(const string &self) { return self.empty(); }
        bool CLASS_NAME::is_number(const string &self) {
            if (self.empty())
                return false;
            bool point = false;
            for (size_t i = 0; i < self.length(); ++i)
                if (self[i] == '.')
                    if (!point)
                        point = true;
                    else
                        return false;
                else if (!std::isdigit(self[i]))
                    return false;
            return true;
        }
        bool CLASS_NAME::begins_with(const string &self, const string &what) {
            return self.substr(0, what.length()) == what;
        }
        bool CLASS_NAME::ends_with(const string &self, const string &what) {
            return self.substr(self.length() - what.length()) == what;
        }
        string CLASS_NAME::get_extension(const string &self) {
            size_t index = self.find_last_of('.');
            return index == string::npos ? "" : self.substr(index + 1);
        }
        string CLASS_NAME::get_basedir(const string &self) {
            size_t index = self.find_last_of('/');
            return index == string::npos ? "" : self.substr(0, index);
        }
        string CLASS_NAME::path_join(const string &self,
                                     const string &to_join) {}
        string CLASS_NAME::get_slice(const string &self,
                                     const string &delimiter, integer index) {
            if (index < 0)
                index = get_slice_count(self, delimiter) + index;
            size_t current = -delimiter.size();
            size_t temp;
            size_t n = 0;
            while (true) {
                temp = current + delimiter.size();
                current = self.find(delimiter, current + delimiter.size());
                if (current == string::npos)
                    throw RuntimeException(
                        "Slice not found while calling "
                        "StringLib::get_slice(\"%s\", \"%s\", %d).",
                        self.c_str(), delimiter.c_str(), index);
                if (n == index)
                    return self.substr(temp, current - temp);
                ++n;
            }
            return "";
        }
        integer CLASS_NAME::get_slice_count(const string &self,
                                            const string &delimiter) {
            size_t count = 0;
            size_t current = -delimiter.size();
            while (true) {
                current = self.find(delimiter, current + delimiter.size());
                if (current == string::npos)
                    break;
                ++count;
            }
            return count;
        }
        vector<string> CLASS_NAME::split(const string &self,
                                         const string &delimiter,
                                         Optional<bool> allow_empty) {
            OPT_DEFAULT(allow_empty, false);

            vector<string> result;
            size_t current = -delimiter.size();
            size_t temp;

            while (true) {
                temp = current + delimiter.size();
                current = self.find(delimiter, current + delimiter.size());
                string to_push;
                if (current == string::npos)
                    to_push = self.substr(temp);
                else
                    to_push = self.substr(temp, current - temp);
                if (!*allow_empty && to_push.empty())
                    continue;
                result.push_back(to_push);
                if (current == string::npos)
                    break;
            }

            return result;
        }
        string CLASS_NAME::ltrim(const string &self, OptionalRef<string> ws) {
            OPT_DEFAULT(ws, "\t\n\r\f\v ")

            string copy = self;
            copy.erase(0, copy.find_first_not_of(*ws));
            return copy;
        }
        string CLASS_NAME::rtrim(const string &self, OptionalRef<string> ws) {
            OPT_DEFAULT(ws, "\t\n\r\f\v ")

            string copy = self;
            copy.erase(copy.find_last_not_of(*ws) + 1);
            return copy;
        }
        string CLASS_NAME::trim(const string &self, OptionalRef<string> ws) {
            OPT_DEFAULT(ws, "\t\n\r\f\v ")

            string copy = self;
            copy.erase(copy.find_last_not_of(*ws) + 1);
            copy.erase(0, copy.find_first_not_of(*ws));
            return copy;
        }

        string CLASS_NAME::absolute(const string &relative,
                                    const string &self) {
            vector<string> units = split(relative, "/", false);
            string result = self;
            for (auto &&unit : units) {
                if (unit == "..") {
                    if (self.empty())
                        continue;
                    result = get_basedir(result);
                } else {
                }
            }
        }
    } // namespace modules
} // namespace phi
