/** @file TestFixtures.h
 * Test initialization classes
 */

#pragma once
#include <memory>
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
    std::string{"loopMulFiller"},
    std::string{"test_excess_blob_gas"}
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
class TestFixtureBase
{
public:
    TestFixtureBase() {}
    virtual ~TestFixtureBase() {}
    virtual TestFixtureBase* copy() const = 0;
    virtual std::string folder() const = 0;
    virtual std::string fillerFoler() const = 0;
    virtual void setAdditionalFillerFolder(std::string&& _folder) const = 0;
    virtual void execute() const = 0;
};

template <class T, class U>
class TestFixture : public TestFixtureBase
{
public:
    TestFixture(std::set<TestExecution> const& _execFlags = {});
    TestFixture(int){};
    std::string folder() const override { return m_suite.suiteFolder().path().string(); }
    std::string fillerFoler() const override { return m_suite.suiteFillerFolder().path().string(); }
    void setAdditionalFillerFolder(std::string&& _folder) const override { m_suite.setFillerPathAdd(std::move(_folder)); };
    TestFixtureBase* copy() const override { return new TestFixture<T, U>(*this); }
    void execute() const override { _execute(m_execFlags); }
    ~TestFixture() override {}

private:
    void _execute(std::set<TestExecution> const& _execFlags) const;
    std::set<TestExecution> m_execFlags;
    T m_suite;
};


void DynamicTestsBoost(std::vector<std::string>& allTests);
void DynamicTestsBoostClean();
class FixtureRegistrator
{
public:
    FixtureRegistrator(TestFixtureBase* _fixture, std::string&& _suiteName);
};

#define ETH_REGISTER_DYNAMIC_TEST_SEARCH(F, S) \
    static FixtureRegistrator* dynamicfixture##F = new FixtureRegistrator(new F(true), S);

}  // namespace test
