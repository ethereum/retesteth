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
#include <dataObject/DataObject.h>
#include <retesteth/EthChecks.h>
#include <retesteth/Options.h>
#include <retesteth/RPCSession.h>
#include <retesteth/TestOutputHelper.h>
#include <retesteth/ethObjects/common.h>
#include <retesteth/testSuites/Common.h>
#include <boost/filesystem/operations.hpp>
#include <boost/test/unit_test.hpp>
#include <thread>

using namespace std;
namespace fs = boost::filesystem;

namespace
{
/// Generate blockchain test from filler
bool FillTest(
    scheme_blockchainTestFiller const& _testObject, string const& _network, DataObject& _testOut)
{
    // construct filled blockchain test
    _testOut["sealEngine"] = _testObject.getSealEngine();
    _testOut["network"] = _network;
    _testOut["pre"] = _testObject.getData().atKey("pre");
    if (_testObject.getData().count("_info"))
        _testOut["_info"] = _testObject.getData().atKey("_info");

    RPCSession& session = RPCSession::instance(TestOutputHelper::getThreadID());
    DataObject genesisObject = _testObject.getGenesisForRPC(_network);
    session.test_setChainParams(genesisObject.asJson());

    _testOut["genesisBlockHeader"] = session.eth_getBlockByNumber("0", false).getBlockHeader();
    _testOut["genesisBlockHeader"].removeKey("transactions");
    _testOut["genesisBlockHeader"].removeKey("uncles");
    // u256 genesisTimestamp = u256(genesisObject.atKey("genesis").atKey("timestamp").asString());
    // session.test_modifyTimestamp(genesisTimestamp.convert_to<size_t>());

    string lastTrHash;
    string lastBlHash;
    size_t number = 0;
    for (auto const& block : _testObject.getBlocks())
    {
        TestOutputHelper::get().setCurrentTestInfo("Network: " + _network +
                                                   ", BlockNumber: " + toString(number++) +
                                                   ", Test: " + TestOutputHelper::get().testName());

        DataObject blockSection;
        for (auto const& tr : block.getTransactions())
        {
            lastTrHash = session.eth_sendRawTransaction(tr.getSignedRLP());
            if (!session.getLastRPCError().empty())
                ETH_ERROR_MESSAGE(session.getLastRPCError());
            if (!isHash<h256>(lastTrHash))
                ETH_ERROR_MESSAGE("eth_sendRawTransaction return invalid hash: '" + lastTrHash +
                                  "' " + TestOutputHelper::get().testInfo());
            blockSection["transactions"].addArrayObject(tr.getData());
        }
        session.test_mineBlocks(1);

        DataObject remoteState = getRemoteState(session, "", false);
        test::scheme_block blockData(remoteState.atKey("rawBlockData"));
        ETH_ERROR_REQUIRE_MESSAGE(blockData.getTransactionCount() == block.getTransactions().size(),
            "BlockchainTest transaction execution failed! " + TestOutputHelper::get().testInfo());
        blockSection["rlp"] = blockData.getBlockRLP();
        blockSection["blockHeader"] = blockData.getBlockHeader();
        _testOut["blocks"].addArrayObject(blockSection);
    }

    DataObject remoteState = getRemoteState(session, lastTrHash, true);
    scheme_state postState(remoteState.atKey("postState"));
    CompareResult res = test::compareStates(
        _testObject.getExpectSection().getExpectSectionFor(_network).getExpectState(), postState);
    ETH_ERROR_REQUIRE_MESSAGE(res == CompareResult::Success, TestOutputHelper::get().testInfo());
    if (res != CompareResult::Success)
        return true;
    _testOut["postState"] = remoteState.atKey("postState");
    _testOut["lastblockhash"] = lastBlHash;
    return false;
}

/// Read and execute the test from the file
bool RunTest(DataObject const& _testObject)
{
    scheme_blockchainTest inputTest(_testObject);
    RPCSession& session = RPCSession::instance(TestOutputHelper::getThreadID());

    session.test_setChainParams(inputTest.getGenesisForRPC(inputTest.getNetwork()).asJson());

    // for all blocks
    for (auto const& brlp : inputTest.getBlockRlps())
    {
        session.test_importRawBlock(brlp);
        if (!session.getLastRPCError().empty())
            ETH_ERROR_MESSAGE("Running blockchain test: " + session.getLastRPCError());
    }

    // wait for blocks to process
    // std::this_thread::sleep_for(std::chrono::seconds(10));

    // compare post state hash
    DataObject remoteState = getRemoteState(session, "", true);
    scheme_state postState(remoteState.atKey("postState"));
    CompareResult res = test::compareStates(inputTest.getPost(), postState);
    ETH_ERROR_REQUIRE_MESSAGE(
        res == CompareResult::Success, "Error in " + inputTest.getData().getKey());
    return (res != CompareResult::Success);
}

}  // namespace closed

