#include "native_families.hpp"
#include <variable.hpp>

namespace phi {
Family G_intFamily;
struct initializer {
    initializer() {
        G_intFamily.constructor = [](void *data) -> Variable {
            return Variable(&G_intFamily, new int(*(int *)data));
        };
    }
};
initializer _;
} // namespace phi
