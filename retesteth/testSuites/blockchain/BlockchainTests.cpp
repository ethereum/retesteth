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
#include <retesteth/helpers/TestOutputHelper.h>

using namespace std;
using namespace test;
namespace fs = boost::filesystem;

namespace test
{

#define BLOCKCHAINSUITE_FOLDER_OVERRIDE(SUITE, FOLDER, FILLER)   \
    TestSuite::TestPath SUITE::suiteFolder() const       \
    {                                                    \
        return TestSuite::TestPath(fs::path(string("BlockchainTests" + string(FOLDER) + m_fillerPathAdd))); \
    }                                                    \
                                                         \
    TestSuite::FillerPath SUITE::suiteFillerFolder() const   \
    {                                                    \
        return TestSuite::FillerPath(fs::path(string("src" + string(FILLER) + m_fillerPathAdd)));  \
    }

BLOCKCHAINSUITE_FOLDER_OVERRIDE(BCGeneralStateTestsSuite, "/GeneralStateTests", "/GeneralStateTestsFiller")
BLOCKCHAINSUITE_FOLDER_OVERRIDE(BCGeneralStateTestsVMSuite, "/GeneralStateTests/VMTests", "/GeneralStateTestsFiller/VMTests")
BLOCKCHAINSUITE_FOLDER_OVERRIDE(BCGeneralStateTestsShanghaiSuite, "/GeneralStateTests/Shanghai", "/GeneralStateTestsFiller/Shanghai")

BLOCKCHAINSUITE_FOLDER_OVERRIDE(BlockchainTestTransitionSuite, "/TransitionTests", "/BlockchainTestsFiller/TransitionTests")
BLOCKCHAINSUITE_FOLDER_OVERRIDE(BlockchainTestInvalidSuite, "/InvalidBlocks", "/BlockchainTestsFiller/InvalidBlocks")
BLOCKCHAINSUITE_FOLDER_OVERRIDE(BlockchainTestValidSuite, "/ValidBlocks", "/BlockchainTestsFiller/ValidBlocks")

BLOCKCHAINSUITE_FOLDER_OVERRIDE(BlockchainTestPyspecSuite, "/Pyspecs", "/BlockchainTestsFiller/Pyspecs/")
BLOCKCHAINSUITE_FOLDER_OVERRIDE(BlockchainTestPyspecSuite_frontier, "/Pyspecs/frontier", "/BlockchainTestsFiller/Pyspecs/frontier")
BLOCKCHAINSUITE_FOLDER_OVERRIDE(BlockchainTestPyspecSuite_homestead, "/Pyspecs/homestead", "/BlockchainTestsFiller/Pyspecs/homestead")
BLOCKCHAINSUITE_FOLDER_OVERRIDE(BlockchainTestPyspecSuite_istanbul, "/Pyspecs/istanbul", "/BlockchainTestsFiller/Pyspecs/istanbul")
BLOCKCHAINSUITE_FOLDER_OVERRIDE(BlockchainTestPyspecSuite_berlin, "/Pyspecs/berlin", "/BlockchainTestsFiller/Pyspecs/berlin")
BLOCKCHAINSUITE_FOLDER_OVERRIDE(BlockchainTestPyspecSuite_merge, "/Pyspecs/merge", "/BlockchainTestsFiller/Pyspecs/merge")
BLOCKCHAINSUITE_FOLDER_OVERRIDE(BlockchainTestPyspecSuite_shanghai, "/Pyspecs/shanghai", "/BlockchainTestsFiller/Pyspecs/shanghai")
BLOCKCHAINSUITE_FOLDER_OVERRIDE(BlockchainTestPyspecSuite_cancun, "/Pyspecs/cancun", "/BlockchainTestsFiller/Pyspecs/cancun")


#define LEGACY_BLOCKCHAINSUITE_FOLDER_OVERRIDE(SUITE, FOLDER, FILLER)   \
    TestSuite::TestPath SUITE::suiteFolder() const       \
    {                                                    \
        return TestSuite::TestPath(fs::path(string("LegacyTests" + string(FOLDER)))); \
    }                                                    \
                                                         \
    TestSuite::FillerPath SUITE::suiteFillerFolder() const   \
    {                                                    \
        return TestSuite::FillerPath(fs::path(string("src/LegacyTests" + string(FILLER))));  \
    }

LEGACY_BLOCKCHAINSUITE_FOLDER_OVERRIDE(LegacyConstantinopleBCGeneralStateTestsSuite,
    "/Constantinople/BlockchainTests/GeneralStateTests",
    "/Constantinople/BlockchainTestsFiller/GeneralStateTests")
LEGACY_BLOCKCHAINSUITE_FOLDER_OVERRIDE(LegacyConstantinopleBlockchainInvalidTestSuite,
    "/Constantinople/BlockchainTests/InvalidBlocks",
    "/Constantinople/BlockchainTestsFiller/InvalidBlocks")
LEGACY_BLOCKCHAINSUITE_FOLDER_OVERRIDE(LegacyConstantinopleBlockchainValidTestSuite,
    "/Constantinople/BlockchainTests/ValidBlocks",
    "/Constantinople/BlockchainTestsFiller/ValidBlocks")


#define BLOCKCHAINSUITE_DOTESTS_OVERRIDE(SUITE, FUNC)   \
    spDataObject SUITE::doTests(spDataObject& _input, TestSuiteOptions& _opt) const \
    {                                                   \
        FUNC                                            \
        return DoTests(_input, _opt);                   \
    }

BLOCKCHAINSUITE_DOTESTS_OVERRIDE(BlockchainTestTransitionSuite, _opt.allowInvalidBlocks = true;)
BLOCKCHAINSUITE_DOTESTS_OVERRIDE(BlockchainTestInvalidSuite, _opt.allowInvalidBlocks = true;)
BLOCKCHAINSUITE_DOTESTS_OVERRIDE(BlockchainTestValidSuite, _opt.allowInvalidBlocks = false;)
BLOCKCHAINSUITE_DOTESTS_OVERRIDE(LegacyConstantinopleBlockchainInvalidTestSuite,
                                 _opt.allowInvalidBlocks = true;
                                 _opt.isLegacyTests = true;)
BLOCKCHAINSUITE_DOTESTS_OVERRIDE(LegacyConstantinopleBlockchainValidTestSuite,
                                 _opt.allowInvalidBlocks = false;
                                 _opt.isLegacyTests = true;)
BLOCKCHAINSUITE_DOTESTS_OVERRIDE(LegacyConstantinopleBCGeneralStateTestsSuite, _opt.isLegacyTests = true;)

}  // Namespace Close


