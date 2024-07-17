#include "native_families.hpp"
#include <exception.hpp>
#include <variable.hpp>

namespace phi {

template <typename T>
static auto CopyConstruct = [](void *&self, void *const &data) {
    //
    self = new T{*(T *)data};
};
template <typename T>
static auto MoveConstruct = [](void *&self, void *&&data) {
    //
    self = data;
    data = nullptr;
};
template <typename T>
static auto Destroy = [](void *&self) {
    delete (T *)self;
    self = nullptr;
};
template <typename T>
static auto Assign = [](void *&self, const Variable &other) {
    switch (other.type()) {
    case GetType<T>:
        *(T *)self = *(T *)other.data();
    default:
        try {
            *(T *)self = *(T *)other.convert(GetType<T>).data();
        } catch (ConvertException) {
            throw RuntimeException("Assign error");
        }
    }
};
template <typename T> static auto Convert = [](void *&self, Family::Type) {};
template <typename T>
static auto Print = [](void *const &self, std::ostream &os) { //
    os << *(T *)self;
};

template <typename T>
Family templateFamily{
    .copyConstruct = CopyConstruct<T>,
    .moveConstruct = MoveConstruct<T>,
    .destroy = Destroy<T>,
    .assign = Assign<T>,
    .convert = Convert<T>,
    .print = Print<T>,
};

Family &G_intFamily =
    templateFamily<int_t>.reinitialize(Family::Type::INT, "int");
Family &G_realFamily =
    templateFamily<real_t>.reinitialize(Family::Type::REAL, "real");
Family &G_boolFamily =
    templateFamily<bool>.reinitialize(Family::Type::REAL, "real");
Family &G_stringFamily =
    templateFamily<string>.reinitialize(Family::Type::STRING, "string");
} // namespace phi
