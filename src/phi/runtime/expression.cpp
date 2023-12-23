#include "expression.hpp"
#include "expression_evaluator.hpp"
#include <sstream>

namespace phi
{
    namespace runtime
    {
        Expression::Expression(const OPCodeContainer &opcodes) : m_opcodes(opcodes)
        {
            toBegin();
        }

        void Expression::setOPCodes(const OPCodeContainer &opcodes)
        {
            m_opcodes = opcodes;
        }

        void Expression::push(const OPCode &code)
        {
            m_opcodes.push_back(code);
        }

        void Expression::clear()
        {
            m_opcodes.clear();
        }

        Ref<Variant> Expression::evaluate(Ref<Block> &block)
        {
            ExpressionEvaluator evaluator;
            m_evaluator = &evaluator;
            evaluator.block(block);
            Ref<Variant> result;
            try
            {
                result = evaluator.evalExpr(*this);
            }
            catch(...)
            {
                m_evaluator = nullptr;
                throw;
            }
            m_evaluator = nullptr;
            return result;
        }

        Ref<Block> &Expression::block()
        {
            static Ref<Block> null;
            if (!m_evaluator)
                return null;
            return m_evaluator->block();
        }

        void Expression::show()
        {
            for (auto &&code : m_opcodes)
            {
                std::cout << code << std::endl;
            }
        }

        std::string Expression::stringify() const
        {
            std::ostringstream ss;
            ss << "Expression(" << m_opcodes.size() << ")";
            if (m_opcodes.size() < 10)
            {
                ss << ": [";
                bool first = true;
                for (auto &&code : m_opcodes)
                {
                    if (first)
                        first = false;
                    else
                        ss << ", ";
                    ss << OPCode::cmdStr(code.command());
                }
                ss << "]";
            }
            return ss.str();
        }
    } // namespace runtime
} // namespace phi
