#include "ast.hpp"
#include <typeinfo>

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
        if (!_M_current)
            return;
        _M_current->print(level);
        OS << "\n";
        if (_M_next)
            _M_next->print(level);
    }

    void Comma::print(uinteger level)
    {
        static bool title = true;
        INIT;
        if (!_M_current)
            return;
        bool flag = false;
        if (title)
        {
            OS << "Comma\n";
            title = false;
            flag = true;
        }
        _M_current->print(level + flag);
        if (_M_next)
        {
            OS << "\n";
            _M_next->print(level + flag);
        }
        if (flag)
            title = true;
    }

    void Call::print(uinteger level)
    {
        INIT;
        OS << "Call\n";
        _M_method->print(level + 1);
        if (_M_args)
        {
            cout << '\n';
            OS << "    Args:\n";
            _M_args->print(level + 2);
        }
    }

    void Access::print(uinteger level)
    {
        INIT;
        OS << "Access\n";
        _M_obj->print(level + 1);
        if (_M_args)
        {
            cout << '\n';
            OS << "    Args:\n";
            _M_args->print(level + 2);
        }
    }

    void Block::print(uinteger level)
    {
        INIT;
        if (_M_seq && !_M_seq->isNull())
        {
            OS << "{\n";
            _M_seq->print(level + 1);
            OS << "}";
        }
        else
            OS << "{}";
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
        OS << "if\n";
        _M_condition->print(level);
        cout << '\n';
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
        OS << "while\n";
        _M_condition->print(level);
        cout << '\n';
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

    void For::print(uinteger level)
    {
        INIT;
        OS << "for\n";
        _M_init->print(level);
        cout << '\n';
        _M_condition->print(level);
        cout << '\n';
        _M_update->print(level);
        cout << '\n';
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
    void Delete::print(uinteger level)
    {
        INIT;
        OS << "delete\n";
        _M_operand->print(level + 1);
    }

    void Import::print(uinteger level)
    {
        INIT;
        OS << "import " << module() << " as " << import();
    }
} // namespace phi
