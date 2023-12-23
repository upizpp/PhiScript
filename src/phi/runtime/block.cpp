#include <phi/exception.hpp>
#include "block.hpp"

namespace phi
{
    namespace runtime
    {
        void Block::showLocals(std::ostream &os) const
        {
            for (auto &&variable : m_locals)
            {
                os << variable.first << ' ' << variable.second << std::endl;
            }
        }

        bool Block::hasLocal(const string &name) const
        {
            return m_locals.find(name) != m_locals.end();
        }

        Ref<Variant> Block::getLocal(const string &name)
        {
            return m_locals[name];
        }

        Ref<Variant> Block::setLocal(const string &name, const Ref<Variant> &value)
        {
            m_locals[name] = value;
            return m_locals[name];
        }

        void Block::deleteLocal(const string &name)
        {
            m_locals.erase(name);
        }

        bool Block::hasVariable(const string &name) const
        {
            if (hasLocal(name))
                return true;
            if (!m_parent.isNull())
                return m_parent->hasVariable(name);
            return false;
        }

        Ref<Variant> Block::loadVariable(const string &name)
        {
            if (hasLocal(name))
                return getLocal(name);
            if (!m_parent.isNull() && m_parent->hasVariable(name))
                return m_parent->loadVariable(name);
            return Variant::Null;
        }

        Ref<Variant> Block::allocateVariable(const string &name)
        {
            if (hasVariable(name))
                throw Exception("Request memory for an identifier '%s' that has already been applied for.", name.c_str());
            return setLocal(name, new Variant);
        }

        void Block::deallocateVariables()
        {
            m_locals.clear();
        }

        void Block::deleteVariable(const string &name)
        {
            if (hasLocal(name))
                deleteLocal(name);
            if (!m_parent.isNull())
                m_parent->deleteVariable(name);
        }
    } // namespace runtime
} // namespace phi