BOOST_AUTO_TEST_SUITE(BlockchainTests)

using BCPyspecSuiteFixture = TestFixture<BlockchainTestPyspecSuite, DefaultFlags>;
//ETH_REGISTER_DYNAMIC_TEST_SEARCH(BCPyspecSuiteFixture, "BlockchainTests/Pyspecs")
BOOST_FIXTURE_TEST_SUITE(Pyspecs, BCPyspecSuiteFixture)
    using BCPyspecSuiteFixture_frontier = TestFixture<BlockchainTestPyspecSuite_frontier, DefaultFlags>;
    ETH_REGISTER_DYNAMIC_TEST_SEARCH(BCPyspecSuiteFixture_frontier, "BlockchainTests/Pyspecs/frontier")
    BOOST_FIXTURE_TEST_SUITE(frontier, BCPyspecSuiteFixture_frontier)
    BOOST_AUTO_TEST_SUITE_END()
    using BCPyspecSuiteFixture_homestead = TestFixture<BlockchainTestPyspecSuite_homestead, DefaultFlags>;
    ETH_REGISTER_DYNAMIC_TEST_SEARCH(BCPyspecSuiteFixture_homestead, "BlockchainTests/Pyspecs/homestead")
    BOOST_FIXTURE_TEST_SUITE(homestead, BCPyspecSuiteFixture_homestead)
    BOOST_AUTO_TEST_SUITE_END()
    using BCPyspecSuiteFixture_istanbul = TestFixture<BlockchainTestPyspecSuite_istanbul, DefaultFlags>;
    ETH_REGISTER_DYNAMIC_TEST_SEARCH(BCPyspecSuiteFixture_istanbul, "BlockchainTests/Pyspecs/istanbul")
    BOOST_FIXTURE_TEST_SUITE(istanbul, BCPyspecSuiteFixture_istanbul)
    BOOST_AUTO_TEST_SUITE_END()
    using BCPyspecSuiteFixture_berlin = TestFixture<BlockchainTestPyspecSuite_berlin, DefaultFlags>;
    ETH_REGISTER_DYNAMIC_TEST_SEARCH(BCPyspecSuiteFixture_berlin, "BlockchainTests/Pyspecs/berlin")
    BOOST_FIXTURE_TEST_SUITE(berlin, BCPyspecSuiteFixture_berlin)
    BOOST_AUTO_TEST_SUITE_END()
    using BCPyspecSuiteFixture_merge = TestFixture<BlockchainTestPyspecSuite_merge, DefaultFlags>;
    ETH_REGISTER_DYNAMIC_TEST_SEARCH(BCPyspecSuiteFixture_merge, "BlockchainTests/Pyspecs/merge")
    BOOST_FIXTURE_TEST_SUITE(merge, BCPyspecSuiteFixture_merge)
    BOOST_AUTO_TEST_SUITE_END()
    using BCPyspecSuiteFixture_shanghai = TestFixture<BlockchainTestPyspecSuite_shanghai, DefaultFlags>;
    ETH_REGISTER_DYNAMIC_TEST_SEARCH(BCPyspecSuiteFixture_shanghai, "BlockchainTests/Pyspecs/shanghai")
    BOOST_FIXTURE_TEST_SUITE(shanghai, BCPyspecSuiteFixture_shanghai)
    BOOST_AUTO_TEST_SUITE_END()
    using BCPyspecSuiteFixture_cancun = TestFixture<BlockchainTestPyspecSuite_cancun, DefaultFlags>;
    ETH_REGISTER_DYNAMIC_TEST_SEARCH(BCPyspecSuiteFixture_cancun, "BlockchainTests/Pyspecs/cancun")
    BOOST_FIXTURE_TEST_SUITE(cancun, BCPyspecSuiteFixture_cancun)
    BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END() // Pyspecs


