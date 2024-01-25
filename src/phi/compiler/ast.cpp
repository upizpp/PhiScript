#include "ast.hpp"

#define INIT string indent = string(level * 4, ' ');
#define OS cout << indent

namespace phi
{
    using namespace ast;

    void Node::print(uinteger level)
    {
        INIT;
        OS << "Node";
    }

    void Expr::print(uinteger level)
    {
        INIT;
    }

    void Sequence::print(uinteger level)
    {
        INIT;
        _M_current->print(level);
        OS << "\n";
        if (_M_next)
            _M_next->print(level);
    }

    void Block::print(uinteger level)
    {
        INIT;
        if (_M_seq)
        {
            OS << "{\n";
            _M_seq->print(level + 1);
        }
        else
            OS << "{";
        OS << "}";
    }

    void Unary::print(uinteger level)
    {
        INIT;
        OS << "Unary(" << *op() << ")\n";
        _M_operand->print(level + 1);
    }

    void Binary::print(uinteger level)
    {
        INIT;
        OS << "Binary(" << *op() << ")\n";
        _M_left->print(level + 1);
        OS << '\n';
        _M_right->print(level + 1);
    }

    void Load::print(uinteger level)
    {
        INIT;
        OS << (_M_var ? "Allocate" : "Load") << "(" << identifier() << ")";
    }

    void Constant::print(uinteger level)
    {
        INIT;
        OS << "Constant(" << *op() << ")";
    }

    void Eval::print(uinteger level)
    {
        INIT;
        OS << "Eval\n";
        _M_expr->print(level + 1);
    }

    void If::print(uinteger level)
    {
        INIT;
        OS << "if\ncond:\n";
        _M_condition->print(level);
        cout << "\nbody:\n";
        _M_body->print(level + 1);
    }

    void IfElse::print(uinteger level)
    {
        INIT;
        If::print(level);
        cout << '\n';
        OS << "else\n";
        _M_else->print(level + 1);
    }

    void While::print(uinteger level)
    {
        INIT;
        OS << "while\ncond:\n";
        _M_condition->print(level);
        cout << "\nbody:\n";
        _M_body->print(level + 1);
    }

    void WhileElse::print(uinteger level)
    {
        INIT;
        While::print(level);
        cout << '\n';
        OS << "else\n";
        _M_else->print(level + 1);
    }

    void phi::ast::For::print(uinteger level)
    {
        INIT;
        OS << "for\ninit:\n";
        _M_init->print(level);
        cout << "\ncond:\n";
        _M_condition->print(level);
        cout << "\nupdate:\n";
        _M_update->print(level);
        cout << "\nbody:\n";
        _M_body->print(level + 1);
    }

    void ForElse::print(uinteger level)
    {
        INIT;
        For::print(level);
        cout << '\n';
        OS << "else\n";
        _M_else->print(level + 1);
    }
} // namespace phi