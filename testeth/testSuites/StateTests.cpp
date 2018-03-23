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

#include <devcore/CommonIO.h>
#include <testeth/Options.h>
#include <testeth/TestOutputHelper.h>
#include <testeth/TestHelper.h>
#include <testeth/TestSuite.h>
#include <testeth/DataObject.h>
#include <testeth/testSuites/StateTests.h>
#include <testeth/ethObjects/stateTest.h>

#include <testeth/RPCSession.h>

using namespace std;
using namespace dev;
namespace fs = boost::filesystem;

namespace test {

DataObject const& stateGenesis()
{
    static DataObject genesis;
    if (genesis.type() == DataType::Null)
    {
        genesis["version"] = "1";
        genesis["params"]["miningMethod"] = "NoProof";
        genesis["params"]["blockReward"] = "0x00";
    }
    return genesis;
}

bool OptionsAllowTransaction(generalTransaction::transactionInfo const& _tr)
{
    Options const& opt = Options::get();
    if ((opt.trDataIndex == (int)_tr.dataInd || opt.trDataIndex == -1) &&
        (opt.trGasIndex == (int)_tr.gasInd || opt.trGasIndex == -1) &&
        (opt.trValueIndex == (int)_tr.valueInd || opt.trValueIndex == -1))
        return true;
    return false;
}

/// Rewrite the test file
DataObject FillTest(DataObject const& _testFile, TestSuite::TestSuiteOptions& _opt)
{
    DataObject filledTest;
    test::stateTestFiller test(_testFile);

    // Copy Sctions form test source
    filledTest.setKey(_testFile.getKey());

    RPCSession& session = RPCSession::instance("/home/wins/.ethereum/geth.ipc");
    filledTest["env"] = test.getEnv().getData();
    filledTest["pre"] = test.getPre().getData();
    filledTest["transaction"] = test.getGenTransaction().getData();

    // run transactions on all networks that we need
    for (auto const& net: test.getAllNetworksFromExpectSection())
    {
        if (!Options::get().singleTestNet.empty() && Options::get().singleTestNet != net)
            continue;

        DataObject genesis = stateGenesis();
		genesis["genesis"] = test.getEnv().getDataForRPC();
        genesis["genesis"]["timestamp"] = "0x00";	//Set Genesis tstmp to 0. the actual timestamp specified in env section is a timestamp of the first block.
        genesis["state"] = test.getPre().getData();
        genesis["params"]["forkRules"] = net;
        session.test_setChainParams(genesis.asJson());

        DataObject forkResults;
        forkResults.setKey(net);

        // run transactions for defined expect sections only
        for (auto const& expect: test.getExpectSections())
        {
            // if expect section for this networks
            if (expect.getNetworks().count(net))
            {
                for (auto& tr: test.getTransactionsUnsafe())
                {
                    if (!OptionsAllowTransaction(tr))
                        continue;

                    bool blockMined = false;
                    // if expect section is for this transaction
                    if (expect.checkIndexes(tr.dataInd, tr.gasInd, tr.valueInd))
                    {
						u256 a(test.getEnv().getDataForRPC().at("timestamp").asString());
                        session.test_modifyTimestamp(a.convert_to<size_t>());
                        session.test_addTransaction(tr.transaction.getData().asJson());
                        session.test_mineBlocks(1);
                        tr.executed = true;
                        blockMined = true;
                        string postHash = session.test_getPostState("{ \"version\" : \"0x01\" }");
                        string fullPost = session.test_getPostState("{ \"version\" : \"0x02\" }");
                        string postLogs = session.test_getPostState("{ \"version\" : \"0x03\" }");

                        // check that the post state qualifies to the expect section
                        state postState = (test::convertJsonCPPtoData(readJson(fullPost)));
                        expectState expectSection (expect.getData().at("result"));
                        CompareResult res = test::compareStates(expectSection, postState);
                        BOOST_CHECK_MESSAGE(res == CompareResult::Success, "Network: " + net + ", TrInfo: d: " + toString(tr.dataInd) + ", g: "
											+ toString(tr.gasInd) + ", v: " + toString(tr.valueInd) + "\n");
                        if (res != CompareResult::Success)
                            _opt.wasErrors = true;

                        DataObject indexes;
                        DataObject transactionResults;
                        indexes["data"] = tr.dataInd;
                        indexes["gas"] = tr.gasInd;
                        indexes["value"] = tr.valueInd;

                        transactionResults["indexes"] = indexes;
                        transactionResults["hash"] = postHash;
                        transactionResults["logs"] = postLogs;
                        forkResults.addArrayObject(transactionResults);
                    }
                    if (blockMined)
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
    test::stateTest test(_testFile);
	RPCSession& session = RPCSession::instance("/home/wins/.ethereum/geth.ipc");

	// read post state results
    for (auto const& post: test.getPost().getResults())
	{
        string const& network = post.first;
        if (!Options::get().singleTestNet.empty() && Options::get().singleTestNet != network)
			continue;

        DataObject genesis = stateGenesis();
		genesis["genesis"] = test.getEnv().getDataForRPC();
        genesis["genesis"]["timestamp"] = "0x00";	//Set Genesis tstmp to 0. the actual timestamp specified in env section is a timestamp of the first block.
        genesis["state"] = test.getPre().getData();
        genesis["params"]["forkRules"] = network;
		session.test_setChainParams(genesis.asJson());

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
					u256 a(test.getEnv().getDataForRPC().at("timestamp").asString());
					session.test_modifyTimestamp(a.convert_to<size_t>());
					session.test_addTransaction(tr.transaction.getData().asJson());
					session.test_mineBlocks(1);
					tr.executed = true;
					blockMined = true;
					string fullPost;
                    string postHash = session.test_getPostState("{ \"version\" : \"0x01\" }");
					if (postHash != result.getData().at("hash").asString())
                        fullPost = session.test_getPostState("{ \"version\" : \"0x02\" }");
					BOOST_CHECK_MESSAGE(postHash == result.getData().at("hash").asString(),
                        "Error at " + testInfo + ", hash mismatch: " + postHash + ", expected: " + result.getData().at("hash").asString()
						 + "\nState Dump: " + fullPost);
					string postLogs = session.test_getPostState("{ \"version\" : \"0x03\" }");
					BOOST_CHECK_MESSAGE(postLogs == result.getData().at("logs").asString(),
                        "Error at " + testInfo + ", logs hash mismatch: " + postHash + ", expected: " + result.getData().at("logs").asString());

				}
				if (blockMined)
					session.test_rewindToBlock(0);
			}
		}

		test.checkUnexecutedTransactions();
	}
}

DataObject StateTestSuite::doTests(DataObject const& _input, TestSuiteOptions& _opt) const
{
	BOOST_REQUIRE_MESSAGE(_input.type() == DataType::Object,
		TestOutputHelper::get().get().testFile().string() + " A GeneralStateTest file should contain an object.");
    BOOST_REQUIRE_MESSAGE(!_opt.doFilling || _input.getSubObjects().size() == 1,
		TestOutputHelper::get().testFile().string() + " A GeneralStateTest filler should contain only one test.");

    DataObject filledTest;
	for (auto& i: _input.getSubObjects())
	{
		string const testname = i.getKey();
		BOOST_REQUIRE_MESSAGE(i.type() == DataType::Object,
			TestOutputHelper::get().testFile().string() + " should contain an object under a test name.");
		DataObject const& inputTest = i;
        DataObject outputTest;
		outputTest.setKey(testname);

        if (_opt.doFilling && !TestOutputHelper::get().testFile().empty())
			BOOST_REQUIRE_MESSAGE(testname + "Filler" == TestOutputHelper::get().testFile().stem().string(),
				TestOutputHelper::get().testFile().string() + " contains a test with a different name '" + testname + "'");

		if (!TestOutputHelper::get().checkTest(testname))
			continue;

        if (_opt.doFilling)
            outputTest = FillTest(inputTest, _opt);
		else
			RunTest(inputTest);

        filledTest.addSubObject(outputTest);
	}
    return filledTest;
}

fs::path StateTestSuite::suiteFolder() const
{
	return "GeneralStateTests";
}

fs::path StateTestSuite::suiteFillerFolder() const
{
	return "GeneralStateTestsFiller";
}

}// Namespace Close

class GeneralTestFixture
{
public:
	GeneralTestFixture()
	{
		test::StateTestSuite suite;
		string casename = boost::unit_test::framework::current_test_case().p_name;
		if (casename == "stQuadraticComplexityTest" && !test::Options::get().all)
		{
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
BOOST_AUTO_TEST_SUITE_END()

