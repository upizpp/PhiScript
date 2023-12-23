#include "io.hpp"
#include <iostream>
#include <phi/builtin/algorithms/string.hpp>
#include <phi/variant.hpp>

#include "reflect_impl"

namespace phi
{
    namespace builtin
    {
        Variant IOLib::print(array &args)
        {
            for (auto &&arg : args)
                std::cout << string(*arg);
            if (!args.empty())
                std::cout << std::endl;
            return *Variant::Null;
        }

        Variant IOLib::input(array &args)
        {
            for (auto &&arg : args)
                std::cout << string(*arg);
            string temp;
            std::getline(std::cin, temp);
            return Variant{temp};
        }

        Variant IOLib::get_number(array &args)
        {
            CHECK_LEAST_AND_MOST(2, 3);
            string input_text = *args[0];
            string tip_text = *args[1];
            std::vector<std::string> white_list;
            if (args.size() == 3)
            {
                array temp = *args[2];
                white_list.reserve(temp.size());
                for (auto &&arg : temp)
                    white_list.push_back(*arg);
            }
            string temp;
            std::cout << input_text;
            std::getline(std::cin, temp);
            while (!(algorithm::isNumber(temp) ||
                     std::find(white_list.begin(), white_list.end(), temp) != white_list.end()))
            {
                std::cout << tip_text;
                std::getline(std::cin, temp);
            }
            return std::find(white_list.begin(), white_list.end(), temp) != white_list.end() ? Variant{temp} : Variant{std::stod(temp)};
        }
    } // namespace builtin
} // namespace phi
