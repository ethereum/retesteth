/** @file BlockchainTests.h
 * BlockchainTests functions.
 */

#pragma once
#include <dataObject/DataObject.h>
#include <retesteth/Options.h>
#include <retesteth/TestHelper.h>
#include <retesteth/TestSuite.h>
#include <retesteth/testSuites/TestFixtures.h>
#include <boost/filesystem/path.hpp>

namespace test
{

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


// Fixture for Legacy/BCGeneralStateTest
class LegacyConstantinopleBCGeneralStateTestFixture
  : public TestFixture<LegacyConstantinopleBCGeneralStateTestsSuite>
{
public:
    LegacyConstantinopleBCGeneralStateTestFixture() : TestFixture({TestExecution::NotRefillable}) {}
};

// Fixture for Legacy/BlockchainTests/InvalidBlocks
class LegacyConstantinoplebcInvalidTestFixture
  : public TestFixture<LegacyConstantinopleBlockchainInvalidTestSuite>
{
public:
    LegacyConstantinoplebcInvalidTestFixture() : TestFixture({TestExecution::NotRefillable}) {}
};

// Fixture for Legacy/BlockchainTests/ValidBlocks
class LegacyConstantinoplebcValidTestFixture
  : public TestFixture<LegacyConstantinopleBlockchainValidTestSuite>
{
public:
    LegacyConstantinoplebcValidTestFixture() : TestFixture({TestExecution::NotRefillable}) {}
};


}  // test
