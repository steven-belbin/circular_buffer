#include "circular_buffer.h"
#include "formatter_vector.h"
#include "test_harness.h"
#include <iostream>
#include <vector>

namespace
{
using namespace prgrmr::testing;

auto when_empty() -> std::vector<bool>
{
    report_start_test_scenario(__func__);

    prgrmr::generic::circular_buffer<int, 10> values;

    auto test_case_number { 0 };

    return
    {
        run_test_case(++test_case_number, __LINE__, values.latest(0),    {}),
        run_test_case(++test_case_number, __LINE__, values.latest(5),    {}),
        run_test_case(++test_case_number, __LINE__, values.latest(10),   {}),
        run_test_case(++test_case_number, __LINE__, values.latest(20),   {}),
        run_test_case(++test_case_number, __LINE__, values.earliest(0),  {}),
        run_test_case(++test_case_number, __LINE__, values.earliest(5),  {}),
        run_test_case(++test_case_number, __LINE__, values.earliest(10), {}),
        run_test_case(++test_case_number, __LINE__, values.earliest(20), {})
    };
}

auto when_partially_filled() -> std::vector<bool>
{
    report_start_test_scenario(__func__);

    prgrmr::generic::circular_buffer<int, 10> values;

    values.push_back(30);
    values.push_back(25);
    values.push_back(20);
    values.push_back(15);
    values.push_back(10);
    values.push_back(5);
    values.push_back(0);

    auto test_case_number{ 0 };

    return 
    {
        run_test_case(++test_case_number, __LINE__, values.latest(0),    {}),
        run_test_case(++test_case_number, __LINE__, values.latest(3),    { 0, 5, 10 }),
        run_test_case(++test_case_number, __LINE__, values.latest(10),   { 0, 5, 10, 15, 20, 25, 30 }),
        run_test_case(++test_case_number, __LINE__, values.latest(20),   { 0, 5, 10, 15, 20, 25, 30 }),
        run_test_case(++test_case_number, __LINE__, values.earliest(0),  {}),
        run_test_case(++test_case_number, __LINE__, values.earliest(3),  { 30, 25, 20 }),
        run_test_case(++test_case_number, __LINE__, values.earliest(10), { 30, 25, 20, 15, 10, 5, 0 }),
        run_test_case(++test_case_number, __LINE__, values.earliest(20), { 30, 25, 20, 15, 10, 5, 0 })
    };
}

auto when_completely_filled() -> std::vector<bool>
{
    report_start_test_scenario(__func__);

    prgrmr::generic::circular_buffer<int, 10> values;

    values.push_back(30);
    values.push_back(25);
    values.push_back(20);
    values.push_back(15);
    values.push_back(10);
    values.push_back(5);
    values.push_back(0);
    values.push_back(100);
    values.push_back(101);
    values.push_back(102);
    values.push_back(103);
    values.push_back(104);
    values.push_back(105);

    auto test_case_number{ 0 };

    return
    {
        run_test_case(++test_case_number, __LINE__, values.latest(0),    {}),
        run_test_case(++test_case_number, __LINE__, values.latest(3),    { 105, 104, 103 }),
        run_test_case(++test_case_number, __LINE__, values.latest(10),   { 105, 104, 103, 102, 101, 100, 0 , 5, 10, 15 }),
        run_test_case(++test_case_number, __LINE__, values.latest(20),   { 105, 104, 103, 102, 101, 100, 0 , 5, 10, 15 }),
        run_test_case(++test_case_number, __LINE__, values.earliest(0),  {}),
        run_test_case(++test_case_number, __LINE__, values.earliest(3),  { 15, 10, 5 }),
        run_test_case(++test_case_number, __LINE__, values.earliest(10), { 15, 10, 5, 0, 100, 101, 102, 103, 104, 105 }),
        run_test_case(++test_case_number, __LINE__, values.earliest(20), { 15, 10, 5, 0, 100, 101, 102, 103, 104, 105 })
    };
}
}

int main()
{
    using namespace prgrmr::testing;

    run_test_scenarios(
        {
            when_empty,
            when_partially_filled,
            when_completely_filled
        }
    );
}
