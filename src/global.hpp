#pragma once
#include <cstdint>
#include <memory>
#include <string>

#define PROPERTY(type, name)                                                   \
  protected:                                                                   \
    type _M_##name;                                                            \
                                                                               \
  public:                                                                      \
    const type &name() const { return _M_##name; }

namespace phi {
using int_t = int64_t;
using real_t = double;
using std::make_shared;
using std::make_unique;
using std::shared_ptr;
using std::string;
using std::unique_ptr;
} // namespace phi
