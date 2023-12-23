#include "opcode_node.hpp"
#include "../exception.hpp"
#include "../runtime/expression.hpp"
#include "../runtime/expression_evaluator.hpp"
#include "../runtime/op_code.hpp"

namespace phi
{
    namespace compiler
    {
        OPCodeNode::OPCodeNode(runtime::Expression *expr)
        {
            if (expr->eof())
                throw SyntaxException("Insufficient operands.");
            m_code = &expr->last();
            const auto &info = runtime::ExpressionEvaluator::commandHandlers[m_code->command()];
            uinteger arg_count = info.first;
            switch (m_code->command())
            {
            case runtime::OPCode::Command::CALL:
            {
                //TODO Analyze CALL
                break;
            }
            case runtime::OPCode::Command::ACCESS:
            {
                uinteger element_count = integer(*expr->current().data());
                m_operands.push_back(new OPCodeNode(expr));
                for (uinteger i = 0; i < element_count; ++i)
                    m_operands.push_back(new OPCodeNode(expr));
                m_operands.push_back(new OPCodeNode(expr));
                break;
            }
            case runtime::OPCode::Command::MAKE_ARRAY:
            {
                uinteger element_count = integer(*expr->current().data());
                m_operands.push_back(new OPCodeNode(expr));
                for (uinteger i = 0; i < element_count; ++i)
                    m_operands.push_back(new OPCodeNode(expr));
                break;
            }
            case runtime::OPCode::Command::MAKE_DICTIONARY:
            {
                uinteger element_count = integer(*expr->current().data());
                m_operands.push_back(new OPCodeNode(expr));
                for (uinteger i = 0; i < element_count; ++i)
                {
                    m_operands.push_back(new OPCodeNode(expr));
                    m_operands.push_back(new OPCodeNode(expr));
                }
                break;
            }
            default:
            {
                m_operands.resize(arg_count);
                auto it = m_operands.begin();
                for (uinteger i = 0; i < arg_count; ++i)
                    *(it++) = new OPCodeNode(expr);
                break;
            }
            }
            m_operands.reverse();
        }

        void OPCodeNode::addOperand(Owner<OPCodeNode> &&node)
        {
            m_operands.push_back(std::move(node));
        }

        void OPCodeNode::analyze()
        {
            for (auto &&operand : m_operands)
            {
                operand->analyze();
            }
            // TODO OPCode::analyze
        }
    } // namespace compiler
} // namespace phi