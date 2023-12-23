#pragma once
#include "node.hpp"
#include <sstream>

namespace phi
{
    namespace compiler
    {
        namespace ast
        {
            class LoopNode : public Node
            {
            private:
                bool m_broke = false;
                bool m_continued = false;
                std::string m_label;

                virtual bool _run() override
                {
                    return false;
                }

            public:
                virtual bool isLoopNode() const override { return true; }

                using Node::Node;
                virtual std::string nodeName() const override { return "LoopNode"; }

                void continueLoop() { m_continued = _TRUNCATE; }
                void breakLoop() { m_broke = true; }
                void reset()
                {
                    m_broke = false;
                    m_continued = false;
                }

                void label(const std::string &l) { m_label = l; }
                const std::string &label() const { return m_label; }
                std::string &label() { return m_label; }

                bool broke() const { return m_broke; }
                bool continued() const { return m_continued; }
            };
        } // namespace ast

    } // namespace compiler

} // namespace phi
