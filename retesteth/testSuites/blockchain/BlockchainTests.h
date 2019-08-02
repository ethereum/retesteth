/** @file BlockchainTests.h
 * BlockchainTests functions.
 */

#pragma once
#include <dataObject/DataObject.h>
#include <retesteth/TestSuite.h>
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
}
