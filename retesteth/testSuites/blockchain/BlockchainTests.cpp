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
#include <boost/filesystem/operations.hpp>
#include <boost/test/unit_test.hpp>
#include <thread>

using namespace std;
namespace fs = boost::filesystem;

namespace test
{
/// !!! DataObject return without reference!!! must be SP!!!
DataObject BlockchainTestTransitionSuite::doTests(
    DataObject const& _input, TestSuiteOptions& _opt) const
{
    return DoTests(_input, _opt);
}

DataObject BlockchainTestInvalidSuite::doTests(
    DataObject const& _input, TestSuiteOptions& _opt) const
{
    _opt.allowInvalidBlocks = true;
    return DoTests(_input, _opt);
}

DataObject BlockchainTestValidSuite::doTests(DataObject const& _input, TestSuiteOptions& _opt) const
{
    return DoTests(_input, _opt);
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

TestSuite::TestPath LegacyConstantinopleBCGeneralStateTestsSuite::suiteFolder() const
{
    if (Options::get().filltests)
        ETH_ERROR_MESSAGE("Legacy tests are sealed and not refillable!");
    return TestSuite::TestPath(
        fs::path("LegacyTests") / "Constantinople" / "BlockchainTests" / "GeneralStateTests");
}

TestSuite::FillerPath LegacyConstantinopleBCGeneralStateTestsSuite::suiteFillerFolder() const
{
    return TestSuite::FillerPath(fs::path("src") / "LegacyTests" / "Constantinople" /
                                 "BlockchainTestsFiller" / "GeneralStateTests");
}

}  // Namespace Close


class BlockchainTestTransitionFixture
{
public:
    BlockchainTestTransitionFixture()
    {
        test::BlockchainTestTransitionSuite suite;
        string casename = boost::unit_test::framework::current_test_case().p_name;
        boost::filesystem::path suiteFillerPath = suite.getFullPathFiller(casename).parent_path();
        suite.runAllTestsInFolder(casename);
        test::TestOutputHelper::get().markTestFolderAsFinished(suiteFillerPath, casename);
    }
};

void RunBCGeneralStateTests(test::TestSuite const& _testSuite)
{
    string const& casename = boost::unit_test::framework::current_test_case().p_name;
    boost::filesystem::path suiteFillerPath = _testSuite.getFullPathFiller(casename).parent_path();

    // skip this test suite if not run with --all flag (cases are already tested in state tests)
    if (!test::Options::get().all)
    {
        std::cout << "Skipping hive test " << casename << ". Use --all to run it.\n";
        test::TestOutputHelper::get().markTestFolderAsFinished(suiteFillerPath, casename);
        return;
    }
    _testSuite.runAllTestsInFolder(casename);
    test::TestOutputHelper::get().markTestFolderAsFinished(suiteFillerPath, casename);
}

class BCGeneralTestsFixture
{
public:
    BCGeneralTestsFixture()
    {
        BCGeneralStateTestsSuite suite;
        RunBCGeneralStateTests(suite);
    }
};

test::LegacyConstantinopleBCGeneralStateTestFixture::LegacyConstantinopleBCGeneralStateTestFixture()
{
    LegacyConstantinopleBCGeneralStateTestsSuite suite;
    RunBCGeneralStateTests(suite);
}

BOOST_AUTO_TEST_SUITE(BlockchainTests)

// Tests that contain only valid blocks and check that import is correct
BOOST_FIXTURE_TEST_SUITE(ValidBlocks, BlockchainTestValidFixture)
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
BOOST_AUTO_TEST_SUITE_END()

// Tests that might have invalid blocks and check that those are rejected
BOOST_FIXTURE_TEST_SUITE(InvalidBlocks, BlockchainTestInvalidFixture)
BOOST_AUTO_TEST_CASE(bcBlockGasLimitTest) {}
BOOST_AUTO_TEST_CASE(bcForgedTest) {}
BOOST_AUTO_TEST_CASE(bcInvalidHeaderTest) {}
BOOST_AUTO_TEST_CASE(bcMultiChainTest) {}
BOOST_AUTO_TEST_CASE(bcUncleHeaderValidity) {}
BOOST_AUTO_TEST_CASE(bcUncleSpecialTests) {}
BOOST_AUTO_TEST_CASE(bcUncleTest) {}
BOOST_AUTO_TEST_SUITE_END()

// Transition from fork to fork tests
BOOST_FIXTURE_TEST_SUITE(TransitionTests, BlockchainTestTransitionFixture)
BOOST_AUTO_TEST_CASE(bcByzantiumToConstantinopleFix) {}
BOOST_AUTO_TEST_CASE(bcEIP158ToByzantium) {}
BOOST_AUTO_TEST_CASE(bcFrontierToHomestead) {}
BOOST_AUTO_TEST_CASE(bcHomesteadToDao) {}
BOOST_AUTO_TEST_CASE(bcHomesteadToEIP150) {}
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE_END()

// General tests in form of blockchain tests
BOOST_FIXTURE_TEST_SUITE(BCGeneralStateTests, BCGeneralTestsFixture)

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
BOOST_AUTO_TEST_CASE(stChangedEIP150) {}
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

// New Tests
BOOST_AUTO_TEST_CASE(stArgsZeroOneBalance) {}
BOOST_AUTO_TEST_CASE(stTimeConsuming) {}
BOOST_AUTO_TEST_CASE(stChainId) {}
BOOST_AUTO_TEST_CASE(stSLoadTest) {}
BOOST_AUTO_TEST_CASE(stSelfBalance) {}

BOOST_AUTO_TEST_SUITE_END()
