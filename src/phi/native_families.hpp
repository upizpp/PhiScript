#pragma once

#include <family.hpp>

namespace phi {
extern Family &G_intFamily;
extern Family &G_realFamily;
extern Family &G_boolFamily;
extern Family &G_stringFamily;

namespace detail {
template <typename T> struct familyOfImpl {
    static Family &get() { return G_intFamily; }
};
template <> struct familyOfImpl<int_t> {
    static Family &get() { return G_intFamily; }
};
template <> struct familyOfImpl<real_t> {
    static Family &get() { return G_realFamily; }
};
template <> struct familyOfImpl<bool> {
    static Family &get() { return G_boolFamily; }
};
template <> struct familyOfImpl<string> {
    static Family &get() { return G_stringFamily; }
};
} // namespace detail

template <typename T> inline Family &familyOf() {
    return detail::familyOfImpl<T>::get();
}
} // namespace phi
