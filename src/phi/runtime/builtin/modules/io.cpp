#include <phi/runtime/builtin/modules/string.hpp>
// // The order is immutable!
#include "io.hpp"
#include "reflect_impl"

namespace phi {
    namespace modules {
        void CLASS_NAME::print(RestParameters what) {
            for (auto &&item : what)
                cout << *item << " ";
            cout << endl;
        }

        string CLASS_NAME::input(const string &msg) {
            cout << msg;
            string tmp;
            std::getline(cin, tmp);
            return tmp;
        }

        real CLASS_NAME::get_number(const string &msg,
                                    OptionalRef<const string> error_msg,
                                    OptionalRef<const array> whitelist) {
            OPT_DEFAULT(error_msg, "Invalid number, please enter again: ");
            OPT_DEFAULT(whitelist, {});

            string tmp = input(msg);
            while (!StringLib::instance->is_number(tmp)) {
                tmp = input(*error_msg);
                for (auto &&ignore : *whitelist)
                    if (*ignore == tmp)
                        goto exit;
            }
        exit:
            return std::stold(tmp);
        }
    } // namespace modules

} // namespace phi