namespace test
{
DataObject BlockchainTestSuite::doTests(DataObject const& _input, TestSuiteOptions& _opt) const
{
    checkDataObject(_input);
    checkAtLeastOneTest(_input);

    DataObject tests;
    // A blockchain test file contains many tests in one .json file
    for (auto const& i : _input.getSubObjects())
    {
        string const& testname = i.getKey();
        TestOutputHelper::get().setCurrentTestName(testname);

        if (_opt.doFilling)
        {
            scheme_blockchainTestFiller testFiller(i);
            // First convert multiple test blockchain filler to a specific network test filler
            // create a blockchain test for each network described in expect section
            for (auto& network : testFiller.getExpectSection().getAllNetworksFromExpectSection())
            {
                // select expect section corresponding to the network (if any)
                for (auto const& expect : testFiller.getExpectSection().getExpectSections())
                {
                    if (expect.getNetworks().count(network))
                    {
                        string const newtestname = testname + "_" + network;
                        TestOutputHelper::get().setCurrentTestName(newtestname);

                        DataObject testOutput;
                        _opt.wasErrors = FillTest(testFiller, network, testOutput);
                        if (testFiller.getData().count("_info"))
                            testOutput["_info"] = testFiller.getData().atKey("_info");
                        tests[newtestname] = testOutput;
                    }
                }
            }
        }
        else
        {
            // Select test by name if --singletest and --singlenet is set
            if (!Options::get().singleTestNet.empty() && Options::get().singleTest)
                if (testname != Options::get().singleTestName + "_" + Options::get().singleTestNet)
                    continue;

            _opt.wasErrors = RunTest(i);
        }
    }

    return tests;
}

TestSuite::TestPath BlockchainTestSuite::suiteFolder() const
{
    return TestSuite::TestPath(fs::path("BlockchainTests"));
}

TestSuite::FillerPath BlockchainTestSuite::suiteFillerFolder() const
{
    return TestSuite::FillerPath(fs::path("src") / "BlockchainTestsFiller");
}

TestSuite::TestPath BCGeneralStateTestsSuite::suiteFolder() const
{
    return TestSuite::TestPath(fs::path("BlockchainTests") / "GeneralStateTests");
}

TestSuite::FillerPath BCGeneralStateTestsSuite::suiteFillerFolder() const
{
    return TestSuite::FillerPath(
        fs::path("src") / fs::path("BlockchainTestsFiller") / "GeneralStateTests");
}

}  // Namespace Close

class BlockchainTestFixture
{
public:
    BlockchainTestFixture()
    {
        test::BlockchainTestSuite suite;
        string casename = boost::unit_test::framework::current_test_case().p_name;
        boost::filesystem::path suiteFillerPath = suite.getFullPathFiller(casename).parent_path();
        if (casename == "stQuadraticComplexityTest" && !test::Options::get().all)
        {
            std::cout << "Skipping " << casename << " because --all option is not specified.\n";
            test::TestOutputHelper::get().markTestFolderAsFinished(suiteFillerPath, casename);
            return;
        }
        // Mark other folders so not to check them for empty tests
        test::TestOutputHelper::get().markTestFolderAsException(suiteFillerPath, "TransitionTests");
        test::TestOutputHelper::get().markTestFolderAsException(
            suiteFillerPath, "GeneralStateTests");

        suite.runAllTestsInFolder(casename);
        test::TestOutputHelper::get().markTestFolderAsFinished(suiteFillerPath, casename);
    }
};

class bcGeneralTestsFixture
{
public:
    bcGeneralTestsFixture()
    {
        test::BCGeneralStateTestsSuite suite;
        string const& casename = boost::unit_test::framework::current_test_case().p_name;
        boost::filesystem::path suiteFillerPath = suite.getFullPathFiller(casename).parent_path();

        //skip this test suite if not run with --all flag (cases are already tested in state tests)
        if (!test::Options::get().all)
        {
            std::cout << "Skipping hive test " << casename << ". Use --all to run it.\n";
            test::TestOutputHelper::get().markTestFolderAsFinished(suiteFillerPath, casename);
            return;
        }
        suite.runAllTestsInFolder(casename);
        test::TestOutputHelper::get().markTestFolderAsFinished(suiteFillerPath, casename);
    }
};

BOOST_FIXTURE_TEST_SUITE(BlockchainTests, BlockchainTestFixture)

BOOST_AUTO_TEST_CASE(bcStateTests){}
BOOST_AUTO_TEST_CASE(bcBlockGasLimitTest){}
BOOST_AUTO_TEST_CASE(bcGasPricerTest){}
BOOST_AUTO_TEST_CASE(bcInvalidHeaderTest){}
BOOST_AUTO_TEST_CASE(bcUncleHeaderValidity){}
BOOST_AUTO_TEST_CASE(bcUncleTest){}
BOOST_AUTO_TEST_CASE(bcValidBlockTest){}
BOOST_AUTO_TEST_CASE(bcWalletTest){}
BOOST_AUTO_TEST_CASE(bcTotalDifficultyTest){}
BOOST_AUTO_TEST_CASE(bcMultiChainTest){}
BOOST_AUTO_TEST_CASE(bcForkStressTest){}
BOOST_AUTO_TEST_CASE(bcForgedTest){}
BOOST_AUTO_TEST_CASE(bcRandomBlockhashTest){}
BOOST_AUTO_TEST_CASE(bcExploitTest) {}
BOOST_AUTO_TEST_CASE(bcUncleSpecialTests){}

