#pragma once
#include <singleton.hpp>
#include <typedef.hpp>

namespace phi {
struct ProgramFollower : Singleton<ProgramFollower> {
    uint64_t line;
};

} // namespace phi
