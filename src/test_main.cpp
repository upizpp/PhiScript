#include <iostream>
#include <test.hpp>
#include <vector>

int main() {
    auto &tests = phi::test::get();
    std::vector<std::string> test_cases;
    for (auto &test : tests) {
        printf("[%d] %s\n", test_cases.size(), test.first.c_str());
        test_cases.push_back(test.first);
    }
    size_t opt;
    while (std::cin >> opt) {
        if (opt >= tests.size()) {
            printf("Index out of range.\n");
            continue;
        }
        tests[test_cases[opt]]->run();
    }
    return 0;
}
