#include "io.hpp"

#include "reflect_impl"

namespace phi
{
    namespace modules
    {
        void IO::print(RestParameters what)
        {
            for (auto &&item : what)
                cout << (string)*item << " ";
            cout << endl;
        }

        string IO::input(string msg)
        {
            cout << msg;
            string tmp;
            std::getline(cin, tmp);
            return tmp;
        }
    } // namespace modules

} // namespace phi
