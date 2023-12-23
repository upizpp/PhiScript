#pragma once
#include "compiler/ast/ast"
#include "variant.hpp"

namespace phi
{
    class Function
    {
    private:
        Owner<compiler::ast::Node> m_node;
        Owner<std::string> m_name;
        std::vector<std::string> m_args;
        bool m_is_vararg = false;

    public:
        Function() = default;
        Function(const std::string &name) : m_name(new std::string{name}) {}

        Variant call(array &);
        Variant operator()(array &args) { return call(args); }

        void setArgs(const std::vector<std::string> &args) { m_args = args; }
        bool hasName() const { return m_name; }
        void name(const std::string &name) { m_name = new std::string{name}; }
        const std::string &name() const { return *m_name; }

        void locate(Ref<runtime::Block> block) { m_node->block()->setParent(block); }

        void setNode(Owner<compiler::ast::Node> &&node) { m_node = std::move(node); }

        string stringify() const;
    };
} // namespace phi
