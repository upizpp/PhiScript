#include "args_parser.hpp"
#include <iostream>
#include <stdexcept>
using namespace phi;
using string = std::string;

static void abort(const string &msg)
{
    std::cerr << msg << std::endl;
    std::abort();
}

void ArgsParser::addOption(const string &name, Importance importance, Type type)
{
    m_options[name] = Option{importance, type};
}

void ArgsParser::addOption(const string &name, Type type)
{
    addOption(name, Importance::OPTIONAL, type);
}

bool ArgsParser::hasOption(const string &name) const
{
    return m_options.find(name) != m_options.end();
}

void ArgsParser::show() const
{
    if (!m_name.empty())
        std::cout << m_name << ":" << std::endl;
    for (auto &&arg: m_values)
    {
        std::cout << arg.first << ": \"" << arg.second << "\"\n";
    }
    if (!m_values.empty())
        std::cout << std::endl;
    for (auto &&arg : m_arguments)
    {
        std::cout << arg << "\n";
    }
    
}

void ArgsParser::parse(int argc, char *args[])
{
    for (int i = 1; i < argc; i++)
    {
        std::string arg = args[i];

        if (arg[0] != '-')
        {
            m_arguments.push_back(arg);
            continue;
        }
        // long argument
        if (arg[1] == '-')
        {
            size_t index = arg.find("=");
            if (index == string::npos)
            {
                std::string name = arg.substr(2);
                if (!hasOption(name))
                {
                    abort("Unknown argument: '" + arg + "'.");
                }
                if (i + 1 >= argc || args[i + 1][0] == '-')
                {
                    if (m_options[name].importance == Importance::REQUIRED)
                        abort("Required argument value '" + name + "' is missing.");
                    m_values[name] = "";
                    continue;
                }
                std::string value = args[++i];
                m_values[name] = value;
            }
            else
            {
                std::string name = std::string(arg.begin() + 2, arg.begin() + index);
                std::string value = std::string(arg.begin() + index + 1, arg.end());
                if (!hasOption(name))
                {
                    abort("Unknown argument: '" + arg + "'.");
                }
                m_values[name] = value;
            }
        }
        // short argument
        else
        {
            // -a 10
            if (arg.length() == 2)
            {
                std::string name = std::string(1, arg[1]);
                if (!hasOption(name))
                {
                    abort("Unknown argument: '" + arg + "'.");
                }
                if (i + 1 >= argc || args[i + 1][0] == '-')
                {
                    if (m_options[name].importance == Importance::REQUIRED)
                        abort("Required argument value '" + name + "' is missing.");
                    m_values[name] = "";
                    continue;
                }
                std::string value = args[++i];
                m_values[name] = value;
                continue;
            }
            // -abcd
            else
            {
                for (uinteger j = 1; j < arg.length(); j++)
                {
                    std::string name = std::string(1, arg[j]);
                    if (!hasOption(name))
                    {
                        abort("Unknown argument: " + arg + ".");
                    }
                    if (m_options[name].importance != Importance::OPTIONAL)
                    {
                        abort("Required argument value '" + name + "' is missing.");
                    }
                    m_values[name] = "";
                }
            }
        }
    }
}
