/*
This file is part of cpp-ethereum.

cpp-ethereum is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

cpp-ethereum is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with cpp-ethereum.  If not, see <http://www.gnu.org/licenses/>.
*/
/** @file BlockchainTests.cpp
 * BlockchainTests functions.
 */

#include "BlockchainTests.h"
#include "BlockchainTestLogic.h"
#include <retesteth/TestOutputHelper.h>

using namespace std;
using namespace test;
namespace fs = boost::filesystem;

namespace test
{
/// !!! DataObject return without reference!!! must be SP!!!
spDataObject BlockchainTestTransitionSuite::doTests(spDataObject& _input, TestSuiteOptions& _opt) const
{
    _opt.allowInvalidBlocks = true;
    return DoTests(_input, _opt);
}

spDataObject BlockchainTestInvalidSuite::doTests(spDataObject& _input, TestSuiteOptions& _opt) const
{
    _opt.allowInvalidBlocks = true;
    return DoTests(_input, _opt);
}

spDataObject BlockchainTestValidSuite::doTests(spDataObject& _input, TestSuiteOptions& _opt) const
{
    _opt.allowInvalidBlocks = false;
    return DoTests(_input, _opt);
}

spDataObject LegacyConstantinopleBlockchainInvalidTestSuite::doTests(spDataObject& _input, TestSuiteOptions& _opt) const
{
    _opt.allowInvalidBlocks = true;
    _opt.isLegacyTests = true;
    return DoTests(_input, _opt);
}

spDataObject LegacyConstantinopleBlockchainValidTestSuite::doTests(spDataObject& _input, TestSuiteOptions& _opt) const
{
    _opt.allowInvalidBlocks = false;
    _opt.isLegacyTests = true;
    return DoTests(_input, _opt);
}

spDataObject LegacyConstantinopleBCGeneralStateTestsSuite::doTests(spDataObject& _input, TestSuiteOptions& _opt) const
{
    _opt.isLegacyTests = true;
    return DoTests(_input, _opt);
}

BCGeneralStateTestsSuite::BCGeneralStateTestsSuite()
{
    TestInfo errorInfo("Initialized ", "BCGeneralStateTestsSuite");
    TestOutputHelper::get().setCurrentTestInfo(errorInfo);
}

TestSuite::TestPath BlockchainTestValidSuite::suiteFolder() const
{
    return TestSuite::TestPath(fs::path("BlockchainTests") / "ValidBlocks");
}

TestSuite::FillerPath BlockchainTestValidSuite::suiteFillerFolder() const
{
    return TestSuite::FillerPath(fs::path("src") / "BlockchainTestsFiller" / "ValidBlocks");
}

TestSuite::TestPath BlockchainTestInvalidSuite::suiteFolder() const
{
    return TestSuite::TestPath(fs::path("BlockchainTests") / "InvalidBlocks");
}

TestSuite::FillerPath BlockchainTestInvalidSuite::suiteFillerFolder() const
{
    return TestSuite::FillerPath(fs::path("src") / "BlockchainTestsFiller" / "InvalidBlocks");
}

TestSuite::TestPath BlockchainTestTransitionSuite::suiteFolder() const
{
    return TestSuite::TestPath(fs::path("BlockchainTests") / "TransitionTests");
}

TestSuite::FillerPath BlockchainTestTransitionSuite::suiteFillerFolder() const
{
    return TestSuite::FillerPath(fs::path("src") / "BlockchainTestsFiller" / "TransitionTests");
}

TestSuite::TestPath BCGeneralStateTestsSuite::suiteFolder() const
{
    return TestSuite::TestPath(fs::path("BlockchainTests") / "GeneralStateTests");
}

TestSuite::FillerPath BCGeneralStateTestsSuite::suiteFillerFolder() const
{
    return TestSuite::FillerPath(fs::path("src") / fs::path("GeneralStateTestsFiller"));
}

BCGeneralStateTestsVMSuite::BCGeneralStateTestsVMSuite()
{
    test::TestOutputHelper::get().markTestFolderAsFinished(getFullPathFiller("VMTests").parent_path().parent_path(), "VMTests");
}

TestSuite::TestPath BCGeneralStateTestsVMSuite::suiteFolder() const
{
    return TestSuite::TestPath(fs::path("BlockchainTests") / "GeneralStateTests" / "VMTests");
}

TestSuite::FillerPath BCGeneralStateTestsVMSuite::suiteFillerFolder() const
{
    return TestSuite::FillerPath(fs::path("src") / fs::path("GeneralStateTestsFiller") / "VMTests");
}

TestSuite::TestPath LegacyConstantinopleBCGeneralStateTestsSuite::suiteFolder() const
{
    return TestSuite::TestPath(
        fs::path("LegacyTests") / "Constantinople" / "BlockchainTests" / "GeneralStateTests");
}

TestSuite::FillerPath LegacyConstantinopleBCGeneralStateTestsSuite::suiteFillerFolder() const
{
    return TestSuite::FillerPath(fs::path("src") / "LegacyTests" / "Constantinople" /
                                 "BlockchainTestsFiller" / "GeneralStateTests");
}

}  // Namespace Close


