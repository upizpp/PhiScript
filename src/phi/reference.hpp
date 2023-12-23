#pragma once
#include "data_type.hpp"
#include <iostream>
#include <type_traits>
#include <utility>

namespace phi
{
    template <typename T>
    class Reference
    {
    private:
        uinteger *m_count = nullptr;
        T *m_data = nullptr;

        void reference();
        void dereference();

        void init_counter()
        {
            m_count = new uinteger;
            *m_count = 1;
        }

    public:
        Reference() noexcept = default;

        Reference(const T &data) noexcept
        {
            m_data = new T(data);
            init_counter();
        }

        Reference(T &&data) noexcept
        {
            m_data = new T(std::forward<T>(data));
            init_counter();
        }

        template <typename U>
            requires std::is_base_of_v<T, U>
        Reference(U *data) noexcept : m_data(data)
        {
            init_counter();
        }

        Reference(T *data) noexcept : m_data(data)
        {
            init_counter();
        }

        Reference(T *data, uinteger* count) noexcept : m_data(data), m_count(count)
        {
            reference();
        }

        Reference(T *data, bool) noexcept : m_data(data)
        {
        }

        Reference(const Reference<T> &ref) noexcept : m_data(ref.m_data)
        {
            m_count = ref.m_count;
            reference();
        }

        Reference(Reference<T> &&ref) noexcept : m_data(ref.data())
        {
            m_count = ref.m_count;
            ref.m_count = nullptr;
        }

        Reference<T> &operator=(T *data)
        {
            reset(data);
            return *this;
        }
        Reference<T> &operator=(const Reference<T> &ref);
        Reference<T> &operator=(Reference<T> &&ref);

        T *release() noexcept
        {
            dereference();
            m_count = nullptr;
            T *temp = m_data;
            m_data = nullptr;
            return temp;
        }

        Reference<T> borrow()
        {
            return {m_data, true};
        }
        void reset(T *data);

        uinteger count() const noexcept { return *m_count; }
        bool isNull() const noexcept { return m_data == nullptr; }
        bool isOwner() const noexcept { return m_count != nullptr; }

        void detach() noexcept { m_count = nullptr; }

        Reference<T> *address() { return this; }
        const Reference<T> *address() const { return this; }
        T *instance() { return m_data; }
        const T *instance() const { return m_data; }
        T *data() { return m_data; }
        const T *data() const { return m_data; }

        T *operator&() { return instance(); }
        const T *operator&() const { return instance(); }
        T *operator->() { return &*this; }
        const T *operator->() const { return &*this; }

        T &operator*() { return *instance(); }
        const T &operator*() const { return *instance(); }

        template <typename U>
            requires std::is_base_of_v<U, T>
        operator Reference<U>()
        {
            return Reference<U>(m_data, m_count);
        }

        bool operator==(const Reference<T> &ref) { return *data() == *ref.data(); }
        bool operator!=(const Reference<T> &ref) { return *data() != *ref.data(); }

        ~Reference() noexcept { dereference(); }

        operator bool() const { return !isNull(); }
    };

    template <typename T>
    Reference(T *) -> Reference<T>;

    template <typename T>
    using Ref = Reference<T>;

    template <typename T>
    inline std::ostream &operator<<(std::ostream &os, const Ref<T> &ref)
    {
        if (!ref.isNull())
            os << *ref;
        else
            os << ":nullptr:";
        return os;
    }

    template <typename T>
    inline void Reference<T>::reference()
    {
        if (m_count == nullptr)
            return;
        ++(*m_count);
    }

    template <typename T>
    inline void Reference<T>::dereference()
    {
        if (m_count == nullptr)
            return;
        --(*m_count);
        if (*m_count == 0)
        {
            delete m_data;
            delete m_count;
        }
    }

    template <typename T>
    inline Reference<T> &Reference<T>::operator=(const Reference<T> &ref)
    {
        dereference();
        m_count = ref.m_count;
        m_data = ref.m_data;
        reference();
        return *this;
    }

    template <typename T>
    inline Reference<T> &Reference<T>::operator=(Reference<T> &&ref)
    {
        dereference();
        m_data = ref.data();
        m_count = ref.m_count;
        ref.m_count = nullptr;
        return *this;
    }

    template <typename T>
    inline void Reference<T>::reset(T *data)
    {
        dereference();
        m_data = data;
        init_counter();
    }
} // namespace upi
