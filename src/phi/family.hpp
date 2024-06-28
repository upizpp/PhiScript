#pragma once
#include <typedef.hpp>

namespace phi {
class Variable;
struct Family {
    func_ptr<Variable(void *)> constructor;
    func_ptr<void(void *)> destroy;
};
} // namespace phi
