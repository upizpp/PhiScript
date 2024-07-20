#pragma once

#include <runtime/program.hpp>

namespace phi {
unique_ptr<Program> compile(Scanner &&scanner);
} // namespace phi
