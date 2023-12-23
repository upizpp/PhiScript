#include "branch_parser.hpp"
#include <iostream>
using namespace phi;
using string = std::string;

static void abort(const string &msg)
{
    std::cerr << msg << std::endl;
    std::abort();
}

void BranchParser::addBranch(const ArgsParser &parser)
{
    m_branches[parser.name()] = parser;
}

void BranchParser::addBranch(const BranchParser &parser)
{
    m_sub_branches[parser.name()] = parser;
}

ArgsParser BranchParser::parse(int argc, char *args[])
{
    if (argc <= 1)
        return ArgsParser();
    std::string name = args[1];
    if (m_branches.find(name) != m_branches.end())
    {
        m_branches[name].parse(argc - 1, args + 1);
        return m_branches[name];
    }
    else if (m_sub_branches.find(name) != m_sub_branches.end())
    {
        return m_sub_branches[name].parse(argc - 1, args + 1);
    }
    else
    {
        abort("Unknown branch: " + name + ".");
    }
    return ArgsParser();
}
