#pragma once

#include <cstdio>
#include <iostream>
#include <list>
#include <map>
#include <sstream>
#include <string>
#include <typeinfo>

#define TEST(CLASS) static phi::test::TestRegister<phi::test::CLASS> _(#CLASS);

#define TESTSECTION(name) TestHelper _(name, this, __FILE__)
#define ASSERT_TRUE(what) assert_true(what, #what, __LINE__)
#define ASSERT_FALSE(what) assert_false(what, #what, __LINE__)
#define ASSERT_EQUAL(a, b) assert_equal(a, b, #a " == " #b, __LINE__)
#define LISTEN_BEGIN() listen_begin()
#define LISTEN_END(expected) listen_end(expected, __LINE__)

namespace phi {
namespace test {
struct Test {
    void run() {
        printl("[Test: ", __name__, "]");
        _run();
    }
    virtual void _run() = 0;

    std::string __name__;

  private:
    struct Subtest {
        std::string name;
        size_t test_points = 0;
        size_t succeed_points = 0;
    };
    std::list<Subtest> _M_tests;
    std::ostringstream _M_listener;
    bool _M_is_listening = false;

    void print_test(size_t line) {
        printf("\033[3m");
        bool first = true;
        for (auto &&test : _M_tests) {
            printf("%c%s", !first ? '.' : '\0', test.name.c_str());
            first = false;
        }
        printf("\033[0m at L%d ", line);
    }

  protected:
    struct TestHelper {
        TestHelper(const char *name, Test *owner, const char *filename)
            : _M_owner(owner) {
            owner->begin(name, filename);
        }
        ~TestHelper() { _M_owner->end(); }

      private:
        Test *_M_owner;
    };

    void listen_begin() { _M_is_listening = true; }
    void listen_end(const std::string &expected, size_t line) {
        _M_is_listening = false;
        assert_equal(_M_listener.str(), expected,
                     ("listener.str() == " + expected).c_str(), line);
        _M_listener.str("");
    }

    template <typename... Args> void printl(const Args &...args) {
        if (!_M_is_listening) {
            ((std::cout << args << ' '), ...);
            std::cout << '\n';
        } else {
            ((_M_listener << args << ' '), ...);
            _M_listener << '\n';
        }
    }
    template <typename... Args> void prints(const Args &...args) {
        if (!_M_is_listening) {
            ((std::cout << args << ' '), ...);
        } else {
            ((_M_listener << args << ' '), ...);
        }
    }
    template <typename... Args> void printt(const Args &...args) {
        if (!_M_is_listening) {
            ((std::cout << args), ...);
            std::cout << '\n';
        } else {
            ((_M_listener << args), ...);
            _M_listener << '\n';
        }
    }
    template <typename... Args> void printts(const Args &...args) {
        if (!_M_is_listening) {
            ((std::cout << args), ...);
        } else {
            ((_M_listener << args), ...);
        }
    }
    template <typename... Args>
    void printf(const char *format, const Args &...args) {
        if (!_M_is_listening) {
            std::printf(format, args...);
        }
    }

    void begin(const std::string &name, const char *filename) {
        printt("Test Section in \"\033[1m", filename, "\033[0m\" \033[33;1m",
               name, "\033[0m started.");
        _M_tests.push_back(Subtest{.name = name});
    }
    void end() {
        printt("Test \033[33;1m", _M_tests.back().name,
               "\033[0m finished with \033[32;1m",
               _M_tests.back().succeed_points, "\033[0m succeed, ",
               "\033[31;1m",
               _M_tests.back().test_points - _M_tests.back().succeed_points,
               "\033[0m failed.");
        printl("---------------------------------------------------------------"
               "-------------------------------------");
        _M_tests.pop_back();
    }
    void assert_true(bool what, const char *s, size_t line) {
        printf("[");
        print_test(line);
        printt("TestPoint \033[1m", _M_tests.back().test_points++, "\033[0m] ",
               what ? "\033[32;1mSucceed\033[0m" : "\033[31;1mFailed\033[0m");
        printt("    ", s);
        if (what)
            ++_M_tests.back().succeed_points;
    }
    void assert_false(bool what, const char *s, size_t line) {
        assert_true(!what, s, line);
    }

    template <typename T, typename U>
    void assert_equal(const T &a, const U &b, const char *s, size_t line) {
        static std::ostringstream ss;
        ss.str("");
        ss << s << ", got " << a << '.';
        assert_true(a == b, ss.str().c_str(), line);
    }
};
std::map<std::string, Test *> &get();
void add(const std::string &name, Test *test);

template <typename T> struct TestRegister {
    TestRegister(const char *name) { add(name, new T); }
};
} // namespace test
} // namespace phi