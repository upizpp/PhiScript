#pragma once
#include <functional>
#include <phi/compiler/scanner.hpp>
#include <phi/typedef.hpp>
#include <phi/variant.hpp>

namespace phi {
    void interact();
    Ref<Variant> doFile(const string &path, const array &args,
                        const string &name = "__main__");
    void compileTo(Scanner *, const string &dest);
    void tryRun(const std::function<void()> what);
} // namespace phi
