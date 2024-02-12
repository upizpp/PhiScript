#include "phi_entry.hpp"
#include <phi/branch_parser.hpp>
#include <cstdlib>
#include <phi/phi_handler.hpp>

namespace phi
{
    int main(int argc, char **args)
    {
        ArgsParser parser;
        parser.parse(argc, args);
        vector<string> parsed_args = parser.getArgs();
        
        if (parsed_args.empty())
        {
            interact();
        }
        else
        {
            string file = parsed_args.front();

            array converted_args{parsed_args.size(), nullptr};
            for (uinteger i = 0; i < parsed_args.size(); ++i)
                converted_args[i] = new Variant{parsed_args[i]};
            tryRun([&](){
                doFile(file, {new Variant{converted_args}});
            });
        }
        return EXIT_SUCCESS;
    }
} // namespace phi
