#pragma once
#include <family.hpp>
#include <typedef.hpp>

namespace phi {
class Variable {
    PROPERTY(native_ptr<Family>, family, nullptr)
    PROPERTY(native_ptr<void>, data, nullptr)

  public:
    Variable(Family *family, void *data) : _M_family(family), _M_data(data) {}
    ~Variable() { family()->destroy(data()); }
};
} // namespace phi