BOOST_AUTO_TEST_SUITE_END()

//Transition from fork to fork tests
/*BOOST_FIXTURE_TEST_SUITE(TransitionTests, bcTransitionFixture)

BOOST_AUTO_TEST_CASE(bcFrontierToHomestead){}
BOOST_AUTO_TEST_CASE(bcHomesteadToDao){}
BOOST_AUTO_TEST_CASE(bcHomesteadToEIP150){}
BOOST_AUTO_TEST_CASE(bcEIP158ToByzantium){}
BOOST_AUTO_TEST_CASE(bcByzantiumToConstantinopleFix) {}

BOOST_AUTO_TEST_SUITE_END()
*/

//General tests in form of blockchain tests
BOOST_FIXTURE_TEST_SUITE(BCGeneralStateTests, bcGeneralTestsFixture)

//Frontier Tests
BOOST_AUTO_TEST_CASE(stCallCodes){}
BOOST_AUTO_TEST_CASE(stCallCreateCallCodeTest){}
BOOST_AUTO_TEST_CASE(stExample){}
BOOST_AUTO_TEST_CASE(stInitCodeTest){}
BOOST_AUTO_TEST_CASE(stLogTests){}
BOOST_AUTO_TEST_CASE(stMemoryTest){}
BOOST_AUTO_TEST_CASE(stPreCompiledContracts){}
BOOST_AUTO_TEST_CASE(stPreCompiledContracts2){}
BOOST_AUTO_TEST_CASE(stRandom){}
BOOST_AUTO_TEST_CASE(stRandom2){}
BOOST_AUTO_TEST_CASE(stRecursiveCreate){}
BOOST_AUTO_TEST_CASE(stRefundTest){}
BOOST_AUTO_TEST_CASE(stSolidityTest){}
BOOST_AUTO_TEST_CASE(stSpecialTest){}
BOOST_AUTO_TEST_CASE(stSystemOperationsTest){}
BOOST_AUTO_TEST_CASE(stTransactionTest){}
BOOST_AUTO_TEST_CASE(stTransitionTest){}
BOOST_AUTO_TEST_CASE(stWalletTest){}

//Homestead Tests
BOOST_AUTO_TEST_CASE(stCallDelegateCodesCallCodeHomestead){}
BOOST_AUTO_TEST_CASE(stCallDelegateCodesHomestead){}
BOOST_AUTO_TEST_CASE(stHomesteadSpecific){}
BOOST_AUTO_TEST_CASE(stDelegatecallTestHomestead){}

//EIP150 Tests
BOOST_AUTO_TEST_CASE(stChangedEIP150){}
BOOST_AUTO_TEST_CASE(stEIP150singleCodeGasPrices){}
BOOST_AUTO_TEST_CASE(stMemExpandingEIP150Calls){}
BOOST_AUTO_TEST_CASE(stEIP150Specific){}

//EIP158 Tests
BOOST_AUTO_TEST_CASE(stEIP158Specific){}
BOOST_AUTO_TEST_CASE(stNonZeroCallsTest){}
BOOST_AUTO_TEST_CASE(stZeroCallsTest){}
BOOST_AUTO_TEST_CASE(stZeroCallsRevert){}
BOOST_AUTO_TEST_CASE(stCodeSizeLimit){}
BOOST_AUTO_TEST_CASE(stCreateTest){}
BOOST_AUTO_TEST_CASE(stRevertTest){}

//Metropolis Tests
BOOST_AUTO_TEST_CASE(stStackTests){}
BOOST_AUTO_TEST_CASE(stStaticCall){}
BOOST_AUTO_TEST_CASE(stReturnDataTest){}
BOOST_AUTO_TEST_CASE(stZeroKnowledge){}
BOOST_AUTO_TEST_CASE(stZeroKnowledge2){}
BOOST_AUTO_TEST_CASE(stBugs){}

//Constantinople Tests
BOOST_AUTO_TEST_CASE(stShift){}
BOOST_AUTO_TEST_CASE(stCreate2){}
BOOST_AUTO_TEST_CASE(stExtCodeHash){}
BOOST_AUTO_TEST_CASE(stSStoreTest){}

//Stress Tests
BOOST_AUTO_TEST_CASE(stAttackTest){}
BOOST_AUTO_TEST_CASE(stMemoryStressTest){}
BOOST_AUTO_TEST_CASE(stQuadraticComplexityTest){}

//Bad opcodes test
BOOST_AUTO_TEST_CASE(stBadOpcode){}

//New Tests
BOOST_AUTO_TEST_CASE(stArgsZeroOneBalance){}
BOOST_AUTO_TEST_SUITE_END()
