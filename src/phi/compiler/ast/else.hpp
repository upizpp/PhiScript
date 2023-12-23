#pragma once
#include "node.hpp"
#include <sstream>

namespace phi
{
    namespace compiler
    {
        namespace ast
        {
            class ElseNode : public Node
            {
            private:
                virtual bool _run() override { return false; }

            public:
                virtual std::string nodeName() const override { return "ElseNode"; }
            };
        } // namespace ast

    } // namespace compiler

} // namespace phi
