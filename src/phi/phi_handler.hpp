#pragma once
#include <phi/typedef.hpp>
#include <phi/variant.hpp>
#include <phi/compiler/scanner.hpp>
#include <functional>

namespace phi
{
    void interact();
    Ref<Variant> doFile(const string& path, const array& args);
    void compileTo(Scanner*, const string& dest);
    void tryRun(const std::function<void()> what);
} // namespace phi
