#pragma once
#include <functional>
#include <list>
#include <phi/owner.hpp>
#include <phi/reference.hpp>
#include <phi/runtime/block.hpp>
#include <set>
#include <stack>

namespace phi
{
    namespace compiler
    {
        namespace ast
        {
            class Node
            {
            protected:
                std::list<Ref<Node>> m_children;
                Ref<runtime::Block> m_block;
                std::stack<Ref<Variant>> m_val; // for return
                Node *m_parent = nullptr;
                bool m_blocked = false;

            private:
                virtual bool _run() { return true; }

            public:
                Node() {}
                Node(const Ref<runtime::Block> &block) : m_block(block) {}
                Node(const Node &node);

                Node &operator=(const Node &node);

                void addChild(Ref<Node>);
                void removeChild(Ref<Node>);

                virtual bool isLoopNode() const { return false; }

                const std::list<Ref<Node>> &getChildren() const { return m_children; }
                std::list<Ref<Node>> &getChildren() { return m_children; }

                Node *getParent() { return m_parent; }
                void setParent(Node *node);
                Node *getRoot();

                void correctTree();

                Ref<runtime::Block> &block() { return m_block; }
                const Ref<runtime::Block> &block() const { return m_block; }
                void setBlock(Ref<runtime::Block>);
                virtual void doBlock(const std::function<void(Ref<runtime::Block> &)> &);

                void setValue(const Variant &val) { m_val.push(new Variant(val)); }
                void blockTree();

                Variant run();
                void runChildren();
                void runChildren(const std::function<bool()> &);
                virtual std::string stringify() const;
                virtual std::string nodeName() const { return "Node"; }

                bool operator==(const Node &node) { return this == &node; }
            };
        } // namespace ast
    }     // namespace compiler
} // namespace phi
