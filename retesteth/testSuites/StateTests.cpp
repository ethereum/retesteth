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
/** @file StateTests.cpp
 * @author Dimitry Khokhlov <dimitry@ethereum.org>
 * @date 2016
 * General State Tests parser.
 */

#include <boost/filesystem/operations.hpp>
#include <boost/test/unit_test.hpp>
#include <thread>
#include <mutex>

#include <dataObject/DataObject.h>
#include <libdevcore/CommonData.h>
#include <libdevcore/CommonIO.h>
#include <retesteth/ExitHandler.h>
#include <retesteth/Options.h>
#include <retesteth/RPCSession.h>
#include <retesteth/TestHelper.h>
#include <retesteth/TestOutputHelper.h>
#include <retesteth/TestSuite.h>
#include <retesteth/ethObjects/common.h>
#include <retesteth/testSuites/BlockchainTests.h>
#include <retesteth/testSuites/Common.h>
#include <retesteth/testSuites/StateTests.h>

using namespace std;
using namespace dev;
namespace fs = boost::filesystem;

namespace
{
bool OptionsAllowTransaction(scheme_generalTransaction::transactionInfo const& _tr)
{
    Options const& opt = Options::get();
    if ((opt.trDataIndex == (int)_tr.dataInd || opt.trDataIndex == -1) &&
        (opt.trGasIndex == (int)_tr.gasInd || opt.trGasIndex == -1) &&
        (opt.trValueIndex == (int)_tr.valueInd || opt.trValueIndex == -1))
        return true;
    return false;
}

/// Generate a blockchain test from state test filler
DataObject FillTestAsBlockchain(DataObject const& _testFile, TestSuite::TestSuiteOptions& _opt)
{
    DataObject filledTest;
    test::scheme_stateTestFiller test(_testFile);

    RPCSession& session = RPCSession::instance(TestOutputHelper::getThreadID());
    // run transactions on all networks that we need
    for (auto const& net : test.getAllNetworksFromExpectSection())
    {
        // run transactions for defined expect sections only
        for (auto const& expect : test.getExpectSections())
        {
            // if expect section for this networks
            if (expect.getNetworks().count(net))
            {
                for (auto& tr : test.getTransactionsUnsafe())
                {
                    if (!OptionsAllowTransaction(tr))
                        continue;

                    // if expect section is for this transaction
                    if (!expect.checkIndexes(tr.dataInd, tr.gasInd, tr.valueInd))
                        continue;

                    TestOutputHelper::get().setCurrentTestInfo(
                        "Network: " + net + ", TrInfo: d: " + toString(tr.dataInd) +
                        ", g: " + toString(tr.gasInd) + ", v: " + toString(tr.valueInd) +
                        ", Test: " + TestOutputHelper::get().testName());

                    // State Tests does not have mining rewards
                    scheme_expectSectionElement mexpect = expect;
                    mexpect.correctMiningReward(net, test.getEnv().getCoinbase());

                    string sEngine = "NoProof";
                    session.test_setChainParams(test.getGenesisForRPC(net, sEngine).asJson());
                    u256 a(test.getEnv().getData().at("currentTimestamp").asString());
                    session.test_modifyTimestamp(a.convert_to<size_t>());
                    string signedTransactionRLP = tr.transaction.getSignedRLP();
                    string trHash = session.eth_sendRawTransaction(signedTransactionRLP);
                    session.test_mineBlocks(1);
                    tr.executed = true;

                    DataObject remoteState =
                        getRemoteState(session, isHash<h256>(trHash) ? trHash : string(), true);

                    // check that the post state qualifies to the expect section
                    string testInfo = TestOutputHelper::get().testInfo();
                    scheme_state postState(remoteState.at("postState"));
                    CompareResult res = test::compareStates(mexpect.getExpectState(), postState);
                    ETH_ERROR_REQUIRE_MESSAGE(res == CompareResult::Success, testInfo);
                    if (res != CompareResult::Success)
                        _opt.wasErrors = true;

                    DataObject aBlockchainTest;
                    if (test.getData().count("_info"))
                        aBlockchainTest["_info"] = test.getData().at("_info");
                    aBlockchainTest["genesisBlockHeader"] = test.getEnv().getDataForRPC();
                    aBlockchainTest["pre"] = test.getPre().getData();
                    aBlockchainTest["postState"] = remoteState.at("postState");
                    aBlockchainTest["network"] = net;
                    aBlockchainTest["sealEngine"] = sEngine;

                    test::scheme_block blockData(remoteState.at("rawBlockData"));
                    ETH_ERROR_REQUIRE_MESSAGE(blockData.getTransactionCount() == 1,
                        "StateTest transaction execution failed! " + testInfo);
                    aBlockchainTest["lastblockhash"] = blockData.getBlockHash();

                    test::scheme_block genesisBlock = session.eth_getBlockByNumber("0", true);
                    aBlockchainTest["genesisRLP"] = genesisBlock.getBlockRLP();

                    DataObject block;
                    block["rlp"] = blockData.getBlockRLP();
                    block["blockHeader"] = blockData.getBlockHeader();
                    aBlockchainTest["blocks"].addArrayObject(block);

                    string dataPostfix = "_d" + toString(tr.dataInd) + "g" + toString(tr.gasInd) +
                                         "v" + toString(tr.valueInd);
                    dataPostfix += "_" + net;

                    // !!! Expect sections could contain double cases !!!
                    filledTest[_testFile.getKey() + dataPostfix] = aBlockchainTest;
                    session.test_rewindToBlock(0);
                }
            }
        }
        test.checkUnexecutedTransactions();
    }
    return filledTest;
}

/// Rewrite the test file. Fill General State Test
DataObject FillTest(DataObject const& _testFile, TestSuite::TestSuiteOptions& _opt)
{
    DataObject filledTest;
    test::scheme_stateTestFiller test(_testFile);

    RPCSession& session = RPCSession::instance(TestOutputHelper::getThreadID());
    if (test.getData().count("_info"))
        filledTest["_info"] = test.getData().at("_info");
    filledTest["env"] = test.getEnv().getData();
    filledTest["pre"] = test.getPre().getData();
    filledTest["transaction"] = test.getGenTransaction().getData();

    // run transactions on all networks that we need
    for (auto const& net: test.getAllNetworksFromExpectSection())
    {
        DataObject forkResults;
        forkResults.setKey(net);
        session.test_setChainParams(test.getGenesisForRPC(net, "NoReward").asJson());

        // run transactions for defined expect sections only
        for (auto const& expect : test.getExpectSections())
        {
            // if expect section for this networks
            if (expect.getNetworks().count(net))
            {
                for (auto& tr : test.getTransactionsUnsafe())
                {
                    if (!OptionsAllowTransaction(tr))
                        continue;

                    // if expect section is for this transaction
                    if (!expect.checkIndexes(tr.dataInd, tr.gasInd, tr.valueInd))
                        continue;

                    TestOutputHelper::get().setCurrentTestInfo(
                        "Network: " + net + ", TrInfo: d: " + toString(tr.dataInd) +
                        ", g: " + toString(tr.gasInd) + ", v: " + toString(tr.valueInd) +
                        ", Test: " + TestOutputHelper::get().testName());

                    u256 a(test.getEnv().getData().at("currentTimestamp").asString());
                    session.test_modifyTimestamp(a.convert_to<size_t>());
                    string trHash = session.eth_sendRawTransaction(tr.transaction.getSignedRLP());
                    if (!session.getLastRPCError().empty())
                        ETH_ERROR_MESSAGE(session.getLastRPCError());

                    session.test_mineBlocks(1);
                    tr.executed = true;

                    DataObject remoteState = getRemoteState(session, trHash, true);

                    // check that the post state qualifies to the expect section
                    scheme_state postState(remoteState.at("postState"));
                    CompareResult res = test::compareStates(expect.getExpectState(), postState);
                    ETH_ERROR_REQUIRE_MESSAGE(
                        res == CompareResult::Success, TestOutputHelper::get().testInfo());
                    if (res != CompareResult::Success)
                        _opt.wasErrors = true;

                    DataObject indexes;
                    DataObject transactionResults;
                    indexes["data"] = tr.dataInd;
                    indexes["gas"] = tr.gasInd;
                    indexes["value"] = tr.valueInd;

                    transactionResults["indexes"] = indexes;
                    transactionResults["hash"] = remoteState.at("postHash").asString();
                    if (remoteState.count("logHash"))
                        transactionResults["logs"] = remoteState.at("logHash").asString();
                    forkResults.addArrayObject(transactionResults);
                    session.test_rewindToBlock(0);
                }
            }
        }
        test.checkUnexecutedTransactions();
        filledTest["post"].addSubObject(forkResults);
    }
    return filledTest;
}

/// Read and execute the test file
void RunTest(DataObject const& _testFile)
{
    test::scheme_stateTest test(_testFile);
    RPCSession& session = RPCSession::instance(TestOutputHelper::getThreadID());

	// read post state results
    for (auto const& post: test.getPost().getResults())
	{
        string const& network = post.first;
        if (!Options::get().singleTestNet.empty() && Options::get().singleTestNet != network)
            continue;

        session.test_setChainParams(test.getGenesisForRPC(network, "NoReward").asJson());

        // read all results for a specific fork
        for (auto const& result: post.second)
        {
			// look for a transaction with this indexes and execute it on a client
            for (auto& tr: test.getTransactionsUnsafe())
			{
				if (!OptionsAllowTransaction(tr))
					continue;

				bool blockMined = false;
                if (result.checkIndexes(tr.dataInd, tr.gasInd, tr.valueInd))
                {
                    string testInfo = TestOutputHelper::get().testName() + ", fork: " + network
                                    + ", TrInfo: d: " + toString(tr.dataInd) + ", g: " + toString(tr.gasInd)
                                    + ", v: " + toString(tr.valueInd);
                    u256 a(test.getEnv()
                               .getData()
                               .at("currentTimestamp")
                               .asString());
                    session.test_modifyTimestamp(a.convert_to<size_t>());
                    string trHash = session.eth_sendRawTransaction(
                        tr.transaction.getSignedRLP());
                    session.test_mineBlocks(1);
                    tr.executed = true;
                    blockMined = true;

                    DataObject remoteState =
                        getRemoteState(session, trHash, false);
                    string expectHash = result.getData().at("hash").asString();
                    string expectLogHash =
                        result.getData().at("logs").asString();
                    if (remoteState.at("postHash").asString() != expectHash) {
                      remoteState.clear();
                      remoteState = getRemoteState(session, trHash, true);
					}

                    ETH_ERROR_REQUIRE_MESSAGE(remoteState.at("postHash").asString() == expectHash,
                        "Error at " + testInfo + ", post hash mismatch: " +
                            remoteState.at("postHash").asString() + ", expected: " + expectHash);
                    if (remoteState.at("postHash").asString() != expectHash)
                        ETH_TEST_MESSAGE("\nState Dump: \n" + remoteState.at("postState").asJson());

                    if (remoteState.count("logHash"))
                    {
                        ETH_ERROR_REQUIRE_MESSAGE(
                            remoteState.at("logHash").asString() == expectLogHash,
                            "Error at " + testInfo +
                                ", logs hash mismatch: " + remoteState.at("logHash").asString() +
                                ", expected: " + expectLogHash);
                    }
                }
                if (blockMined)
					session.test_rewindToBlock(0);
			}
		}

		test.checkUnexecutedTransactions();
	}
}
}  // namespace closed

