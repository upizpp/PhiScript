#pragma once
#include <phi/variant.hpp>

namespace phi {
    bool numeric(Variant::Type type);

    Variant::Type improve(Variant::Type, Variant::Type);
} // namespace phi
