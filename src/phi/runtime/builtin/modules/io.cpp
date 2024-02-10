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

        string CLASS_NAME::input(string msg)
        {
            cout << msg;
            string tmp;
            std::getline(cin, tmp);
            return tmp;
        }

        integer CLASS_NAME::get_number(string msg)
        {
            return 0;
        }
    } // namespace modules

} // namespace phi
