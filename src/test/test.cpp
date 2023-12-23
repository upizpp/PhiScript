#include "test.hpp"

namespace test
{
    void Test::GroupBegin(const string &name, bool print_detail)
    {
        Group group;
        group.name = name;
        group.print_detail = print_detail;
        m_groups.push(group);
        LDEBUG_F("TEST group begins. (%s)", name.c_str());
    }

    void Test::GroupEnd()
    {
        LDEBUG_F("TEST group ends. (%s)", current().name.c_str());
        SummarizeGroup(current());
        m_groups.pop();
    }

    void Test::SummarizeGroup(const Group &group)
    {
        cout << "================================================================" << endl;
        string status = group.tested == group.passed ? COLOR(C_GREEN, "PASS") : COLOR(C_RED, "FAIL");
        cout << COLOR(C_YELLOW, "[" + group.name + " ") << status << COLOR(C_YELLOW, "]") << endl;

        if (group.print_detail)
        {
            for (size_t i = 0; i < group.cases.size(); i++)
            {
                const Case &assert_case = group.cases[i];
                string status = assert_case.status == Case::Status::PASS ? COLOR(C_GREEN, "PASS") : COLOR(C_RED, "FAIL");
                cout << COLOR(C_BLUE, "[Test" + std::to_string(i) + " " + status + "]") << " " << assert_case.detail;
                if (assert_case.status == Case::Status::PASS)
                {
                    cout << "\tvalue: " << assert_case.actually << endl;
                }
                else
                {
                    cout << endl
                         << "\texpectedly: " << assert_case.expectedly << ", actually: " << assert_case.actually << endl;
                }
            }
        }

        cout << COLOR({C_AQUA, C_BOLD}, group.tested) << " tested, " << COLOR({C_AQUA, C_BOLD}, group.passed) << " passed." << endl;
        cout << "================================================================" << endl
             << endl;
    }
} // namespace test