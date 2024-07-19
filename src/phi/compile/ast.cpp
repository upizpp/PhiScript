#include "ast.hpp"
#include <exception.hpp>
#include <follower.hpp>

#define INIT string WS(4 * level, ' ')
#define REINIT WS = string(4 * level, ' ')
#define WOUT cout << WS

namespace phi {
namespace ast {
Expr::Expr() : line(ProgramFollower::get().line) {}
void Expr::print(int16_t level) {
    INIT;
    WOUT << "Expr";
}
void IntegerExpr::print(int16_t level) {
    INIT;
    WOUT << "Constant: " << value;
}
void RealExpr::print(int16_t level) {
    INIT;
    WOUT << "Constant: " << value;
}
void StringExpr::print(int16_t level) {
    INIT;
    WOUT << "Constant: " << '"' << *value << '"';
}

void BoolExpr::print(int16_t level) {
    INIT;
    WOUT << "Constant: " << (value ? "true" : "false");
}
void Sequence::print(int16_t level) {
    if (current)
        current->print(level);
    if (next) {
        if (current)
            cout << endl;
        next->print(level);
    }
}
void Block::print(int16_t level) {
    INIT;
    WOUT << "Block: " << endl;
    if (body)
        body->print(level + 1);
}
void UnaryExpr::print(int16_t level) {
    INIT;
    WOUT << "Unary('" << op->toString() << "')" << endl;
    expr->print(level + 1);
}
void BinaryExpr::print(int16_t level) {
    INIT;
    WOUT << "Binary('" << op->toString() << "')" << endl;
    left->print(level + 1);
    cout << endl;
    right->print(level + 1);
}
void Load::print(int16_t level) {
    INIT;
    WOUT << (withVar ? "Allocate" : "Load");
    cout << ": " << *identifier;
}
void If::print(int16_t level) {
    INIT;
    WOUT << "If:" << endl;
    level += 1;
    REINIT;
    WOUT << "Condition:" << endl;
    condition->print(level + 1);
    cout << endl;
    WOUT << "Body:" << endl;
    body->print(level + 1);
    if (elseBody) {
        cout << endl;
        WOUT << "Else:" << endl;
        elseBody->print(level + 1);
    }
}
void While::print(int16_t level) {
    INIT;
    WOUT << "While: " << (tag ? *tag : "") << endl;
    level += 1;
    REINIT;
    WOUT << "Condition:" << endl;
    condition->print(level + 1);
    cout << endl;
    WOUT << "Body:" << endl;
    body->print(level + 1);
    if (elseBody) {
        cout << endl;
        WOUT << "Else:" << endl;
        elseBody->print(level + 1);
    }
}
void For::print(int16_t level) {
    INIT;
    WOUT << "For: " << (tag ? *tag : "") << endl;
    level += 1;
    REINIT;
    WOUT << "Initializer:" << endl;
    initializer->print(level + 1);
    cout << endl;
    WOUT << "Condition:" << endl;
    condition->print(level + 1);
    cout << endl;
    WOUT << "Update:" << endl;
    update->print(level + 1);
    cout << endl;
    WOUT << "Body:" << endl;
    body->print(level + 1);
    if (elseBody) {
        cout << endl;
        WOUT << "Else:" << endl;
        elseBody->print(level + 1);
    }
}
void Access::print(int16_t level) {
    INIT;
    WOUT << "Access: " << endl;
    level += 1;
    REINIT;
    WOUT << "Target: " << endl;
    target->print(level + 1);
    cout << endl;
    WOUT << "Args: " << endl;
    args->print(level + 1);
}
void Call::print(int16_t level) {
    INIT;
    WOUT << "Call: " << endl;
    level += 1;
    REINIT;
    WOUT << "Target: " << endl;
    target->print(level + 1);
    cout << endl;
    WOUT << "Args: " << endl;
    args->print(level + 1);
}
void Func::print(int16_t level) {
    INIT;
    WOUT << "Func: " << (name ? *name : "") << endl;
    level += 1;
    REINIT;
    if (!args.empty()) {
        WOUT << "Args:" << endl;
        for (auto &&arg : args)
            WOUT << "    " << *arg << endl;
    }
    WOUT << "Body:" << endl;
    if (body)
        body->print(level + 1);
    else
        WOUT << "    Empty";
}
void Return::print(int16_t level) {
    INIT;
    WOUT << "Return: " << endl;
    if (value)
        value->print(level + 1);
}
void Break::print(int16_t level) {
    INIT;
    WOUT << "Break " << (loop->tag ? *loop->tag : "");
}
void Continue::print(int16_t level) {
    INIT;
    WOUT << "Continue " << (loop->tag ? *loop->tag : "");
}
std::vector<native_ptr<Loop>> Loop::_M_loops;
void Loop::push(native_ptr<Loop> loop) { _M_loops.push_back(loop); }
void Loop::pop() { _M_loops.pop_back(); }
native_ptr<Loop> Loop::top() { return _M_loops.back(); }
native_ptr<Loop> Loop::find(shared_ptr<string> tag) {
    for (auto &&loop : _M_loops)
        if (*loop->tag == *tag)
            return loop;
    throw SyntaxException("Undefined loop tag");
}
} // namespace ast

} // namespace phi
