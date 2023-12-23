#pragma once
#include "node.hpp"
#include <phi/runtime/expression.hpp>
#include <sstream>

namespace phi
{
    namespace compiler
    {
        namespace ast
        {
            class StatementNode : public Node
            {
            private:
                runtime::Expression m_expression;

                virtual bool _run() override
                {
                    m_expression.evaluate(block());
                    return true;
                }

            public:
                using Node::Node;
                StatementNode(const runtime::Expression &expr) : m_expression(expr) {}

                virtual std::string stringify() const override
                {
                    std::ostringstream ss;
                    ss << Node::stringify() << '\t';
                    ss << m_expression.stringify();
                    return ss.str();
                }

                virtual void doBlock(const std::function<void(Ref<runtime::Block> &)> &func) override
                {
                    Node::doBlock(func);
                    func(m_expression.block());
                }

                virtual std::string nodeName() const override { return "StatementNode"; }
            };
        } // namespace ast

    } // namespace compiler

} // namespace phi