BOOST_AUTO_TEST_SUITE(BlockchainTests)

// Tests that contain only valid blocks and check that import is correct
using BCValidSuiteFixture = TestFixture<BlockchainTestValidSuite, DefaultFlags>;
BOOST_FIXTURE_TEST_SUITE(ValidBlocks, BCValidSuiteFixture)
BOOST_AUTO_TEST_CASE(bcBlockGasLimitTest) {}
BOOST_AUTO_TEST_CASE(bcExploitTest) {}
BOOST_AUTO_TEST_CASE(bcForkStressTest) {}
BOOST_AUTO_TEST_CASE(bcGasPricerTest) {}
BOOST_AUTO_TEST_CASE(bcMultiChainTest) {}
BOOST_AUTO_TEST_CASE(bcRandomBlockhashTest) {}
BOOST_AUTO_TEST_CASE(bcStateTests) {}
BOOST_AUTO_TEST_CASE(bcTotalDifficultyTest) {}
BOOST_AUTO_TEST_CASE(bcUncleSpecialTests) {}
BOOST_AUTO_TEST_CASE(bcUncleTest) {}
BOOST_AUTO_TEST_CASE(bcValidBlockTest) {}
BOOST_AUTO_TEST_CASE(bcWalletTest) {}
BOOST_AUTO_TEST_CASE(bcExample) {}
BOOST_AUTO_TEST_CASE(bcEIP1559) {}
BOOST_AUTO_TEST_CASE(bcEIP3675) {}

BOOST_AUTO_TEST_SUITE_END() // ValidBlocks

// Tests that might have invalid blocks and check that those are rejected
using BCInValidSuiteFixture = TestFixture<BlockchainTestInvalidSuite, DefaultFlags>;
BOOST_FIXTURE_TEST_SUITE(InvalidBlocks, BCInValidSuiteFixture)
BOOST_AUTO_TEST_CASE(bcBlockGasLimitTest) {}
BOOST_AUTO_TEST_CASE(bcForgedTest) {}
BOOST_AUTO_TEST_CASE(bcInvalidHeaderTest) {}
BOOST_AUTO_TEST_CASE(bcMultiChainTest) {}
BOOST_AUTO_TEST_CASE(bcUncleHeaderValidity) {}
BOOST_AUTO_TEST_CASE(bcUncleSpecialTests) {}
BOOST_AUTO_TEST_CASE(bcUncleTest) {}
BOOST_AUTO_TEST_CASE(bcExample) {}
BOOST_AUTO_TEST_CASE(bcEIP1559) {}
BOOST_AUTO_TEST_CASE(bcEIP3675) {}
BOOST_AUTO_TEST_SUITE_END()

using BCInValidSuiteFixture2 = TestFixture<BlockchainTestInvalidSuite, RequireOptionFill>;
BOOST_FIXTURE_TEST_SUITE(Retesteth, BCInValidSuiteFixture2)
BOOST_AUTO_TEST_CASE(bcExpectSection) {}
BOOST_AUTO_TEST_SUITE_END()

// Transition from fork to fork tests
using BCTransitionFixture = TestFixture<BlockchainTestTransitionSuite, DefaultFlags>;
BOOST_FIXTURE_TEST_SUITE(TransitionTests, BCTransitionFixture)
BOOST_AUTO_TEST_CASE(bcByzantiumToConstantinopleFix) {}
BOOST_AUTO_TEST_CASE(bcEIP158ToByzantium) {}
BOOST_AUTO_TEST_CASE(bcFrontierToHomestead) {}
BOOST_AUTO_TEST_CASE(bcHomesteadToDao) {}
BOOST_AUTO_TEST_CASE(bcHomesteadToEIP150) {}
BOOST_AUTO_TEST_CASE(bcBerlinToLondon) {}
BOOST_AUTO_TEST_CASE(bcArrowGlacierToMerge) {}
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE_END()

// General tests in form of blockchain tests
using BCGeneralsStateSuiteFixture = TestFixture<BCGeneralStateTestsSuite, RequireOptionAllNotRefillable>;
BOOST_FIXTURE_TEST_SUITE(BCGeneralStateTests, BCGeneralsStateSuiteFixture)

