#pragma once
#include "logger.hpp"
#include "phi_entry.hpp"
#include "owner.hpp"
#include <cstdio>
#include <iostream>
#include <typeinfo>

namespace phi
{
    template <typename T>
    class Singleton
    {
        typedef T value_type;

    private:
        static Owner<T> m_instance;

    public:
        Singleton() = delete;
        Singleton(const Singleton &) = delete;
        Singleton(Singleton &&) = delete;
        ~Singleton() = delete;
        Singleton &operator=(const Singleton &) = delete;
        Singleton &operator=(Singleton &&) = delete;

        inline static T* instance()
        {
            if (m_instance.isNull())
            {
                m_instance.reset(new T());
            }
            return m_instance.borrow();
        }
    };

    template<typename T>
    Owner<T> Singleton<T>::m_instance = nullptr;
} // namespace phi
