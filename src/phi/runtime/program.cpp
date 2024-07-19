#include "program.hpp"
#include <iomanip>
#include <stack>

namespace phi {
Program::ConstantReference Program::pushConstant(const Variable &variable) {
    _M_constants.push_back(variable);
    return {_M_constants, (index_t)(_M_constants.size() - 1)};
}
Program::OPCodeReference Program::pushCode(const OPCode &code, uint32_t line) {
    _M_codes.push_back(code);
    if (!_M_line_map.empty() && _M_line_map.back().line == line)
        ++_M_line_map.back().count;
    else
        _M_line_map.push_back(LineInfo{.line = line, .count = 1});
    return {_M_codes, (index_t)(_M_codes.size() - 1)};
}
index_t Program::emitLabel() {
    _M_labels.push_back(getTop());
    return _M_labels.size() - 1;
}

static bool is_from_constants(OPCode::Command command) {
    return command == OPCode::Command::LOAD_CONST ||
           command == OPCode::Command::LOAD ||
           command == OPCode::Command::ALLOCATE;
}
static bool is_from_labels(OPCode::Command command) {
    return command == OPCode::Command::GOTO ||
           command == OPCode::Command::IFFALSE ||
           command == OPCode::Command::IFTRUE ||
           command == OPCode::Command::FUNC_FLAG;
}

std::ostream &operator<<(std::ostream &os, const Program &program) {
    using namespace std;
    os << "Program:\n";
    os << "Codes: \n";
    auto line_iterator = program.line_map().begin();
    auto &codes = program.codes();
    uint32_t line_counter = 0;
    uint64_t i = 0;
    bool func_begun = false;
    std::stack<uint64_t> func_label;
    for (auto &&code_it = codes.begin(); code_it != codes.end();
         ++code_it, ++i) {
        cout << left << setw(6) << i;
        cout << left << setw(6) << line_iterator->line;
        auto next = code_it;
        ++next;
        if (!func_label.empty() && i == func_label.top()) {
            func_label.pop();
            func_begun = true;
        }
        if (code_it->command() == OPCode::Command::MAKE_FUNC)
            func_begun = false;

        if (!func_begun) {
            if (next != codes.end() && (is_from_constants(next->command()) ||
                                        is_from_labels(next->command()))) {
                if (next->command() == OPCode::Command::FUNC_FLAG)
                    func_label.push(
                        program.labels().at((index_t)code_it->command()));
                if (is_from_constants(next->command()))
                    cout << "CONST("
                         << program.constants().at((index_t)code_it->command())
                         << ")" << endl;
                else if (is_from_labels(next->command()))
                    cout << "LABEL("
                         << program.labels().at((index_t)code_it->command())
                         << ")" << endl;
            } else {
                cout << code_it->toString() << endl;
            }
        } else {
            cout << "CONST("
                 << program.constants().at((index_t)code_it->command()) << ")"
                 << endl;
        }

        ++line_counter;
        if (line_counter == line_iterator->count) {
            line_counter = 0;
            ++line_iterator;
        }
    }

    return os;
}
} // namespace phi
