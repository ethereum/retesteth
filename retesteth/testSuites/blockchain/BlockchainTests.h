/** @file BlockchainTests.h
 * BlockchainTests functions.
 */

#pragma once
#include <dataObject/DataObject.h>
#include <retesteth/Options.h>
#include <retesteth/TestHelper.h>
#include <retesteth/TestSuite.h>
#include <boost/filesystem/path.hpp>

namespace test
{
enum class TestExecution
{
    RequireOptionAll,
    NotRefillable
};

static std::vector<std::string> const c_timeConsumingTestSuites{std::string{"stTimeConsuming"},
    std::string{"stQuadraticComplexityTest"}, std::string{"bcExploitTest"},
    std::string{"bcWalletTest"}, std::string{"stQuadraticComplexityTest"}};

/// Suite run and check blockchain tests with valid blocks only
class BlockchainTestValidSuite : public TestSuite
{
public:
    DataObject doTests(DataObject const& _input, TestSuiteOptions& _opt) const override;
    TestPath suiteFolder() const override;
    FillerPath suiteFillerFolder() const override;
};

/// Suite run/check generate blockchain tests that has malicious blocks
class BlockchainTestInvalidSuite : public TestSuite
{
public:
    DataObject doTests(DataObject const& _input, TestSuiteOptions& _opt) const override;
    TestPath suiteFolder() const override;
    FillerPath suiteFillerFolder() const override;
};

/// Suite run/check blockchain tests with fork transition configurations
class BlockchainTestTransitionSuite : public TestSuite
{
public:
    DataObject doTests(DataObject const& _input, TestSuiteOptions& _opt) const override;
    TestPath suiteFolder() const override;
    FillerPath suiteFillerFolder() const override;
};

/// Suite run/check stateTests converted into blockchain by testeth
class BCGeneralStateTestsSuite : public BlockchainTestValidSuite
{
    test::TestSuite::TestPath suiteFolder() const override;
    test::TestSuite::FillerPath suiteFillerFolder() const override;
};


/// Suite run/check stateTests converted into blockchain by testeth
class LegacyConstantinopleBCGeneralStateTestsSuite : public BlockchainTestValidSuite
{
    DataObject doTests(DataObject const& _input, TestSuiteOptions& _opt) const override;
    test::TestSuite::TestPath suiteFolder() const override;
    test::TestSuite::FillerPath suiteFillerFolder() const override;
};


template <class T>
class bcTestFixture
{
public:
    bcTestFixture(std::set<TestExecution> const& _execFlags = {})
    {
        T suite;
        if (_execFlags.count(TestExecution::NotRefillable) &&
            (Options::get().fillchain || Options::get().filltests))
            ETH_ERROR_MESSAGE("Tests are sealed and not refillable!");

        string const casename = boost::unit_test::framework::current_test_case().p_name;
        boost::filesystem::path const suiteFillerPath =
            suite.getFullPathFiller(casename).parent_path();

        // skip wallet test as it takes too much time (250 blocks) run it with --all flag
        if (inArray(c_timeConsumingTestSuites, casename) && !test::Options::get().all)
        {
            std::cout << "Skipping " << casename << " because --all option is not specified.\n";
            test::TestOutputHelper::get().markTestFolderAsFinished(suiteFillerPath, casename);
            return;
        }

        suite.runAllTestsInFolder(casename);
        test::TestOutputHelper::get().markTestFolderAsFinished(suiteFillerPath, casename);
    }
};


class LegacyConstantinopleBlockchainInvalidTestSuite : public BlockchainTestInvalidSuite
{
    DataObject doTests(DataObject const& _input, TestSuiteOptions& _opt) const override;
    TestPath suiteFolder() const override
    {
        return TestSuite::TestPath(
            fs::path("LegacyTests/Constantinople/BlockchainTests/InvalidBlocks"));
    }
    FillerPath suiteFillerFolder() const override
    {
        return TestSuite::FillerPath(
            fs::path("/src/LegacyTests/Constantinople/BlockchainTestsFiller/InvalidBlocks"));
    }
};

class LegacyConstantinopleBlockchainValidTestSuite : public BlockchainTestValidSuite
{
    DataObject doTests(DataObject const& _input, TestSuiteOptions& _opt) const override;
    TestPath suiteFolder() const override
    {
        return TestSuite::TestPath(
            fs::path("LegacyTests/Constantinople/BlockchainTests/ValidBlocks"));
    }
    FillerPath suiteFillerFolder() const override
    {
        return TestSuite::FillerPath(
            fs::path("/src/LegacyTests/Constantinople/BlockchainTestsFiller/ValidBlocks"));
    }
};

// Fixture for BlockchainTests/InvalidBlocks
class BlockchainTestInvalidFixture
  : public bcTestFixture<BlockchainTestInvalidSuite>
{
public:
    BlockchainTestInvalidFixture() : bcTestFixture() {}
};


// Fixture for BlockchainTests/ValidBlocks
class BlockchainTestValidFixture
  : public bcTestFixture<BlockchainTestValidSuite>
{
public:
    BlockchainTestValidFixture() : bcTestFixture() {}
};


// Fixture for Legacy/BCGeneralStateTest
class LegacyConstantinopleBCGeneralStateTestFixture
{
public:
    LegacyConstantinopleBCGeneralStateTestFixture();
};

// Fixture for Legacy/BlockchainTests/InvalidBlocks
class LegacyConstantinoplebcInvalidTestFixture
  : public bcTestFixture<LegacyConstantinopleBlockchainInvalidTestSuite>
{
public:
    LegacyConstantinoplebcInvalidTestFixture() : bcTestFixture({TestExecution::NotRefillable}) {}
};

// Fixture for Legacy/BlockchainTests/ValidBlocks
class LegacyConstantinoplebcValidTestFixture
  : public bcTestFixture<LegacyConstantinopleBlockchainValidTestSuite>
{
public:
    LegacyConstantinoplebcValidTestFixture() : bcTestFixture({TestExecution::NotRefillable}) {}
};


}  // test
