#pragma once
#include "args_parser.hpp"
#include <phi/data_type.hpp>
#include <map>

namespace phi
{
    class BranchParser
    {
    public:
        using Branches = std::map<std::string, ArgsParser>;
        using SubBranches = std::map<std::string, BranchParser>;

    private:
        std::string m_name;
        Branches m_branches;
        SubBranches m_sub_branches;

    public:
        BranchParser() = default;
        explicit BranchParser(const std::string &name,
                              const Branches &branches = {},
                              const SubBranches &sub_branches = {}) : m_name(name), m_branches(branches), m_sub_branches(sub_branches) {}

        void addBranch(const ArgsParser &);
        void addBranch(const BranchParser &);

        void name(const std::string &name) { m_name = name; }
        std::string &name() { return m_name; }
        const std::string &name() const { return m_name; }

        ArgsParser parse(int argc, char *args[]);
    };
} // namespace upi
