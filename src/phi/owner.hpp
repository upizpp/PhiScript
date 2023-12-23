#pragma once
#include "reference.hpp"

namespace phi
{
    template <typename T>
    class Owner
    {
    private:
        T *m_data = nullptr;

    public:
        Owner() noexcept = default;
        Owner(const Owner &owner) noexcept : m_data(owner ? new T(*owner.m_data) : nullptr)
        {
        }
        Owner(T *data) : m_data(data)
        {
        }
        Owner(Owner &&other) noexcept
        {
            m_data = other.release();
        }

        Owner &operator=(T *data) noexcept
        {
            reset(data);
            return *this;
        }
        Owner &operator=(const Owner &owner) noexcept
        {
            reset(owner ? new T(*owner.m_data) : nullptr);
            return *this;
        }
        Owner &operator=(Owner &&other) noexcept
        {
            if (this != &other)
            {
                delete m_data;
                m_data = other.release();
            }
            return *this;
        }

        ~Owner()
        {
            delete m_data;
        }

        T *release()
        {
            T *temp = m_data;
            m_data = nullptr;
            return temp;
        }

        void reset(T *data)
        {
            if (m_data != data)
            {
                delete m_data;
                m_data = data;
            }
        }

        const T *operator->() const noexcept { return m_data; }
        T *operator->() noexcept { return m_data; }

        const T &operator*() const noexcept { return *m_data; }
        T &operator*() noexcept { return *m_data; }

        operator T *() noexcept { return release(); }

        T *data() { return m_data; }
        const T *data() const { return m_data; }

        T *borrow() { return m_data; }
        const T *borrow() const { return m_data; }

        Ref<T> asRef() { return Ref<T>(m_data, true); }

        bool isNull() const { return m_data == nullptr; }

        bool operator==(const T *ptr) const { return m_data == ptr; }

        operator bool() const { return !isNull(); }

        operator Ref<T>() { return asRef(); }
    };
} // namespace upi
