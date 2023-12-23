#pragma once
#include "node.hpp"
#include <sstream>
#include <phi/runtime/expression.hpp>

namespace phi
{
    namespace compiler
    {
        namespace ast
        {
            class ReturnNode : public Node
            {
            private:
                runtime::Expression m_expr;

                virtual bool _run() override
                {
                    Node* root = getRoot();
                    root->setValue(*m_expr.evaluate(block()));
                    root->blockTree();
                    return false;
                }

            public:
                using Node::Node;
                virtual std::string nodeName() const override { return "ReturnNode"; }
                virtual std::string stringify() const override
                {
                    std::ostringstream ss;
                    ss << Node::stringify() << '\t' << m_expr.stringify();
                    return ss.str();
                }

                virtual void doBlock(const std::function<void(Ref<runtime::Block> &)> &func) override
                {
                    Node::doBlock(func);
                    func(m_expr.block());
                }

                void setExpression(const runtime::Expression& expr) { m_expr = expr;}
            };
        } // namespace ast

    } // namespace compiler

} // namespace phi
