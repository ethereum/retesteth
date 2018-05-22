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

#include <libdevcore/CommonIO.h>
#include <retesteth/Options.h>
#include <retesteth/TestOutputHelper.h>
#include <retesteth/TestHelper.h>
#include <retesteth/TestSuite.h>
#include <retesteth/DataObject.h>
#include <retesteth/testSuites/StateTests.h>
#include <retesteth/ethObjects/common.h>
#include <retesteth/RPCSession.h>

using namespace std;
using namespace dev;
namespace fs = boost::filesystem;

namespace test {

DataObject stateGenesis(string const& _netRules)
{
    test::checkAllowedNetwork(_netRules);
    DataObject genesis;
    genesis["sealEngine"] = "NoProof";
    if (_netRules == "Frontier")
    {
        genesis["params"] = DataObject(DataType::Object);
    }
    else if (_netRules == "Homestead")
    {
        genesis["params"]["homesteadForkBlock"] = "0x00";
    }
    else if (_netRules == "EIP150")
    {
        genesis["params"]["homesteadForkBlock"] = "0x00";
        genesis["params"]["EIP150ForkBlock"] = "0x00";
    }
    else if (_netRules == "EIP158")
    {
        genesis["params"]["homesteadForkBlock"] = "0x00";
        genesis["params"]["EIP150ForkBlock"] = "0x00";
        genesis["params"]["EIP158ForkBlock"] = "0x00";
    }
    else if (_netRules == "Byzantium")
    {
        genesis["params"]["homesteadForkBlock"] = "0x00";
        genesis["params"]["EIP150ForkBlock"] = "0x00";
        genesis["params"]["EIP158ForkBlock"] = "0x00";
        genesis["params"]["byzantiumForkBlock"] = "0x00";
    }
    else if (_netRules == "Constantinople")
    {
        genesis["params"]["homesteadForkBlock"] = "0x00";
        genesis["params"]["EIP150ForkBlock"] = "0x00";
        genesis["params"]["EIP158ForkBlock"] = "0x00";
        genesis["params"]["byzantiumForkBlock"] = "0x00";
        genesis["params"]["constantinopleForkBlock"] = "0x00";
    }

    /*static std::string const c_configString = R"(
    {
        "sealEngine": "NoProof",
        "params": {
            "accountStartNonce": "0x00",
            "maximumExtraDataSize": "0x1000000",
            "blockReward": "0x",
            "allowFutureBlocks": true,
            "homesteadForkBlock": "0x00",
            "EIP150ForkBlock": "0x00",
            "EIP158ForkBlock": "0x00"
        },
        "genesis": {
            "author": "0000000000000010000000000000000000000000",
            "timestamp": "0x00",
            "parentHash": "0x0000000000000000000000000000000000000000000000000000000000000000",
            "extraData": "0x",
            "gasLimit": "0x1000000000000"
        },
        "accounts": {
            "0000000000000000000000000000000000000001": { "wei": "1", "precompiled": { "name": "ecrecover", "linear": { "base": 3000, "word": 0 } } },
            "0000000000000000000000000000000000000002": { "wei": "1", "precompiled": { "name": "sha256", "linear": { "base": 60, "word": 12 } } },
            "0000000000000000000000000000000000000003": { "wei": "1", "precompiled": { "name": "ripemd160", "linear": { "base": 600, "word": 120 } } },
            "0000000000000000000000000000000000000004": { "wei": "1", "precompiled": { "name": "identity", "linear": { "base": 15, "word": 3 } } },
            "0000000000000000000000000000000000000005": { "wei": "1", "precompiled": { "name": "modexp" } },
            "0000000000000000000000000000000000000006": { "wei": "1", "precompiled": { "name": "alt_bn128_G1_add", "linear": { "base": 500, "word": 0 } } },
            "0000000000000000000000000000000000000007": { "wei": "1", "precompiled": { "name": "alt_bn128_G1_mul", "linear": { "base": 40000, "word": 0 } } },
            "0000000000000000000000000000000000000008": { "wei": "1", "precompiled": { "name": "alt_bn128_pairing_product" } }
        }
    }
    )";
    */

    /*if (genesis.type() == DataType::Null)
    {
        genesis["version"] = "1";
        genesis["params"]["miningMethod"] = "NoProof";
        genesis["params"]["blockReward"] = "0x00";
    }*/
    return genesis;
}

DataObject getRemoteState(RPCSession& _session, string const& _trHash, bool _fullPost)
{
    DataObject remoteState;
    const int cmaxRows = 1000;
    string latestBlockNumber = toString(u256(_session.eth_blockNumber()));

    test::scheme_block latestBlock = _session.eth_getBlockByNumber(latestBlockNumber, false);
    remoteState["postHash"] = latestBlock.getData().at("stateRoot");
    remoteState["logHash"] = _session.test_getLogHash(_trHash);
    remoteState["postState"] = "";

    // Get Log Receipts
    /*if (latestBlock.getTransactionCount() != 0)
    {
        test::scheme_transactionReceipt trReceipt = _session.eth_getTransactionReceipt(_trHash);
        remoteState["logHash"] = trReceipt.getLogHash();
        transactionBlockNumber = toString(trReceipt.getData().at("blockNumber").asInt());
        trIndex = trReceipt.getData().at("transactionIndex").asInt();
    }
    else
    {
        // returned latest block has no transactions. this transaction is probably invalid.
        remoteState["logHash"] = toHexPrefixed(dev::EmptyListSHA3);
        transactionBlockNumber = latestBlockNumber;
    }*/

    if (_fullPost)
    {
        int trIndex = 1;
        DataObject accountObj;
        Json::Value res = _session.debug_accountRangeAt(latestBlockNumber, trIndex, "0", cmaxRows);
        for (auto acc : res["addressMap"]) {
          // Balance
          Json::Value ret =
              _session.eth_getBalance(acc.asString(), latestBlockNumber);
          accountObj[acc.asString()]["balance"] = dev::toCompactHexPrefixed(
              u256(ret.asString()), 1); // fix odd strings

          // Code
          ret = _session.eth_getCode(acc.asString(), latestBlockNumber);
          accountObj[acc.asString()]["code"] = ret.asString();

          // Nonce
          ret = _session.eth_getTransactionCount(acc.asString(),
                                                 latestBlockNumber);
          accountObj[acc.asString()]["nonce"] =
              dev::toCompactHexPrefixed(u256(ret.asString()), 1);

          // Storage
          DataObject storage(DataType::Object);
          DataObject debugStorageAt =
              test::convertJsonCPPtoData(_session.debug_storageRangeAt(
                  latestBlockNumber, 1, acc.asString(), "0", cmaxRows));
          for (auto const &element : debugStorageAt["storage"].getSubObjects())
            storage[element.at("key").asString()] =
                element.at("value").asString();
          accountObj[acc.asString()]["storage"] = storage;
        }

        remoteState["postState"].clear();
        remoteState["postState"] = accountObj;
    }
    return remoteState;
}

bool OptionsAllowTransaction(scheme_generalTransaction::transactionInfo const& _tr)
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
    test::scheme_stateTestFiller test(_testFile);

