#pragma once
#include "else.hpp"
#include "loop_node.hpp"
#include <phi/owner.hpp>
#include <phi/runtime/expression.hpp>
#include <sstream>

namespace phi
{
    namespace compiler
    {
        namespace ast
        {
            class WhileNode : public LoopNode
            {
            private:
                runtime::Expression m_condition;
                Owner<ElseNode> m_else_node = nullptr;

                virtual bool _run() override
                {
                    bool cond = *m_condition.evaluate(block());
                    if (cond)
                    {
                        while (cond)
                        {
                            runChildren([this]() -> bool
                                        {
                                if (this->broke())
                                    return false;
                                if (this->continued())
                                    return false;
                                return true; });
                            if (continued())
                            {
                                reset();
                            }
                            if (broke())
                            {
                                reset();
                                break;
                            }
                            cond = *m_condition.evaluate(block());
                        }
                    }
                    else if (m_else_node != nullptr)
                        m_else_node->runChildren();
                    return false;
                }

            public:
                using LoopNode::LoopNode;
                WhileNode(const runtime::Expression &expr) : m_condition(expr) {}

                void setCondition(runtime::Expression &&expr) { m_condition = std::move(expr); }
                void setElseNode(ElseNode *node) { m_else_node = node; }

                virtual std::string stringify() const override
                {
                    std::ostringstream ss;
                    ss << Node::stringify() << " - " << label() << '\t';
                    ss << m_condition.stringify();
                    return ss.str();
                }

                virtual void doBlock(const std::function<void(Ref<runtime::Block> &)> &func) override
                {
                    Node::doBlock(func);
                    func(m_condition.block());
                    if (m_else_node)
                        m_else_node->doBlock(func);
                }

                virtual std::string nodeName() const override { return "WhileNode"; }
            };
        } // namespace ast

    } // namespace compiler

} // namespace phi
