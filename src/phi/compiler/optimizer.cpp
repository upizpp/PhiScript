#include "optimizer.hpp"

namespace phi {
    void Optimizer::optimizeSimply(State &state) {
        for (auto &&code : state.getCodes()) {
            if (code.opt() == OPCode::Command::GOTO) {
                OPCode &target = state.getCode(state.label(code.value()));
                if (target.opt() == OPCode::Command::GOTO) {
                    code.value(target.value());
                }
            }
        }
    }

    State Optimizer::optimize(const State &) {
        // TODO optimize
        return State();
    }
} // namespace phi