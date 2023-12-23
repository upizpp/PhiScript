#pragma once
#include "else.hpp"
#include "loop_node.hpp"
#include <phi/owner.hpp>
#include <phi/runtime/expression.hpp>

namespace phi
{
    namespace compiler
    {
        namespace ast
        {
            class ForNode : public LoopNode
            {
            private:
                runtime::Expression m_entry;
                runtime::Expression m_condition;
                runtime::Expression m_handler;
                Owner<ElseNode> m_else_node = nullptr;

                virtual bool _run() override
                {
                    bool flag = true;
                    for (m_entry.evaluate(block()); *m_condition.evaluate(block()); m_handler.evaluate(block()))
                    {
                        flag = false;
                        runChildren([this]() -> bool {
                            if (this->broke())
                                return false;
                            if (this->continued())
                                return false;
                            return true;
                        });
                        if (this->continued())
                        {
                            reset();
                            continue;
                        }
                        if (broke())
                        {
                            reset();
                            break;
                        }
                    }
                    if (flag)
                        m_else_node->runChildren();

                    return false;
                }

            public:
                using LoopNode::LoopNode;

                void setEntry(runtime::Expression &&entry) { m_entry = std::move(entry); }
                void setCondition(runtime::Expression &&condition) { m_condition = std::move(condition); }
                void setHandler(runtime::Expression &&handler) { m_handler = std::move(handler); }
                void setElseNode(ElseNode *node) { m_else_node = node; }

                virtual void doBlock(const std::function<void(Ref<runtime::Block> &)> &func) override
                {
                    LoopNode::doBlock(func);
                    func(m_entry.block());
                    func(m_condition.block());
                    func(m_handler.block());

                    if (m_else_node)
                        m_else_node->doBlock(func);
                }

                virtual std::string nodeName() const override { return "ForNode"; }
            };
        } // namespace ast

    } // namespace compiler

} // namespace phi
