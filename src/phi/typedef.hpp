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

template <typename T> using native_ptr = T *;

template <typename F> struct __func_ptr_impl;
template <typename R, typename... Args> struct __func_ptr_impl<R(Args...)> {
    using type = R (*)(Args...);
};
template <typename F> using func_ptr = typename __func_ptr_impl<F>::type;

} // namespace phi
