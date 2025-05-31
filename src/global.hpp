#pragma once
#include <cstdint>
#include <memory>
#include <string>


#define PROPERTY(type, name)                                                   \
  private:                                                                     \
    type _M_##name;                                                            \
                                                                               \
  public:                                                                      \
    type &name() { return _M_##name; }                                         \
    const type &name() const { return _M_##name; }

namespace phi {
using int_t = int64_t;
using real_t = double;
using std::shared_ptr;
using std::string;
using std::unique_ptr;
} // namespace phi
