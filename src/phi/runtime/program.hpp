#pragma once
#include <map>
#include <ostream>
#include <runtime/opcode.hpp>
#include <typedef.hpp>
#include <variable.hpp>
#include <vector>

namespace phi {
struct Program {
    using ConstantReference =
        ContainerReference<Variable, std::vector<Variable>, index_t>;
    using OPCodeReference =
        ContainerReference<OPCode, std::vector<OPCode>, index_t>;

    ConstantReference pushConstant(const Variable &);
    OPCodeReference pushCode(const OPCode &, uint32_t line);
    index_t emitLabel();
    index_t getTop() { return _M_codes.size(); }

    struct LineInfo {
        uint32_t line;
        uint32_t count;
    };

  private:
    PROPERTY(std::vector<OPCode>, codes, {})
    PROPERTY(std::vector<uint64_t>, labels, {})
    PROPERTY(std::vector<LineInfo>, line_map, {})
    PROPERTY(std::vector<Variable>, constants, {})
};

std::ostream &operator<<(std::ostream &, const Program &);

} // namespace phi