// Frontier Tests
BOOST_AUTO_TEST_CASE(stCallCodes) {}
BOOST_AUTO_TEST_CASE(stCallCreateCallCodeTest) {}
BOOST_AUTO_TEST_CASE(stExample) {}
BOOST_AUTO_TEST_CASE(stInitCodeTest) {}
BOOST_AUTO_TEST_CASE(stLogTests) {}
BOOST_AUTO_TEST_CASE(stMemoryTest) {}
BOOST_AUTO_TEST_CASE(stPreCompiledContracts) {}
BOOST_AUTO_TEST_CASE(stPreCompiledContracts2) {}
BOOST_AUTO_TEST_CASE(stRandom) {}
BOOST_AUTO_TEST_CASE(stRandom2) {}
BOOST_AUTO_TEST_CASE(stRecursiveCreate) {}
BOOST_AUTO_TEST_CASE(stRefundTest) {}
BOOST_AUTO_TEST_CASE(stSolidityTest) {}
BOOST_AUTO_TEST_CASE(stSpecialTest) {}
BOOST_AUTO_TEST_CASE(stSystemOperationsTest) {}
BOOST_AUTO_TEST_CASE(stTransactionTest) {}
BOOST_AUTO_TEST_CASE(stTransitionTest) {}
BOOST_AUTO_TEST_CASE(stWalletTest) {}

// Homestead Tests
BOOST_AUTO_TEST_CASE(stCallDelegateCodesCallCodeHomestead) {}
BOOST_AUTO_TEST_CASE(stCallDelegateCodesHomestead) {}
BOOST_AUTO_TEST_CASE(stHomesteadSpecific) {}
BOOST_AUTO_TEST_CASE(stDelegatecallTestHomestead) {}

// EIP150 Tests
BOOST_AUTO_TEST_CASE(stEIP150singleCodeGasPrices) {}
BOOST_AUTO_TEST_CASE(stMemExpandingEIP150Calls) {}
BOOST_AUTO_TEST_CASE(stEIP150Specific) {}

// EIP158 Tests
BOOST_AUTO_TEST_CASE(stEIP158Specific) {}
BOOST_AUTO_TEST_CASE(stNonZeroCallsTest) {}
BOOST_AUTO_TEST_CASE(stZeroCallsTest) {}
BOOST_AUTO_TEST_CASE(stZeroCallsRevert) {}
BOOST_AUTO_TEST_CASE(stCodeSizeLimit) {}
BOOST_AUTO_TEST_CASE(stCreateTest) {}
BOOST_AUTO_TEST_CASE(stRevertTest) {}

// Metropolis Tests
BOOST_AUTO_TEST_CASE(stStackTests) {}
BOOST_AUTO_TEST_CASE(stStaticCall) {}
BOOST_AUTO_TEST_CASE(stReturnDataTest) {}
BOOST_AUTO_TEST_CASE(stZeroKnowledge) {}
BOOST_AUTO_TEST_CASE(stZeroKnowledge2) {}
BOOST_AUTO_TEST_CASE(stCodeCopyTest) {}
BOOST_AUTO_TEST_CASE(stBugs) {}

// Constantinople Tests
BOOST_AUTO_TEST_CASE(stShift) {}
BOOST_AUTO_TEST_CASE(stCreate2) {}
BOOST_AUTO_TEST_CASE(stExtCodeHash) {}
BOOST_AUTO_TEST_CASE(stSStoreTest) {}

// Stress Tests
BOOST_AUTO_TEST_CASE(stAttackTest) {}
BOOST_AUTO_TEST_CASE(stMemoryStressTest) {}
BOOST_AUTO_TEST_CASE(stQuadraticComplexityTest) {}

// Bad opcodes test
BOOST_AUTO_TEST_CASE(stBadOpcode) {}

// Recent Tests
BOOST_AUTO_TEST_CASE(stArgsZeroOneBalance) {}
BOOST_AUTO_TEST_CASE(stChainId) {}
BOOST_AUTO_TEST_CASE(stSLoadTest) {}
BOOST_AUTO_TEST_CASE(stSelfBalance) {}
BOOST_AUTO_TEST_CASE(stStaticFlagEnabled) {}
BOOST_AUTO_TEST_CASE(stSubroutine) {}
BOOST_AUTO_TEST_CASE(stEIP2537) {}
BOOST_AUTO_TEST_CASE(stEIP2930) {}
BOOST_AUTO_TEST_CASE(stEIP1559) {}
BOOST_AUTO_TEST_CASE(stEIP3607) {}

// Heavy
BOOST_AUTO_TEST_CASE(stTimeConsuming) {}

// Converted VMTests
using BCGeneralStateTestsVMFixture = TestFixture<BCGeneralStateTestsVMSuite, RequireOptionAll>;
BOOST_FIXTURE_TEST_SUITE(VMTests, BCGeneralStateTestsVMFixture)
BOOST_AUTO_TEST_CASE(vmArithmeticTest) {}
BOOST_AUTO_TEST_CASE(vmBitwiseLogicOperation) {}
BOOST_AUTO_TEST_CASE(vmIOandFlowOperations) {}
BOOST_AUTO_TEST_CASE(vmLogTest) {}
BOOST_AUTO_TEST_CASE(vmPerformance) {}
BOOST_AUTO_TEST_CASE(vmTests) {}
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE_END()
