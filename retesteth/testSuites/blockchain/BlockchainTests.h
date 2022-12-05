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

#define REGISTER_SUITE(SUITE, BASE, CODE)               \
    class SUITE : public BASE                           \
    {                                                   \
        CODE                                            \
    public:                                             \
        spDataObject doTests(spDataObject& _input, TestSuiteOptions& _opt) const override; \
        TestPath suiteFolder() const override;          \
        FillerPath suiteFillerFolder() const override;  \
    };

REGISTER_SUITE(BlockchainTestValidSuite, TestSuite,)
REGISTER_SUITE(BlockchainTestInvalidSuite, TestSuite,)
REGISTER_SUITE(BlockchainTestTransitionSuite, TestSuite,)
REGISTER_SUITE(BlockchainTestEIPSuite, BlockchainTestInvalidSuite,)

REGISTER_SUITE(BCGeneralStateTestsSuite, BlockchainTestValidSuite,)
REGISTER_SUITE(BCGeneralStateTestsVMSuite, BCGeneralStateTestsSuite,)
REGISTER_SUITE(BCGeneralStateTestsShanghaiSuite, BCGeneralStateTestsSuite,)

REGISTER_SUITE(BCEIPStateTestsSuite, BlockchainTestValidSuite,)
REGISTER_SUITE(BCEIPStateTestsEOFSuite, BCEIPStateTestsSuite,)

#define LEGACYFLAG  \
    protected:      \
        bool legacyTestSuiteFlag() const override { return  true; }

REGISTER_SUITE(LegacyConstantinopleBCGeneralStateTestsSuite, BlockchainTestValidSuite, LEGACYFLAG)
REGISTER_SUITE(LegacyConstantinopleBlockchainInvalidTestSuite, BlockchainTestInvalidSuite, LEGACYFLAG)
REGISTER_SUITE(LegacyConstantinopleBlockchainValidTestSuite, BlockchainTestValidSuite, LEGACYFLAG)

}  // test