namespace test
{
DataObject StateTestSuite::doTests(DataObject const& _input, TestSuiteOptions& _opt) const
{
    checkDataObject(_input);

    // Do not check only one test if RUNNING a blockchain test with (--fillhchain)
    if (!Options::get().fillchain || _opt.doFilling)
        checkOnlyOneTest(_input);

    DataObject filledTest;
    DataObject const& inputTest = _input.getSubObjects().at(0);
    string const testname = inputTest.getKey();
    if (!TestOutputHelper::get().checkTest(testname))
        return filledTest;

    if (_opt.doFilling)
    {
        checkTestNameIsEqualToFileName(_input);
        DataObject outputTest;
        if (Options::get().fillchain)
        {
            // Each transaction will produce many tests
            outputTest = FillTestAsBlockchain(inputTest, _opt);
            for (auto const& obj : outputTest.getSubObjects())
                filledTest.addSubObject(obj);
        }
        else
        {
            outputTest[testname] = FillTest(inputTest, _opt);
            filledTest = outputTest;
        }
    }
    else
    {
        if (Options::get().fillchain)
        {
            BlockchainTestSuite bcTestSuite;
            bcTestSuite.doTests(_input, _opt);
        }
        else
            RunTest(inputTest);
    }
    return filledTest;
}

TestSuite::TestPath StateTestSuite::suiteFolder() const
{
    if (Options::get().fillchain)
        return TestSuite::TestPath(fs::path("BlockchainTests") / "GeneralStateTests2");
    return TestSuite::TestPath(fs::path("GeneralStateTests"));
}

TestSuite::FillerPath StateTestSuite::suiteFillerFolder() const
{
    return TestSuite::FillerPath(fs::path("src") / "GeneralStateTestsFiller");
}

}// Namespace Close

