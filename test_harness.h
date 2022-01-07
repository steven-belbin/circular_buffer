#include <format>
#include <functional>
#include <iterator>
#include <iostream>
#include <vector>

/// <summary>
/// I typically don't implement a unit testing framework, but I found
/// myself crafting one for the purposes of unit testing of the
/// circular buffer sample.
/// 
/// I choose this path since I didn't want to introduce dependencies
/// or expect that others would have VCPKG up & running.
/// 
/// Should VCPKG be configured for this Git project, then I'll
/// convert to use https://github.com/boost-ext/ut since it's lightweight
/// and has more features.
/// 
/// This testing harness was good enough for now.
/// </summary>

namespace prgrmr::testing
{

template<typename values_type>
auto run_test_case(const auto number,
                   const auto line,
                   const values_type& values,
                   const values_type& expected)
{
    const auto result{ values == expected };

    std::cout << std::format("#       : {}\n", number)
              << std::format("Line    : {}\n", line)
              << std::format("Result  : {}\n", result ? "PASSED" : "FAILED")
              << std::format("Got     : {}\n", values)
              << std::format("Expected: {}\n", expected)
              << "\n";

    return result;
}

auto run_test_scenario(const auto& scenario)
{
    const auto results { scenario() };

    const auto passed { std::count(std::begin(results), std::end(results), true) };
    const auto failed { std::count(std::begin(results), std::end(results), false) };
    const auto result { passed == results.size() };

    std::cout << std::format("Result  : {}\n", result ? "PASSED" : "FAILED")
              << std::format("Passed  : {}\n", passed)
              << std::format("Failed  : {}\n", failed)
              << "\n\n";

    return result;
}

void run_test_scenarios(const std::initializer_list<std::function<std::vector<bool> ()>>& scenarios)
{
    std::vector<bool> results;

    for (auto scenario : scenarios)
        results.push_back(run_test_scenario(scenario));

    const auto passed { std::count(std::begin(results), std::end(results), true) };
    const auto failed { std::count(std::begin(results), std::end(results), false) };
    const auto result { passed == results.size() };

    std::cout << "=====================================================================\n"
              << "\t\t Scenario summary\n"
              << "=====================================================================\n"
              << std::format("Result  : {}\n", result ? "PASSED" : "FAILED")
              << std::format("Passed  : {}\n", passed)
              << std::format("Failed  : {}\n", failed)
              << "=====================================================================\n"
              << "\n\n";
}

void report_start_test_scenario(const char* scenario_name)
{
    std::cout << "=====================================================================\n"
              << std::format("\tRunning scenario:\t{}\n", scenario_name)
              << "=====================================================================\n";
}
}
