#pragma once
#include "loop_node.hpp"
#include <phi/exception.hpp>
#include <sstream>
#include <stack>

namespace phi
{
    namespace compiler
    {
        namespace ast
        {
            class ContinueNode : public Node
            {
            private:
                std::string m_label;

                virtual bool _run() override
                {
                    std::stack<LoopNode *> loop_stack;
                    bool found = false;
                    for(Node* temp = m_parent; temp; temp = temp->getParent())
                    {
                        if (LoopNode *loop_node = dynamic_cast<LoopNode *>(temp))
                        {
                            loop_stack.push(loop_node);
                            if (!m_label.empty() && loop_node->label() != m_label)
                                continue;
                            found = true;
                            break;
                        }
                    }
                    if (loop_stack.empty())
                        throw SyntaxException("Attempt to use continue statement outside of loop.");
                    if (!found)
                        throw SyntaxException("Attempt to continue of a non-existent loop.");
                    while (!loop_stack.empty())
                    {
                        loop_stack.top()->continueLoop();
                        loop_stack.pop();
                    }
                    return false;
                }

            public:
                using Node::Node;
                virtual std::string nodeName() const override { return "ContinueNode"; }
                virtual std::string stringify() const override
                {
                    std::ostringstream ss;
                    ss << Node::stringify() << '\t';
                    return ss.str();
                }

                void label(const std::string &l) { m_label = l; }
                const std::string &label() const { return m_label; }
                std::string &label() { return m_label; }
            };
        } // namespace ast

    } // namespace compiler

} // namespace phi
