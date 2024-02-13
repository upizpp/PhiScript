#include "phi_entry.hpp"
#include <cstdlib>
#include <phi/branch_parser.hpp>
#include <phi/phi_handler.hpp>
#include <phi/pout.hpp>

namespace phi
{
    int main(int argc, char **args)
    {
        ArgsParser parser;
        parser.addOption("o", ArgsParser::Type::SHORT);
        parser.parse(argc, args);

        vector<string> parsed_args = parser.getArgs();

        if (parsed_args.empty() && parser.hasNoExtraArgs())
        {
            interact();
        }
        else if (parser.hasNoExtraArgs())
        {
            string file = parsed_args.front();

            array converted_args{parsed_args.size(), nullptr};
            for (uinteger i = 0; i < parsed_args.size(); ++i)
                converted_args[i] = new Variant{parsed_args[i]};
            tryRun([&]()
                   { doFile(file, {new Variant{converted_args}}); });
            return EXIT_SUCCESS;
        }
        else if (!parsed_args.empty())
        {
            string output = parser.get("o");
            tryRun([&]()
                   { compileTo(new FileScanner{parsed_args.front()}, output); });
        }
        return EXIT_SUCCESS;
    }
} // namespace phi
