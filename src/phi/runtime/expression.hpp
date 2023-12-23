#pragma once
#include "typedef.hpp"

namespace phi
{
    namespace runtime
    {
        class ExpressionEvaluator;

        class Expression
        {
        private:
            OPCodeContainer m_opcodes;
            ExpressionEvaluator* m_evaluator = nullptr;

            OPCodeContainer::iterator m_iterator;

        public:
            Expression() {}
            Expression(const OPCodeContainer &);

            void setOPCodes(const OPCodeContainer &);
            void push(const OPCode &);
            void clear();

            const OPCode &top() const { return m_opcodes.back(); }
            const OPCode &last() { return *(m_iterator--); }
            const OPCode &next() { return *(m_iterator++); }
            void toBegin() { m_iterator = m_opcodes.begin(); }
            void toBack() { m_iterator = --m_opcodes.end(); }
            const OPCode &current() const { return *m_iterator; }
            bool eof() const { return m_iterator == m_opcodes.end(); }

            OPCodeContainer *operator->() { return &m_opcodes; }
            const OPCodeContainer *operator->() const { return &m_opcodes; }

            Ref<Variant> evaluate(Ref<Block>&);
            void show();

            Ref<Block>& block();

            std::string stringify() const;
        };
    } // namespace runtime

} // namespace phi
