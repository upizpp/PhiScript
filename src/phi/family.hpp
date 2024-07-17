#pragma once
#include <functional>
#include <ostream>
#include <typedef.hpp>

namespace phi {
class Variable;
using array = std::vector<Variable>;
struct Family {
    enum class Type {
        INT,
        REAL,
        BOOL,
        STRING,
        ARRAY,
        DICTIONARY,
        OBJECT,
        MAX,
    };

    string_view name;
    Type type;
    std::function<void(void *&, void *const &)> copyConstruct;
    std::function<void(void *&, void *&&)> moveConstruct;
    std::function<void(void *&)> destroy;
    std::function<void(void *&, const Variable &)> assign;
    std::function<void(void *&, Type)> convert;
    std::function<void(void *const &, std::ostream &)> print;

    Family &reinitialize(Type new_type, std::string_view new_name) {
        type = new_type;
        name = new_name;
        return *this;
    }
};

template <typename T> constexpr Family::Type GetType = Family::Type::OBJECT;

template <> constexpr Family::Type GetType<int_t> = Family::Type::INT;
template <> constexpr Family::Type GetType<real_t> = Family::Type::REAL;
template <> constexpr Family::Type GetType<bool> = Family::Type::BOOL;
template <> constexpr Family::Type GetType<string> = Family::Type::STRING;
template <> constexpr Family::Type GetType<array> = Family::Type::ARRAY;
// template <> Family::Type GetType<dictionary> = Family::Type::DICTIONARY;

} // namespace phi
