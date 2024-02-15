#pragma once
#include <phi/typedef.hpp>

#define OPT_DEFAULT(variable, value)                                           \
    static decltype(variable)::value_t __##variable##_default__{value};        \
    variable.setDefault(&__##variable##_default__);

namespace phi {
    template <typename T> class Optional {
      public:
        using value_t = T;

      private:
        Owner<T> _M_val;

      public:
        Optional(){};
        Optional(nullptr_t){};
        Optional(T *ptr) : _M_val(ptr) {}
        Optional(const T &val) : _M_val(new T{val}) {}
        Optional(const Optional &opt)
            : _M_val(opt.hasVal() ? new T{*opt._M_val} : nullptr) {}
        Optional(Optional &&opt) : _M_val(std::move(opt._M_val)) {}

        T *operator->() { return _M_val->data(); }
        const T *operator->() const { return _M_val->data(); }
        T &operator*() { return *_M_val; }
        const T &operator*() const { return *_M_val; }

        void setDefault(const T &val) {
            if (!hasVal())
                _M_val = new T{val};
        }

        bool hasVal() const { return _M_val; }
        operator bool() const { return hasVal(); }
    };

    template <typename T> class OptionalRef {
      public:
        using value_t = T;

      private:
        Borrower<T> _M_val;

      public:
        OptionalRef(){};
        OptionalRef(T *ptr) : _M_val(ptr) {}
        OptionalRef(const OptionalRef &opt) : _M_val(opt._M_val) {}

        T *operator->() { return _M_val->data(); }
        const T *operator->() const { return _M_val->data(); }
        T &operator*() { return *_M_val; }
        const T &operator*() const { return *_M_val; }

        void setDefault(T *val) {
            if (!hasVal())
                _M_val = val;
        }

        bool hasVal() const { return _M_val; }
        operator bool() const { return hasVal(); }
    };
} // namespace phi
