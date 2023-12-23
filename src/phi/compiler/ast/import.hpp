#pragma once
#include "node.hpp"
#include <sstream>
#include <phi/builtin/methods.hpp>

namespace phi
{
    namespace compiler
    {
        namespace ast
        {
            class ImportNode : public Node
            {
            private:
                std::string m_import_name;
                std::string m_module_name;

                virtual bool _run() override
                {
                    builtin::import(block(), m_module_name, m_import_name);
                    return false;
                }

            public:
                virtual std::string nodeName() const override { return "ImportNode"; }
                virtual std::string stringify() const override
                {
                    std::ostringstream ss;
                    ss << Node::stringify() << '\t';
                    ss << "(import \"" << m_module_name << "\" as \"" << m_import_name << "\")";
                    return ss.str();
                }

                ImportNode(const std::string &import_name, const std::string &module_name) : m_import_name(import_name), m_module_name(module_name) {}
            };
        } // namespace ast

    } // namespace compiler

} // namespace phi
