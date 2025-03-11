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

REGISTER_SUITE(BCGeneralStateTestsSuite, BlockchainTestInvalidSuite)
REGISTER_SUITE(BCGeneralStateTestsVMSuite, BCGeneralStateTestsSuite)
REGISTER_SUITE(BCGeneralStateTestsShanghaiSuite, BCGeneralStateTestsSuite)

#define LEGACYFLAG  \
    protected:      \
        bool legacyTestSuiteFlag() const override { return  true; }

REGISTER_SUITE_OVERRIDE(LegacyConstantinopleBCGeneralStateTestsSuite, BlockchainTestValidSuite, LEGACYFLAG)
REGISTER_SUITE_OVERRIDE(LegacyConstantinopleBlockchainInvalidTestSuite, BlockchainTestInvalidSuite, LEGACYFLAG)
REGISTER_SUITE_OVERRIDE(LegacyConstantinopleBlockchainValidTestSuite, BlockchainTestValidSuite, LEGACYFLAG)

REGISTER_SUITE_OVERRIDE(LegacyCancunBCGeneralStateTestsSuite, BlockchainTestInvalidSuite, LEGACYFLAG)
REGISTER_SUITE_OVERRIDE(LegacyCancunBlockchainInvalidTestSuite, BlockchainTestInvalidSuite, LEGACYFLAG)
REGISTER_SUITE_OVERRIDE(LegacyCancunBlockchainValidTestSuite, BlockchainTestValidSuite, LEGACYFLAG)
REGISTER_SUITE_OVERRIDE(LegacyCancunBlockchainTransitionTestSuite, BlockchainTestTransitionSuite, LEGACYFLAG)

}  // test
