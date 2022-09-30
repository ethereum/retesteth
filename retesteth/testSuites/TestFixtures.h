/** @file TestFixtures.h
 * Test initialization classes
 */

#pragma once
#include <set>
#include <string>
#include <vector>

namespace test
{

static std::vector<std::string> const c_timeConsumingTestSuites{
    std::string{"stTimeConsuming"},
    std::string{"stQuadraticComplexityTest"},
    std::string{"bcExploitTest"},
    std::string{"bcExpectSection"},
    std::string{"bcWalletTest"},
    std::string{"stQuadraticComplexityTest"}
};

static std::vector<std::string> const c_cpuIntenseTests{
    std::string{"CALLBlake2f_MaxRoundsFiller"},
    std::string{"loopMulFiller"}
};

class TestChecker
{
public:
    static bool isCPUIntenseTest(std::string const& _testSuite);
    static bool isTimeConsumingTest(std::string const& _testName);
};

enum class TestExecution
{
    RequireOptionFill,
    RequireOptionAll,
    NotRefillable
};

class NotRefillable
{
public:
    NotRefillable() {}
    std::set<TestExecution> getFlags() { return {TestExecution::NotRefillable}; }
};

class RequireOptionAll
{
public:
    RequireOptionAll() {}
    std::set<TestExecution> getFlags() { return {TestExecution::RequireOptionAll}; }
};

class RequireOptionAllNotRefillable
{
public:
    RequireOptionAllNotRefillable() {}
    std::set<TestExecution> getFlags() {
        return {TestExecution::RequireOptionAll, TestExecution::NotRefillable};
    }
};

class RequireOptionFill
{
public:
    RequireOptionFill() {}
    std::set<TestExecution> getFlags() { return {TestExecution::RequireOptionFill}; }
};

class DefaultFlags
{
public:
    DefaultFlags() {}
    std::set<TestExecution> getFlags() { return std::set<TestExecution>(); }
};

// what if U has the information about flags
template <class T, class U>
class TestFixture
{
public:
    TestFixture(std::set<TestExecution> const& _execFlags = {});
};


}  // namespace test
