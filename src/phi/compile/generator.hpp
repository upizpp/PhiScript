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
    index_t getTop() { return _M_result->getTop(); }
};
struct Generator {
    Generator(unique_ptr<ast::Expr> &node, const string &chunk)
        : _M_node(node), _M_process(new GenerateProcess), _M_chunk(chunk) {}

    unique_ptr<Program> generate() {
        generateWithoutReturn();
        unique_ptr<Program> res = std::move(_M_process->result());
        res->chunk() = _M_chunk;
        return res;
    }

  private:
    Generator(unique_ptr<ast::Expr> &node, shared_ptr<GenerateProcess> process)
        : _M_node(node), _M_process(process) {}
    void generate(unique_ptr<ast::Expr> &node, bool with_clear = true) {
        Generator gen(node, _M_process);
        gen.generateWithoutReturn(with_clear);
    }
    void generateWithoutReturn(bool with_clear = true);

    shared_ptr<GenerateProcess> _M_process;
    unique_ptr<ast::Expr> &_M_node;
    string _M_chunk;
};

} // namespace phi
