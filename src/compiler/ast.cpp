#include "ast.hpp"
#include <iostream>

namespace phi {
namespace ast {
void Node::print(int64_t level, std::string msg) {
    for (int64_t i = 0; i < level; ++i)
        std::cout << "  ";
    std::cout << msg;
}
void Sequence::print(int64_t level) {
    if (expr) {
        expr->print(level);
        if (next)
            std::cout << '\n';
    }
    if (next)
        next->print(level);
}
void Unary::print(int64_t level) {
    Node::print(level, "Unary: " + op->stringify() + "\n");
    expr->print(level + 1);
}
void Binary::print(int64_t level) {
    Node::print(level, "Binary: " + op->stringify() + "\n");
    lhs->print(level + 1);
    std::cout << '\n';
    rhs->print(level + 1);
}
void Constant::print(int64_t level) {
    Node::print(level, "Constant: " + value->stringify());
}
void Load::print(int64_t level) {
    if (allocate)
        Node::print(level, "Allocate: " + what->stringify());
    else
        Node::print(level, "Load: " + what->stringify());
}
void Block::print(int64_t level) {
    if (body) {
        Node::print(level, "{\n");
        body->print(level + 1);
        std::cout << '\n';
        Node::print(level, "}");
    } else
        Node::print(level, "{}");
}
} // namespace ast
} // namespace phi
