#pragma once
#include <phi/logger.hpp>
#include <phi/singleton.hpp>
#include <iostream>
#include <sstream>
#include <stack>
#include <string>
#include <vector>

namespace test
{
    using std::cout;
    using std::endl;
    using std::stack;
    using std::string;
    using std::vector;

    const char C_RED[] = "\033[1;31m";
    const char C_GREEN[] = "\033[1;32m";
    const char C_YELLOW[] = "\033[1;33m";
    const char C_BLUE[] = "\033[1;34m";
    const char C_AQUA[] = "\033[1;36m";
    const char C_BOLD[] = "\033[1m";
    const char C_RESET[] = "\033[0m";

    static inline const string COLOR(const char *color, const string &content)
    {
        return color + content + C_RESET;
    }
    static inline const string COLOR(const vector<const char *> &colors, const string &content)
    {
        string result;
        for (auto &&color : colors)
        {
            result += color;
        }
        result += content;
        result += C_RESET;
        return result;
    }
    static inline const string COLOR(const char *color, long long n)
    {
        return COLOR(color, std::to_string(n));
    }
    static inline const string COLOR(const vector<const char *> &colors, long long n)
    {
        return COLOR(colors, std::to_string(n));
    }

    class Test
    {
    private:
        struct Case
        {
            enum class Status
            {
                PASS,
                FAIL
            };

            Status status;
            string detail;

            string expectedly;
            string actually;
        };

        struct Group
        {
            string name;
            long tested = 0;
            long passed = 0;

            bool print_detail;

            vector<Case> cases;
        };

        stack<Group> m_groups;

    protected:
        template <typename T, typename U>
        void Assert(T actually, U expectedly, const string &detail = "");

        void GroupBegin(const string &name, bool print_detail = true);
        void GroupEnd();

        void SummarizeGroup(const Group &);

        Group &current() { return m_groups.top(); };
        const Group &current() const { return m_groups.top(); };

    public:
        void Run()
        {
            LDEBUG("Running Test...");
            _run();
            while (!m_groups.empty())
            {
                GroupEnd();
            }
        };

        virtual void _run() = 0;
    };

    template <typename T, typename U>
    void Test::Assert(T actually, U expectedly, const string &detail)
    {
        Case assert_case;
        assert_case.detail = detail;
        assert_case.status = actually == expectedly ? Case::Status::PASS : Case::Status::FAIL;

        std::ostringstream ss;
        ss << actually;
        assert_case.actually = ss.str();
        ss.str("");
        ss << expectedly;
        assert_case.expectedly = ss.str();

        current().cases.push_back(assert_case);
        current().tested += 1;
        current().passed += assert_case.status == Case::Status::PASS ? 1 : 0;
        LDEBUG_F("TEST assert - expected: %s, actually: %s status: %s", assert_case.expectedly.c_str(), assert_case.actually.c_str(), assert_case.status == Case::Status::PASS ? "PASS" : "FAIL");
    }
} // namespace test
