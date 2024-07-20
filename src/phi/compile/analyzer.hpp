#pragma once
#include <compile/ast.hpp>

namespace phi {
struct Analyzer {
    Analyzer(unique_ptr<ast::Expr> &tree) : _M_tree(tree) {}

    void analyze();

  private:
    unique_ptr<ast::Expr> &_M_tree;
};

} // namespace phi
