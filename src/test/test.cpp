#include "test.hpp"

namespace phi {
namespace test {
std::map<std::string, Test *> &get() {
    static std::map<std::string, Test *> _tests;
    return _tests; 
}
void add(const std::string &name, Test *test) {
    test->__name__ = name;
    get().insert(std::make_pair(name, test));
}

} // namespace test`
} // namespace phi