// Tests that contain only valid blocks and check that import is correct
using BCValidSuiteFixture = TestFixture<BlockchainTestValidSuite, DefaultFlags>;
ETH_REGISTER_DYNAMIC_TEST_SEARCH(BCValidSuiteFixture, "BlockchainTests/ValidBlocks")
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
ETH_REGISTER_DYNAMIC_TEST_SEARCH(BCInValidSuiteFixture, "BlockchainTests/InvalidBlocks")
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
ETH_REGISTER_DYNAMIC_TEST_SEARCH(BCTransitionFixture, "BlockchainTests/TransitionTests")
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
ETH_REGISTER_DYNAMIC_TEST_SEARCH(BCGeneralsStateSuiteFixture, "BCGeneralStateTests")
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
BOOST_AUTO_TEST_CASE(stEIP2930) {}
BOOST_AUTO_TEST_CASE(stEIP1559) {}
BOOST_AUTO_TEST_CASE(stEIP3607) {}

// Heavy
BOOST_AUTO_TEST_CASE(stTimeConsuming) {}

// Converted VMTests
using BCGeneralStateTestsVMFixture = TestFixture<BCGeneralStateTestsVMSuite, RequireOptionAll>;
ETH_REGISTER_DYNAMIC_TEST_SEARCH(BCGeneralStateTestsVMFixture, "BCGeneralStateTests/VMTests")
BOOST_FIXTURE_TEST_SUITE(VMTests, BCGeneralStateTestsVMFixture)
BOOST_AUTO_TEST_CASE(vmArithmeticTest) {}
BOOST_AUTO_TEST_CASE(vmBitwiseLogicOperation) {}
BOOST_AUTO_TEST_CASE(vmIOandFlowOperations) {}
BOOST_AUTO_TEST_CASE(vmLogTest) {}
BOOST_AUTO_TEST_CASE(vmPerformance) {}
BOOST_AUTO_TEST_CASE(vmTests) {}
BOOST_AUTO_TEST_SUITE_END()

// Shanghai tests
using BCGeneralStateTestsShanghaiFixture = TestFixture<BCGeneralStateTestsShanghaiSuite, RequireOptionAll>;
ETH_REGISTER_DYNAMIC_TEST_SEARCH(BCGeneralStateTestsShanghaiFixture, "BCGeneralStateTests/Shanghai")
BOOST_FIXTURE_TEST_SUITE(Shanghai, BCGeneralStateTestsShanghaiFixture)
BOOST_AUTO_TEST_SUITE_END()


BOOST_AUTO_TEST_SUITE_END()
