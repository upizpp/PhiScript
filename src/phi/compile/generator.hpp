#pragma once
#include <compile/ast.hpp>
#include <runtime/program.hpp>

namespace phi {
struct GenerateProcess {
    PROPERTY(unique_ptr<Program>, result, make_unique<Program>())

    Program::ConstantReference pushConstant(const Variable &v) {
        return _M_result->pushConstant(v);
    }
    Program::OPCodeReference pushCode(const OPCode &op, uint32_t line) {
        return _M_result->pushCode(op, line);
    }
    index_t emitLabel() { return _M_result->emitLabel(); }
};
struct Generator {
    Generator(unique_ptr<ast::Expr> &node)
        : _M_node(node), _M_process(new GenerateProcess) {}

    unique_ptr<Program> generate() {
        generateWithoutReturn();
        return std::move(_M_process->result());
    }

  private:
    Generator(unique_ptr<ast::Expr> &node, shared_ptr<GenerateProcess> process)
        : _M_node(node), _M_process(process) {}
    void generate(unique_ptr<ast::Expr> &node) {
        Generator gen(node, _M_process);
        gen.generateWithoutReturn();
    }
    void generateWithoutReturn();

    shared_ptr<GenerateProcess> _M_process;
    unique_ptr<ast::Expr> &_M_node;
};

} // namespace phi
