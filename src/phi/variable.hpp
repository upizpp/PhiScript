#pragma once
#include <native_families.hpp>
#include <ostream>
#include <typedef.hpp>
#include <unordered_map>
#include <vector>

#define USE_LEFT true

namespace phi {
class Variable {
    PROPERTY(native_ptr<Family>, family, nullptr)
    PROPERTY(native_ptr<void>, data, nullptr)

  public:
    template <typename T>
    explicit Variable(const T &value)
        : _M_family(&familyOf<T>()) {
        family()->copyConstruct(data(), (void *const)&value);
    }
    Variable(const Variable &other) : _M_family(other.family()) {
        family()->copyConstruct(data(), other.data());
    }
    Variable(Family *f, void *const &d, bool = USE_LEFT) : _M_family(f) {
        family()->copyConstruct(data(), d);
    }
    Variable(Family *f, void *&&d) : _M_family(f) {
        family()->moveConstruct(data(), std::move(d));
    }
    ~Variable() { family()->destroy(data()); }

    Family::Type type() const { return family()->type; }
    Variable convert(Family::Type type) const {
        Variable temp = *this;
        temp.convertTo(type);
        return temp;
    }
    void convertTo(Family::Type type) { family()->convert(data(), type); }

    Variable &operator=(const Variable &other) {
        family()->assign(data(), other);
        return *this;
    }

    template <typename T> T &to() { return (T)*data(); }
    template <typename T> const T &to() const { return *(T *)data(); }
};

inline std::ostream &operator<<(std::ostream &os, const Variable &v) {
    v.family()->print(v.data(), os);
    return os;
}

using array = std::vector<Variable>;
} // namespace phi
