#include "node.hpp"
#include <algorithm>
#include <sstream>

namespace phi
{
    namespace compiler
    {
        namespace ast
        {
            Node::Node(const Node &node)
            {
                operator=(node);
            }

            Node &Node::operator=(const Node &node)
            {
                m_children = node.m_children;
                m_block = node.m_block;
                m_val = node.m_val;
                m_parent = node.m_parent;
                m_blocked = node.m_blocked;
                correctTree();
                return *this;
            }

            void Node::addChild(Ref<Node> node)
            {
                m_children.push_back(node);
                node->m_parent = this;
            }

            void Node::removeChild(Ref<Node> node)
            {
                m_children.erase(std::find(m_children.begin(), m_children.end(), node));
                node->m_parent = nullptr;
            }

            void Node::setParent(Node *node)
            {
                m_parent = node;
                if (block() != node->block() && block()->getParent() != node->block())
                {
                    doBlock([&node](Ref<runtime::Block> &block)
                            { if (block) block->setParent(node->block()); });
                }
            }

            Node *Node::getRoot()
            {
                Node *temp = m_parent;
                while (true)
                {
                    if (temp->m_parent)
                        temp = temp->m_parent;
                    else
                        return temp;
                }
            }

            void Node::correctTree()
            {
                for (auto &child : m_children)
                {
                    child->correctTree();
                    child->setParent(this);
                }
            }

            Variant Node::run()
            {
                correctTree();
                m_blocked = false;
                if (!_run())
                {
                    return *Variant::Null;
                }
                runChildren();

                if (m_block.isOwner())
                    m_block->deallocateVariables();

                if (!m_val.empty())
                {
                    Ref<Variant> value = m_val.top();
                    m_val.pop();
                    return *value;
                }
                return *Variant::Null;
            }

            void Node::setBlock(Ref<runtime::Block> block)
            {
                doBlock([&block](Ref<runtime::Block> &b)
                        {
                    if (b) b = block; });
                correctTree();
            }

            void Node::doBlock(const std::function<void(Ref<runtime::Block> &)> &func)
            {
                func(m_block);
            }

            void Node::blockTree()
            {
                m_blocked = true;
                for (auto &child : m_children)
                    child->blockTree();
            }

            std::string Node::stringify() const
            {
                std::ostringstream ss;
                ss << nodeName() << " - " << &block();
                return ss.str();
            }

            void Node::runChildren()
            {
                for (auto &child : m_children)
                {
                    child->run();
                    if (m_blocked)
                    {
                        m_blocked = false;
                        break;
                    }
                }
            }
            void Node::runChildren(const std::function<bool()> &handler)
            {
                for (auto &child : m_children)
                {
                    child->run();
                    if (m_blocked)
                    {
                        m_blocked = false;
                        break;
                    }
                    if (!handler())
                        break;
                }
            }
        } // namespace ast
    }     // namespace compiler
} // namespace phi
