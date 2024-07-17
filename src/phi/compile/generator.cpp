#include "generator.hpp"

namespace phi {
using namespace ast;
void Generator::generateWithoutReturn() {
    if (!_M_process)
        return;
    switch (_M_node->getType()) {
    case Expr::Type::LOAD: {
        auto &node = static_cast<Load &>(*_M_node);
        index_t index =
            _M_process->pushConstant(Variable(*node.identifier)).index();
        _M_process->pushCode(OPCode(OPCode::Command(index)), node.line);
        _M_process->pushCode(OPCode(node.withVar ? OPCode::Command::ALLOCATE
                                                 : OPCode::Command::LOAD),
                             node.line);
        break;
    }
    case Expr::Type::INTEGER: {
        auto &node = static_cast<IntegerExpr &>(*_M_node);
        index_t index = _M_process->pushConstant(Variable(node.value)).index();
        _M_process->pushCode(OPCode(OPCode::Command(index)), node.line);
        _M_process->pushCode(OPCode(OPCode::Command::LOAD_CONST), node.line);
        break;
    }
    case Expr::Type::REAL: {
        auto &node = static_cast<RealExpr &>(*_M_node);
        index_t index = _M_process->pushConstant(Variable(&node.value)).index();
        _M_process->pushCode(OPCode(OPCode::Command(index)), node.line);
        _M_process->pushCode(OPCode(OPCode::Command::LOAD_CONST), node.line);
        break;
    }
    case Expr::Type::BOOL: {
        auto &node = static_cast<RealExpr &>(*_M_node);
        index_t index = _M_process->pushConstant(Variable(&node.value)).index();
        _M_process->pushCode(OPCode(OPCode::Command(index)), node.line);
        _M_process->pushCode(OPCode(OPCode::Command::LOAD_CONST), node.line);
        break;
    }
    case Expr::Type::STRING: {
        auto &node = static_cast<StringExpr &>(*_M_node);
        index_t index = _M_process->pushConstant(Variable(&node.value)).index();
        _M_process->pushCode(OPCode(OPCode::Command(index)), node.line);
        _M_process->pushCode(OPCode(OPCode::Command::LOAD_CONST), node.line);
        break;
    }
    case Expr::Type::UNARY_EXPR: {
        auto &node = static_cast<UnaryExpr &>(*_M_node);
        generate(node.expr);
        _M_process->pushCode(OPCode(node.op->tag), node.line);
        break;
    }
    case Expr::Type::BINARY_EXPR: {
        auto &node = static_cast<BinaryExpr &>(*_M_node);
        generate(node.left);
        generate(node.right);
        _M_process->pushCode(OPCode(node.op->tag), node.line);
        break;
    }
    case Expr::Type::BLOCK: {
        auto &node = static_cast<Block &>(*_M_node);
        if (!node.body)
            break;
        _M_process->pushCode(OPCode(OPCode::Command::ENTER_BLOCK), node.line);
        generate(node.body);
        _M_process->pushCode(OPCode(OPCode::Command::EXIT_BLOCK), node.line);
        break;
    }
    case Expr::Type::SEQUENCE: {
        auto &node = static_cast<Sequence &>(*_M_node);
        if (node.current) {
            generate(node.current);
            _M_process->pushCode(OPCode(OPCode::Command::CLEAR), node.line);
        }
        if (node.next)
            generate(node.next);
        break;
    }
    case Expr::Type::IF: {
        auto &node = static_cast<If &>(*_M_node);
        // Likely
        if (!node.elseBody) {
            /*
                eval x
                ifFalse x goto L0
                body
                L0:
            */
            generate(node.condition);
            auto L0 = _M_process->pushCode(OPCode(), node.line);
            _M_process->pushCode(OPCode(OPCode::Command::IFFALSE), node.line);
            generate(node.body);
            L0.value().command() = (OPCode::Command)_M_process->emitLabel();
        } else {
            /*
                eval x
                ifFalse x goto L0
                body
                goto L1
                L0: else_body
                L1:
            */
            generate(node.condition);
            auto L0 = _M_process->pushCode(OPCode(), node.line);
            _M_process->pushCode(OPCode(OPCode::Command::IFFALSE), node.line);
            generate(node.body);
            auto L1 = _M_process->pushCode(OPCode(), node.line);
            _M_process->pushCode(OPCode(OPCode::Command::GOTO), node.line);
            L0.value() = (OPCode::Command)_M_process->emitLabel();
            generate(node.elseBody);
            L1.value() = (OPCode::Command)_M_process->emitLabel();
        }
        break;
    }
    case Expr::Type::WHILE: {
        auto &node = static_cast<While &>(*_M_node);
        if (!node.elseBody) {
            /*
                L0: eval x
                ifFalse x goto L1
                body
                goto L0
                L1:
            */
            auto L0_i = _M_process->emitLabel();
            generate(node.condition);
            auto L1 = _M_process->pushCode(OPCode(), node.line);
            _M_process->pushCode(OPCode(OPCode::Command::IFFALSE), node.line);
            generate(node.body);
            _M_process->pushCode(OPCode((OPCode::Command)L0_i), node.line);
            _M_process->pushCode(OPCode(OPCode::Command::GOTO), node.line);
            L1.value() = (OPCode::Command)_M_process->emitLabel();
        } else {
            /*
                   eval x
                   ifTrue x goto L1
                   else_body
                   goto L2
                   L0: eval x
                   ifFalse x goto L2
                   L1: body
                   goto L0
                   L2:
           */
            generate(node.condition);
            auto L1 = _M_process->pushCode(OPCode(), node.line);
            _M_process->pushCode(OPCode(OPCode::Command::IFTRUE), node.line);
            generate(node.elseBody);
            auto L2_1 = _M_process->pushCode(OPCode(), node.line);
            _M_process->pushCode(OPCode(OPCode::Command::GOTO), node.line);
            auto L0_i = _M_process->emitLabel();
            generate(node.condition);
            auto L2_2 = _M_process->pushCode(OPCode(), node.line);
            _M_process->pushCode(OPCode(OPCode::Command::IFFALSE), node.line);
            L1.value() = (OPCode::Command)_M_process->emitLabel();
            generate(node.body);
            _M_process->pushCode(OPCode((OPCode::Command)L0_i), node.line);
            _M_process->pushCode(OPCode(OPCode::Command::GOTO), node.line);
            L2_1.value() = (OPCode::Command)_M_process->emitLabel();
            L2_2.value() = L2_1.value();
        }
        break;
    }
    case Expr::Type::FOR: {
        auto &node = static_cast<For &>(*_M_node);
        if (!node.elseBody) {
            /*
                push_env
                init
                L0: eval test
                ifFalse test goto L1
                body
                update
                goto L0
                L1: pop_env
            */
            _M_process->pushCode(OPCode::Command::ENTER_BLOCK, node.line);
            if (node.initializer)
                generate(node.initializer);
            auto L0_i = _M_process->emitLabel();
            if (node.condition)
                generate(node.condition);
            auto L1 = _M_process->pushCode(OPCode(), node.line);
            _M_process->pushCode(OPCode(OPCode::Command::IFFALSE), node.line);
            generate(node.body);
            if (node.update)
                generate(node.update);
            _M_process->pushCode(OPCode((OPCode::Command)L0_i), node.line);
            _M_process->pushCode(OPCode(OPCode::Command::GOTO), node.line);
            L1.value() = (OPCode::Command)_M_process->emitLabel();
            _M_process->pushCode(OPCode::Command::EXIT_BLOCK, node.line);
        } else {
            /*
                push_env
                init
                eval test
                ifTrue test goto L0
                else_body
                goto L2
                L1: eval test
                ifFalse test goto L2
                L0: body
                update
                goto L1
                L2: pop_env
            */
            _M_process->pushCode(OPCode::Command::ENTER_BLOCK, node.line);
            if (node.initializer)
                generate(node.initializer);
            if (node.condition)
                generate(node.condition);
            auto L0 = _M_process->pushCode(OPCode(), node.line);
            _M_process->pushCode(OPCode(OPCode::Command::IFTRUE), node.line);
            generate(node.elseBody);
            auto L2_1 = _M_process->pushCode(OPCode(), node.line);
            _M_process->pushCode(OPCode(OPCode::Command::GOTO), node.line);
            auto L1_i = _M_process->emitLabel();
            if (node.condition)
                generate(node.condition);
            auto L2_2 = _M_process->pushCode(OPCode(), node.line);
            _M_process->pushCode(OPCode(OPCode::Command::IFFALSE), node.line);
            L0.value() = (OPCode::Command)_M_process->emitLabel();
            generate(node.body);
            if (node.update)
                generate(node.update);
            _M_process->pushCode(OPCode((OPCode::Command)L1_i), node.line);
            _M_process->pushCode(OPCode(OPCode::Command::GOTO), node.line);
            L2_1.value() = (OPCode::Command)_M_process->emitLabel();
            L2_2.value() = L2_1.value();
            _M_process->pushCode(OPCode::Command::EXIT_BLOCK, node.line);
        }
        break;
    }
    }
}
} // namespace phi
