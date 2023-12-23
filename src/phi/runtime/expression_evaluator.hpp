#pragma once
#include "typedef.hpp"
#include <functional>
#include <stack>

namespace phi
{
    namespace runtime
    {
        class ExpressionEvaluator
        {
        private:
            Ref<Block> m_block;

            std::stack<Ref<Variant>> m_data;

        public:
            typedef std::function<void(ExpressionEvaluator *, array &)> CommandHandler;
            typedef std::pair<integer, CommandHandler> CommandHandlerInfo;
            typedef std::map<OPCode::Command, CommandHandlerInfo> HandlerContainer;
            static HandlerContainer commandHandlers;

        public:
            ExpressionEvaluator() {}
            ExpressionEvaluator(Ref<Block> block) : m_block(block) {}

            ExpressionEvaluator(const ExpressionEvaluator &) = delete;
            ExpressionEvaluator(ExpressionEvaluator &&) = delete;
            ExpressionEvaluator &operator=(const ExpressionEvaluator &) = delete;
            ExpressionEvaluator &operator=(ExpressionEvaluator &&) = delete;

            void clear();
            void push(const OPCode &);
            void push(const Ref<Variant> &);
            Ref<Variant> pop();
            Ref<Variant> top();
            Ref<Variant> evalExpr(Expression &);

            Ref<Block>& block() { return m_block; }
            const Ref<Block>& block() const { return m_block; }
            void block(Ref<Block> block) { m_block = std::move(block); }
        };
    } // namespace runtime

} // namespace phi
