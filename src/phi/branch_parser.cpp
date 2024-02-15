#include "branch_parser.hpp"
#include <iostream>
using string = std::string;

namespace phi {
    static void abort(const string &msg) {
        std::cerr << msg << std::endl;
        std::abort();
    }

    void BranchParser::addBranch(const ArgsParser &parser) {
        _M_branches[parser.name()] = parser;
    }

    void BranchParser::addBranch(const BranchParser &parser) {
        _M_sub_branches[parser.name()] = parser;
    }

    ArgsParser BranchParser::parse(int argc, char *args[]) {
        if (argc <= 1)
            return ArgsParser();
        std::string name = args[1];
        if (_M_branches.find(name) != _M_branches.end()) {
            _M_branches[name].parse(argc - 1, args + 1);
            return _M_branches[name];
        } else if (_M_sub_branches.find(name) != _M_sub_branches.end()) {
            return _M_sub_branches[name].parse(argc - 1, args + 1);
        } else {
            abort("Unknown branch: " + name + ".");
        }
        return ArgsParser();
    }

} // namespace phi
