#pragma once
#include "../data_type.hpp"
#include "../owner.hpp"
#include <list>
#include <set>
#include <string>
#include <phi/runtime/op_code.hpp>

namespace phi
{
    namespace runtime
    {
        class Expression;
    }
    namespace compiler
    {
        class OPCodeNode
        {
        private:
            const runtime::OPCode *m_code;
            std::list<Owner<OPCodeNode>> m_operands;

        public:
            OPCodeNode(runtime::OPCode &code) : m_code(&code) {}
            OPCodeNode(runtime::Expression *);

            void addOperand(Owner<OPCodeNode> &&);
            const std::list<Owner<OPCodeNode>> &getChildren() const { return m_operands; }
            std::list<Owner<OPCodeNode>> &getChildren() { return m_operands; }
            std::string stringify() const { return m_code->stringify(); }

            void analyze();
        };
    } // namespace compiler
} // namespace phi
