#pragma once
#include <phi/args_parser.hpp>
#include <phi/typedef.hpp>

namespace phi
{
    class BranchParser
    {
    public:
        using Branches = map<string, ArgsParser>;
        using SubBranches = map<string, BranchParser>;

    private:
        string _M_name;
        Branches _M_branches;
        SubBranches _M_sub_branches;

    public:
        BranchParser() = default;
        explicit BranchParser(const string &name,
                              const Branches &branches = {},
                              const SubBranches &sub_branches = {}) : _M_name(name), _M_branches(branches), _M_sub_branches(sub_branches) {}

        void addBranch(const ArgsParser &);
        void addBranch(const BranchParser &);

        void name(const string &name) { _M_name = name; }
        string &name() { return _M_name; }
        const string &name() const { return _M_name; }

        ArgsParser parse(int argc, char *args[]);
    };
} // namespace upi
