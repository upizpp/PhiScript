#pragma once
#include <phi/typedef.hpp>
#include <phi/variant.hpp>
#include <functional>

namespace phi
{
    void interact();
    Ref<Variant> doFile(const string& path, const array& args);
    void tryRun(const std::function<void()> what);
} // namespace phi
