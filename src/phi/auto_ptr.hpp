#pragma once
#include <map>
#include <ostream>

namespace phi
{
    struct RefCount
    {
        using counter_t = unsigned int;
        static std::map<void *, counter_t> *data_ptr;

        static counter_t count(void *ptr)
        {
            return (*data_ptr)[ptr];
        }

        static void reference(void *ptr)
        {
            if (data_ptr == nullptr)
                data_ptr = new std::map<void *, counter_t>();
            if (data_ptr->find(ptr) == data_ptr->end())
                (*data_ptr)[ptr] = 1;
            (*data_ptr)[ptr]++;
        }

        static bool dereference(void *ptr)
        {
            if (!data_ptr)
                return false;
            if (data_ptr->find(ptr) == data_ptr->end())
                return false;
            --(*data_ptr)[ptr];
            if ((*data_ptr)[ptr] == 0)
            {
                data_ptr->erase(ptr);
                return true;
            }
            return false;
        }
    };

    template <typename T>
    class Reference
    {
    private:
        T *_M_ptr;

    public:
        using counter_t = unsigned int;

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
        template <typename U,
                  typename = typename std::enable_if<std::is_base_of<T, U>::value || std::is_base_of<U, T>::value>::type>
        Reference(const Reference<U> &ref) : _M_ptr(static_cast<T *>(const_cast<U *>(ref.data())))
        {
            reference();
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

        T *release()
        {
            T *tmp = _M_ptr;
            reset(nullptr);
            return tmp;
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

        counter_t count() { return RefCount::count(_M_ptr); }

        T &operator*() { return *_M_ptr; }
        const T &operator*() const { return *_M_ptr; }
        T *operator->() { return _M_ptr; }
        const T *operator->() const { return _M_ptr; }

        operator bool() const
        {
            return _M_ptr;
        }

        bool operator==(const Reference<T> &ref) const
        {
            return _M_ptr == ref._M_ptr;
        }
        bool operator==(const T *ptr) const
        {
            return _M_ptr == ptr;
        }
        bool operator!=(const Reference<T> &ref) const
        {
            return _M_ptr != ref._M_ptr;
        }
        bool operator!=(const T *ptr) const
        {
            return _M_ptr != ptr;
        }

        void reference()
        {
            if (!_M_ptr)
                return;
            RefCount::reference(_M_ptr);
        }

        void dereference()
        {
            if (!_M_ptr)
                return;
            if (RefCount::dereference(_M_ptr))
                delete _M_ptr;
        }
    };

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
            reset(std::move(owner));
            return *this;
        }

        T *release()
        {
            T *tmp = _M_ptr;
            _M_ptr = nullptr;
            return tmp;
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

        bool operator==(const Owner<T> &owner) const
        {
            return _M_ptr == owner._M_ptr;
        }
        bool operator==(const T *ptr) const
        {
            return _M_ptr == ptr;
        }
        bool operator!=(const Owner<T> &ptr) const
        {
            return _M_ptr != ptr;
        }
        bool operator!=(const T *ptr) const
        {
            return _M_ptr != ptr;
        }

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

    template <typename T>
    class Borrower
    {
    private:
        T *_M_ptr;

    public:
        Borrower() : _M_ptr(nullptr) {}
        Borrower(T *ptr) : _M_ptr(ptr) {}
        Borrower(Reference<T> ref) : _M_ptr(ref.data()) {}
        Borrower(Owner<T> owner) : _M_ptr(owner.data()) {}

        T *data() { return _M_ptr; }
        const T *data() const { return _M_ptr; }
        T &operator*() { return *_M_ptr; }
        const T &operator*() const { return *_M_ptr; }
        T *operator->() { return _M_ptr; }
        const T *operator->() const { return _M_ptr; }

        void reset(T *ptr)
        {
            if (_M_ptr == ptr)
                return;
            _M_ptr = ptr;
        }
        void reset(const Borrower<T> &borrower)
        {
            if (this == &borrower)
                return;
            _M_ptr = borrower._M_ptr;
        }
        void reset(const Reference<T> &ref)
        {
            if (_M_ptr == ref.data())
                return;
            _M_ptr = ref.data();
        }
        void reset(const Owner<T> &owner)
        {
            if (_M_ptr == owner.data())
                return;
            _M_ptr = owner.data();
        }
        T *release()
        {
            T *tmp = _M_ptr;
            reset(nullptr);
            return tmp;
        }

        Borrower<T> &operator=(T *ptr)
        {
            reset(ptr);
            return *this;
        }
        Borrower<T> &operator=(const Borrower<T> &borrower)
        {
            reset(borrower);
            return *this;
        }

        operator bool()
        {
            return _M_ptr;
        }

        bool operator==(const Borrower<T> &borrower) const
        {
            return _M_ptr == borrower._M_ptr;
        }
        bool operator==(const T *ptr) const
        {
            return _M_ptr == ptr;
        }
        bool operator!=(const Borrower<T> &borrower) const
        {
            return _M_ptr != borrower._M_ptr;
        }
        bool operator!=(const T *ptr) const
        {
            return _M_ptr != ptr;
        }

        operator Reference<T>()
        {
            return Reference<T>(_M_ptr);
        }
        operator Owner<T>()
        {
            return Owner<T>(_M_ptr);
        }
    };
} // namespace phi

namespace std
{
    template <typename T>
    struct hash<phi::Reference<T>>
    {
        size_t operator()(const phi::Reference<T> &ref) const
        {
            return std::hash<T>()(*ref);
        }
    };

} // namespace std
