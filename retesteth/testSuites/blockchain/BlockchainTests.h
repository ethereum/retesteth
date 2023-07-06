/** @file BlockchainTests.h
 * BlockchainTests functions.
 */

#pragma once
#include <libdataobj/DataObject.h>
#include <retesteth/testSuiteRunner/TestSuite.h>
#include <retesteth/testSuites/TestFixtures.h>
#include <boost/filesystem/path.hpp>

namespace test
{
using namespace dataobject;

#define REGISTER_SUITE_OVERRIDE(SUITE, BASE, CODE)      \
    class SUITE : public BASE                           \
    {                                                   \
        CODE                                            \
    public:                                             \
        spDataObject doTests(spDataObject& _input, TestSuiteOptions& _opt) const override; \
        TestPath suiteFolder() const override;          \
        FillerPath suiteFillerFolder() const override;  \
    };

#define REGISTER_SUITE(SUITE, BASE)                     \
    class SUITE : public BASE                           \
    {                                                   \
    public:                                             \
        TestPath suiteFolder() const override;          \
        FillerPath suiteFillerFolder() const override;  \
    };

REGISTER_SUITE_OVERRIDE(BlockchainTestValidSuite, TestSuite,)
REGISTER_SUITE_OVERRIDE(BlockchainTestInvalidSuite, TestSuite,)
REGISTER_SUITE_OVERRIDE(BlockchainTestTransitionSuite, TestSuite,)

REGISTER_SUITE(BlockchainTestEIPSuite, BlockchainTestInvalidSuite)
REGISTER_SUITE(BlockchainTestPyspecSuite, BlockchainTestInvalidSuite)
    REGISTER_SUITE(BlockchainTestPyspecSuite_frontier, BlockchainTestPyspecSuite)
    REGISTER_SUITE(BlockchainTestPyspecSuite_homestead, BlockchainTestPyspecSuite)
    REGISTER_SUITE(BlockchainTestPyspecSuite_istanbul, BlockchainTestPyspecSuite)
    REGISTER_SUITE(BlockchainTestPyspecSuite_berlin, BlockchainTestPyspecSuite)
    REGISTER_SUITE(BlockchainTestPyspecSuite_merge, BlockchainTestPyspecSuite)
    REGISTER_SUITE(BlockchainTestPyspecSuite_shanghai, BlockchainTestPyspecSuite)
    REGISTER_SUITE(BlockchainTestPyspecSuite_cancun, BlockchainTestPyspecSuite)

REGISTER_SUITE(BlockchainTestEIPPyspecSuite, BlockchainTestInvalidSuite)
    REGISTER_SUITE(BlockchainTestEIPPyspecSuite_frontier, BlockchainTestEIPPyspecSuite)
    REGISTER_SUITE(BlockchainTestEIPPyspecSuite_homestead, BlockchainTestEIPPyspecSuite)
    REGISTER_SUITE(BlockchainTestEIPPyspecSuite_istanbul, BlockchainTestEIPPyspecSuite)
    REGISTER_SUITE(BlockchainTestEIPPyspecSuite_berlin, BlockchainTestEIPPyspecSuite)
    REGISTER_SUITE(BlockchainTestEIPPyspecSuite_merge, BlockchainTestEIPPyspecSuite)
    REGISTER_SUITE(BlockchainTestEIPPyspecSuite_shanghai, BlockchainTestEIPPyspecSuite)
    REGISTER_SUITE(BlockchainTestEIPPyspecSuite_cancun, BlockchainTestEIPPyspecSuite)

REGISTER_SUITE(BCGeneralStateTestsSuite, BlockchainTestInvalidSuite)
REGISTER_SUITE(BCGeneralStateTestsVMSuite, BCGeneralStateTestsSuite)
REGISTER_SUITE(BCGeneralStateTestsShanghaiSuite, BCGeneralStateTestsSuite)

REGISTER_SUITE(BCEIPStateTestsSuite, BlockchainTestValidSuite)
REGISTER_SUITE(BCEIPStateTestsEOFSuite, BCEIPStateTestsSuite)

#define LEGACYFLAG  \
    protected:      \
        bool legacyTestSuiteFlag() const override { return  true; }

REGISTER_SUITE_OVERRIDE(LegacyConstantinopleBCGeneralStateTestsSuite, BlockchainTestValidSuite, LEGACYFLAG)
REGISTER_SUITE_OVERRIDE(LegacyConstantinopleBlockchainInvalidTestSuite, BlockchainTestInvalidSuite, LEGACYFLAG)
REGISTER_SUITE_OVERRIDE(LegacyConstantinopleBlockchainValidTestSuite, BlockchainTestValidSuite, LEGACYFLAG)

}  // test
