// // The order is immutable!
#include "filesystem.hpp"
#include "reflect_impl"
#include <filesystem>
namespace fs = std::filesystem;

namespace phi
{
    namespace modules
    {
        bool CLASS_NAME::exists(const string &filename)
        {
            return fs::exists(filename);
        }
        bool CLASS_NAME::is_dir(const string &filename)
        {
            fs::file_status s = fs::status(filename);
            return fs::status_known(s) && s.type() == fs::file_type::directory;
        }
        bool CLASS_NAME::is_file(const string &filename)
        {
            fs::file_status s = fs::status(filename);
            return fs::status_known(s) && s.type() == fs::file_type::regular;
        }
        integer CLASS_NAME::type(const string &filename)
        {
            fs::file_status s = fs::status(filename);
            return (integer)s.type();
        }
    } // namespace modules
} // namespace phi
