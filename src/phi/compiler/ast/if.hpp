#pragma once
#include "node.hpp"
#include <phi/owner.hpp>
#include <phi/runtime/expression.hpp>
#include <sstream>
#include "else.hpp"
#include <phi/owner.hpp>

namespace phi
{
    namespace compiler
    {
        namespace ast
        {

            class IfNode : public Node
            {
            private:
                runtime::Expression m_condition;
                Owner<ElseNode> m_else_node = nullptr;

                virtual bool _run() override
                {
                    bool cond = *m_condition.evaluate(block());
                    if (cond)
                        return true;
                    if (m_else_node != nullptr)
                        m_else_node->runChildren();
                    return false;
                }

            public:
                using Node::Node;
                IfNode(const runtime::Expression &expr) : m_condition(expr) {}

                void setCondition(runtime::Expression &&expr) { m_condition = std::move(expr); }
                void setElseNode(Owner<ElseNode> node) { m_else_node = std::move(node); }

                virtual std::string stringify() const override
                {
                    std::ostringstream ss;
                    ss << Node::stringify() << '\t';
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

                virtual std::string nodeName() const override { return "IfNode"; }
            };
        } // namespace ast

    } // namespace compiler

} // namespace phi
