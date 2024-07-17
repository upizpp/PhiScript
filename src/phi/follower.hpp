#pragma once
#include <singleton.hpp>
#include <typedef.hpp>

namespace phi {
struct ProgramFollower : Singleton<ProgramFollower> {
    uint64_t line;
};

inline uint64_t getCurrentLine() { return ProgramFollower::get().line; }

} // namespace phi
