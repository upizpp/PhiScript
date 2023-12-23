#pragma once
#include <phi/reference.hpp>
#include "typedef.hpp"
#include <iostream>

namespace phi
{
    namespace runtime
    {
        class Block
        {
        public:
        private:
            VariantPool m_locals;
            Ref<Block> m_parent;

        public:
            Block() {}
            Block(const Ref<Block> &p) : m_parent(p->m_parent), m_locals(p->m_locals) {}

            void setParent(const Ref<Block> &p) { m_parent = p; }
            Ref<Block> getParent() const { return m_parent; }

            void showLocals() const { showLocals(std::cout); }
            void showLocals(std::ostream &) const;

            bool hasLocal(const string &) const;
            Ref<Variant> getLocal(const string &);
            Ref<Variant> setLocal(const string &, const Ref<Variant> &);
            void deleteLocal(const string &);
            bool hasVariable(const string &) const;
            Ref<Variant> loadVariable(const string &);
            Ref<Variant> allocateVariable(const string &);
            void deallocateVariables();
            void deleteVariable(const string &);

            bool operator==(const Block &block) { return this == &block; }
        };
    } // namespace runtime

} // namespace phi
