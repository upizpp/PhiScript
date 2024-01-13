#pragma once
#include <map>
#include <ostream>

namespace phi
{
    template <typename T>
    class Reference
    {
    private:
        T *_M_ptr;

    public:
        using counter_t = unsigned int;
        static std::map<const T *, counter_t> refCount;

        Reference() : _M_ptr(nullptr)
        {
        }
        Reference(T *ptr) : _M_ptr(ptr)
        {
            reference();
        }
        Reference(const Reference<T> &ref) : _M_ptr(ref._M_ptr)
        {
            reference();
        }
        Reference(Reference<T> &&ref) : _M_ptr(ref._M_ptr)
        {
            ref._M_ptr = nullptr;
        }
        ~Reference()
        {
            dereference();
        }

        Reference<T> &operator=(T *ptr)
        {
            reset(ptr);
            return *this;
        }
        Reference<T> &operator=(const Reference<T> &ref)
        {
            reset(ref);
            return *this;
        }
        Reference<T> &operator=(Reference<T> &&ref)
        {
            if (this == &ref)
                return *this;
            _M_ptr = ref._M_ptr;
            ref._M_ptr = nullptr;
            return *this;
        }

        void reset(T *ptr)
        {
            if (_M_ptr == ptr)
                return;
            dereference();
            _M_ptr = ptr;
            reference();
        }

        void reset(const Reference<T> &ref)
        {
            if (this == &ref)
                return;
            dereference();
            _M_ptr = ref._M_ptr;
            reference();
        }

        T *data() { return _M_ptr; }
        const T *data() const { return _M_ptr; }

        counter_t count() { return refCount[_M_ptr]; }

        T &operator*() { return *_M_ptr; }
        const T &operator*() const { return *_M_ptr; }
        T *operator->() { return _M_ptr; }
        const T *operator->() const { return _M_ptr; }

        operator bool()
        {
            return _M_ptr;
        }

        void reference()
        {
            if (!_M_ptr)
                return;
            if (refCount.find(_M_ptr) == refCount.end())
                refCount.insert({_M_ptr, 0});
            ++refCount[_M_ptr];
        }

        void dereference()
        {
            if (!_M_ptr)
                return;
            if (refCount.find(_M_ptr) == refCount.end())
                return;
            --refCount[_M_ptr];
            if (refCount[_M_ptr] == 0)
            {
                refCount.erase(_M_ptr);
                delete _M_ptr;
            }
        }
    };

    template <typename T>
    std::map<const T *, typename Reference<T>::counter_t> Reference<T>::refCount;

    template <typename T>
    std::ostream &operator<<(std::ostream &os, const Reference<T> &ref)
    {
        os << ref.data();
        return os;
    }

    template <typename T>
    class Owner
    {
    private:
        T *_M_ptr;

    public:
        Owner() : _M_ptr(nullptr) {}
        Owner(T *ptr) : _M_ptr(ptr) {}
        Owner(const Owner &) = delete;
        Owner(Owner &&owner) : _M_ptr(owner._M_ptr)
        {
            owner._M_ptr = nullptr;
        }
        ~Owner() { delete _M_ptr; }

        Owner &operator=(T *ptr)
        {
            reset(ptr);
            return *this;
        }
        Owner &operator=(const Owner &) = delete;
        Owner &operator=(Owner &&owner)
        {
            reset(owner);
            return *this;
        }

        void reset(T *ptr)
        {
            if (_M_ptr == ptr)
                return;
            delete _M_ptr;
            _M_ptr = ptr;
        }

        void reset(Owner<T> &&owner)
        {
            if (this == &owner)
                return;
            delete _M_ptr;
            _M_ptr = owner._M_ptr;
            owner._M_ptr = nullptr;
        }

        T *data() { return _M_ptr; }
        const T *data() const { return _M_ptr; }

        T &operator*() { return *_M_ptr; }
        const T &operator*() const { return *_M_ptr; }
        T *operator->() { return _M_ptr; }
        const T *operator->() const { return _M_ptr; }

        operator bool()
        {
            return _M_ptr;
        }
    };

    template <typename T>
    std::ostream &operator<<(std::ostream &os, const Owner<T> &owner)
    {
        os << owner.data();
        return os;
    }
} // namespace phi