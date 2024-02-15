#pragma once
#include <phi/typedef.hpp>
#include <phi/variant.hpp>

namespace phi {
    map<string, Ref<Variant>> &getGlobals();
    void setGlobal(const string &, Ref<Variant>);
    Ref<Variant> getGlobal(const string &);
    Borrower<std::pair<const string, Ref<Variant>>>
    getGlobalPair(const string &);
    bool hasGlobal(const string &);
} // namespace phi
