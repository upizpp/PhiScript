#include "ast.hpp"
#include <follower.hpp>

namespace phi {
namespace ast {
Node::Node(): line(ProgramFollower::get().line) {
    
}
} // namespace ast

} // namespace phi