    // Copy Sctions form test source
    filledTest.setKey(_testFile.getKey());

    RPCSession& session = RPCSession::instance(TestOutputHelper::getThreadID());
	if (test.getData().count("_info"))
		filledTest["_info"] = test.getData().at("_info");
    filledTest["env"] = test.getEnv().getData();
    filledTest["pre"] = test.getPre().getData();
    filledTest["transaction"] = test.getGenTransaction().getData();

    // run transactions on all networks that we need
    for (auto const& net: test.getAllNetworksFromExpectSection())
    {
        if (!Options::get().singleTestNet.empty() && Options::get().singleTestNet != net)
            continue;

        DataObject genesis = stateGenesis(net);
		genesis["genesis"] = test.getEnv().getDataForRPC();
                genesis["accounts"] = test.getPre().getDataForRPC(net);
                session.test_setChainParams(genesis.asJson());

                DataObject forkResults;
                forkResults.setKey(net);

                // run transactions for defined expect sections only
                for (auto const &expect : test.getExpectSections()) {
                  // if expect section for this networks
                  if (expect.getNetworks().count(net)) {
                    for (auto &tr : test.getTransactionsUnsafe()) {
                      if (!OptionsAllowTransaction(tr))
                        continue;

                    bool blockMined = false;
                    // if expect section is for this transaction
                    if (expect.checkIndexes(tr.dataInd, tr.gasInd, tr.valueInd))
                    {
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
                          getRemoteState(session, trHash, true);

                      // check that the post state qualifies to the expect
                      // section
                      scheme_state postState(remoteState.at("postState"));
                      CompareResult res = test::compareStates(
                          expect.getExpectState(), postState);
                      ETH_CHECK_MESSAGE(
                          res == CompareResult::Success,
                          "Network: " + net +
                              ", TrInfo: d: " + toString(tr.dataInd) +
                              ", g: " + toString(tr.gasInd) +
                              ", v: " + toString(tr.valueInd) + "\n");
                      if (res != CompareResult::Success)
                        _opt.wasErrors = true;

                      DataObject indexes;
                      DataObject transactionResults;
                      indexes["data"] = tr.dataInd;
                      indexes["gas"] = tr.gasInd;
                      indexes["value"] = tr.valueInd;

                      transactionResults["indexes"] = indexes;
                      transactionResults["hash"] =
                          remoteState.at("postHash").asString();
                      transactionResults["logs"] =
                          remoteState.at("logHash").asString();
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

std::mutex g_mutex;
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
        //        if (network == "Byzantium" || network == "Constantinople")
        //            continue;

        DataObject genesis = stateGenesis(network);
		genesis["genesis"] = test.getEnv().getDataForRPC();
                genesis["accounts"] = test.getPre().getDataForRPC(network);
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

					ETH_CHECK_MESSAGE(remoteState.at("postHash").asString() == expectHash,
						"Error at " + testInfo + ", post hash mismatch: " + remoteState.at("postHash").asString()
						+ ", expected: " + expectHash
						+ "\nState Dump: \n" + remoteState.at("postState").asJson());
					ETH_CHECK_MESSAGE(remoteState.at("logHash").asString() == expectLogHash,
						"Error at " + testInfo + ", logs hash mismatch: " + remoteState.at("logHash").asString()
						+ ", expected: " + expectLogHash);
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
    ETH_REQUIRE_MESSAGE(_input.type() == DataType::Object,
		TestOutputHelper::get().get().testFile().string() + " A GeneralStateTest file should contain an object.");
    ETH_REQUIRE_MESSAGE(!_opt.doFilling || _input.getSubObjects().size() == 1,
		TestOutputHelper::get().testFile().string() + " A GeneralStateTest filler should contain only one test.");

    DataObject filledTest;
	for (auto& i: _input.getSubObjects())
	{
		string const testname = i.getKey();
        ETH_REQUIRE_MESSAGE(i.type() == DataType::Object,
			TestOutputHelper::get().testFile().string() + " should contain an object under a test name.");
		DataObject const& inputTest = i;
        DataObject outputTest;
		outputTest.setKey(testname);

        if (_opt.doFilling && !TestOutputHelper::get().testFile().empty())
            ETH_REQUIRE_MESSAGE(testname + "Filler" == TestOutputHelper::get().testFile().stem().string(),
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

