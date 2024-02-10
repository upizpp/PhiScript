#include <phi/runtime/builtin/modules/string.hpp>
// // The order is immutable!
#include "io.hpp"
#include "reflect_impl"

namespace phi
{
    namespace modules
    {
        void CLASS_NAME::print(RestParameters what)
        {
            for (auto &&item : what)
                cout << (string)*item << " ";
            cout << endl;
        }

        string CLASS_NAME::input(const string& msg)
        {
            cout << msg;
            string tmp;
            std::getline(cin, tmp);
            return tmp;
        }

        real CLASS_NAME::get_number(const string& msg, OptionalRef<const string> error_msg)
        {
            OPT_DEFAULT(error_msg, "Invalid number, please enter again: ");

            string tmp = input(msg);
            while (!StringLib::instance->is_number(tmp))
                tmp = input(*error_msg);
            return std::stold(tmp);
        }
    } // namespace modules

} // namespace phi
