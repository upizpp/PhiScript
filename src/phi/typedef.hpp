#pragma once

#include <cstdint>
#include <iostream>
#include <memory>
#include <string>

#define __always_inline inline __attribute__((always_inline))

#define PRIVATE(name) _M_##name
#define PROPERTY(type, name, default_)                                         \
  private:                                                                     \
    type PRIVATE(name) = default_;                                             \
                                                                               \
  public:                                                                      \
    __always_inline type &name() { return PRIVATE(name); }                     \
    __always_inline const type &name() const { return PRIVATE(name); }

namespace phi {
using std::int16_t;
using std::int32_t;
using std::int64_t;
using std::int8_t;
using std::uint16_t;
using std::uint32_t;
using std::uint64_t;
using std::uint8_t;

using char_t = unsigned char;
using real_t = double;

using std::string;

using std::cout;
using std::endl;

using std::make_shared;
using std::make_unique;
using std::move;
using std::shared_ptr;
using std::unique_ptr;
using std::weak_ptr;

namespace detail {
template <class T, std::size_t N, std::size_t... I>
constexpr std::array<std::remove_cv_t<T>, N>
to_array_impl(T (&&a)[N], std::index_sequence<I...>) {
    return {{std::move(a[I])...}};
}
} // namespace detail

template <class T, std::size_t N>
constexpr std::array<std::remove_cv_t<T>, N> to_array(T (&&a)[N]) {
    return detail::to_array_impl(std::move(a), std::make_index_sequence<N>{});
}

template <typename T> using native_ptr = T *;

template <typename F> struct __func_ptr_impl;
template <typename R, typename... Args> struct __func_ptr_impl<R(Args...)> {
    using type = R (*)(Args...);
};
template <typename F> using func_ptr = typename __func_ptr_impl<F>::type;

} // namespace phi