class GeneralTestFixture
{
public:
    GeneralTestFixture()
    {
        test::StateTestSuite suite;
        string casename = boost::unit_test::framework::current_test_case().p_name;
        static vector<string> const timeConsumingTestSuites{
            string{"stTimeConsuming"}, string{"stQuadraticComplexityTest"}};
        if (test::inArray(timeConsumingTestSuites, casename) && !test::Options::get().all)
        {
            if (!ExitHandler::receivedExitSignal())
                std::cout << "Skipping " << casename << " because --all option is not specified.\n";
            return;
        }
        suite.runAllTestsInFolder(casename);
    }
};

BOOST_FIXTURE_TEST_SUITE(GeneralStateTests, GeneralTestFixture)

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
BOOST_AUTO_TEST_CASE(stCodeCopyTest){}
BOOST_AUTO_TEST_CASE(stBugs){}

//Stress Tests
BOOST_AUTO_TEST_CASE(stAttackTest){}
BOOST_AUTO_TEST_CASE(stMemoryStressTest){}
BOOST_AUTO_TEST_CASE(stQuadraticComplexityTest){}

//Invalid Opcode Tests
BOOST_AUTO_TEST_CASE(stBadOpcode){}

//New Tests
BOOST_AUTO_TEST_CASE(stArgsZeroOneBalance){}
BOOST_AUTO_TEST_CASE(stEWASMTests){}
BOOST_AUTO_TEST_CASE(stTimeConsuming) {}
BOOST_AUTO_TEST_SUITE_END()

