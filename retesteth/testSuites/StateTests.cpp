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
#include <retesteth/TestHelper.h>
#include <retesteth/TestOutputHelper.h>
#include <retesteth/TestSuite.h>
#include <retesteth/ethObjects/common.h>
#include <retesteth/session/RPCSession.h>
#include <retesteth/testSuites/Common.h>
#include <retesteth/testSuites/StateTests.h>
#include <retesteth/testSuites/blockchain/BlockchainTests.h>

#include <retesteth/testStructures/Common.h>

using namespace std;
using namespace dev;
using namespace test::teststruct;
namespace fs = boost::filesystem;

namespace
{

bool OptionsAllowTransaction(test::teststruct::TransactionInGeneralSection const& _tr)
{
    Options const& opt = Options::get();
    if ((opt.trDataIndex == (int)_tr.dataInd() || opt.trDataIndex == -1) &&
        (opt.trGasIndex == (int)_tr.gasInd() || opt.trGasIndex == -1) &&
        (opt.trValueIndex == (int)_tr.valueInd() || opt.trValueIndex == -1))
        return true;
    return false;
}

/// Generate a blockchain test from state test filler
DataObject FillTestAsBlockchain(StateTestInFiller const& _test)
{
    (void)_test;
    DataObject filledTest;
    /*
    test::scheme_stateTestFiller test(_testFile);

    SessionInterface& session = RPCSession::instance(TestOutputHelper::getThreadID());
    // run transactions on all networks that we need
    for (auto const& net : test.getExpectSection().getAllNetworksFromExpectSection())
    {
        // run transactions for defined expect sections only
        for (auto const& expect : test.getExpectSection().getExpectSections())
        {
            // if expect section for this networks
            if (expect.getNetworks().count(net))
            {
                for (auto& tr : test.getTransactionsUnsafe())
                {
                    TestInfo errorInfo (net, tr.dataInd, tr.gasInd, tr.valueInd);
                    TestOutputHelper::get().setCurrentTestInfo(errorInfo);

                    if (!OptionsAllowTransaction(tr))
                    {
                        tr.skipped = true;
                        continue;
                    }

                    // if expect section is for this transaction
                    if (!expect.checkIndexes(tr.dataInd, tr.gasInd, tr.valueInd))
                        continue;

                    // State Tests does not have mining rewards
                    scheme_expectSectionElement mexpect = expect;
                    mexpect.correctMiningReward(net, test.getEnv().getCoinbase());

                    string sEngine = scheme_blockchainTestBase::m_sNoProof;
                    session.test_setChainParams(test.getGenesisForRPC(net, sEngine));
                    u256 a(test.getEnv().getData().atKey("currentTimestamp").asString());
                    session.test_modifyTimestamp(a.convert_to<size_t>());
                    string trHash = session.eth_sendRawTransaction(tr.transaction);

                    if (session.getLastRPCError().type() != DataType::Null)
                        ETH_ERROR_MESSAGE(session.getLastRPCError().atKey("message").asString());
                    if (!isHash<h256>(trHash))
                        ETH_ERROR_MESSAGE("eth_sendRawTransaction return invalid hash: '" + trHash);

                    string latestBlockNumber = session.test_mineBlocks(1);
                    tr.executed = true;

                    scheme_RPCBlock remoteBlock =
                        session.eth_getBlockByNumber(latestBlockNumber, true);
                    scheme_state remoteState = getRemoteState(session, remoteBlock);
                    if (remoteState.isHash())
                        compareStates(mexpect.getExpectState(), session, remoteBlock);
                    else
                        compareStates(mexpect.getExpectState(), remoteState);

                    DataObject aBlockchainTest;
                    if (test.getData().count("_info"))
                        aBlockchainTest["_info"] = test.getData().atKey("_info");

                    test::scheme_RPCBlock latestBlock =
                        session.eth_getBlockByNumber(BlockNumber("0"), false);
                    aBlockchainTest["genesisBlockHeader"] = latestBlock.getBlockHeader();
                    aBlockchainTest["genesisBlockHeader"].removeKey("transactions");
                    aBlockchainTest["genesisBlockHeader"].removeKey("uncles");

                    aBlockchainTest["pre"] = test.getPre().getData();
                    if (remoteState.isHash())
                        aBlockchainTest["postStateHash"] = remoteState.getData();
                    else
                        aBlockchainTest["postState"] = remoteState.getData();
                    aBlockchainTest["network"] = net;
                    aBlockchainTest["sealEngine"] = sEngine;
                    aBlockchainTest["lastblockhash"] = remoteBlock.getBlockHash();

                    test::scheme_RPCBlock genesisBlock =
                        session.eth_getBlockByNumber(BlockNumber("0"), true);
                    aBlockchainTest["genesisRLP"] = genesisBlock.getBlockRLP();

                    DataObject block;
                    block["rlp"] = remoteBlock.getBlockRLP();
                    block["blockHeader"] = remoteBlock.getBlockHeader();
                    block["transactions"].addArrayObject(tr.transaction.getDataForBCTest());
                    ETH_ERROR_REQUIRE_MESSAGE(remoteBlock.getTransactionCount() == 1,
                        "Failed to execute transaction on remote client! State test transaction must be valid!");
                    aBlockchainTest["blocks"].addArrayObject(block);

                    string dataPostfix = "_d" + toString(tr.dataInd) + "g" + toString(tr.gasInd) +
                                         "v" + toString(tr.valueInd);
                    dataPostfix += "_" + net;

                    if (filledTest.count(_testFile.getKey() + dataPostfix))
                        ETH_ERROR_MESSAGE("The test filler contain redundunt expect section: ");

                    filledTest[_testFile.getKey() + dataPostfix] = aBlockchainTest;
                    session.test_rewindToBlock(0);
                }
            }
        }
        test.checkUnexecutedTransactions();
    }
    return filledTest;
    */
    return filledTest;
}

void checkUnexecutedTransactions(std::vector<TransactionInGeneralSection> const& _txs)
{
    bool atLeastOneExecuted = false;
    bool atLeastOneWithoutExpectSection = false;
    for (auto const& tr : _txs)
    {
        if (tr.getExecuted())
            atLeastOneExecuted = true;
        bool transactionExecutedOrSkipped = tr.getExecuted() || tr.getSkipped();
        atLeastOneWithoutExpectSection = !transactionExecutedOrSkipped || atLeastOneWithoutExpectSection;
        if (!transactionExecutedOrSkipped || atLeastOneWithoutExpectSection)
        {
            TestInfo errorInfo("all", (int)tr.dataInd(), (int)tr.gasInd(), (int)tr.valueInd());
            TestOutputHelper::get().setCurrentTestInfo(errorInfo);
            ETH_MARK_ERROR("Test has transaction uncovered with expect section!");
        }
    }
    if (!atLeastOneExecuted)
    {
        Options const& opt = Options::get();
        TestInfo errorInfo(
            opt.singleTestNet.empty() ? "N/A" : opt.singleTestNet, opt.trDataIndex, opt.trGasIndex, opt.trValueIndex);
        TestOutputHelper::get().setCurrentTestInfo(errorInfo);
    }
    ETH_ERROR_REQUIRE_MESSAGE(atLeastOneExecuted, "Specified filter did not run a single transaction! ");
}

/// Rewrite the test file. Fill General State Test
DataObject FillTest(StateTestInFiller const& _test)
{
    DataObject filledTest;
    filledTest.setAutosort(true);

    SessionInterface& session = RPCSession::instance(TestOutputHelper::getThreadID());

    if (_test.hasInfo())
        filledTest["_info"]["comment"] = _test.Info().comment();

    filledTest["env"] = _test.Env().asDataObject();
    filledTest["pre"] = _test.Pre().asDataObject();
    filledTest["transaction"] = _test.GeneralTr().asDataObject();

    // Gather Transactions from general transaction section
    std::vector<TransactionInGeneralSection> txs = _test.GeneralTr().buildTransactions();

    // run transactions on all networks that we need
    for (auto const& fork : _test.getAllForksFromExpectSections())  // object constructed!!!
    {
        // Skip by --singlenet option
        bool networkSkip = false;
        Options const& opt = Options::get();
        if ((!opt.singleTestNet.empty() && FORK(opt.singleTestNet) != fork) ||
            !Options::getDynamicOptions().getCurrentConfig().checkForkAllowed(fork))
            networkSkip = true;

        DataObject forkResults;
        forkResults.setKey(fork.asString());

        DataObject request = prepareChainParams(fork, SealEngine::NoReward, _test.Pre(), _test.Env());
        session.test_setChainParams(request);

        // Run transactions for defined expect sections only
        for (auto const& expect : _test.Expects())
        {
            // if expect section for this networks
            if (expect.forks().count(fork))
            {
                for (auto& tr : txs)
                {
                    TestInfo errorInfo(fork.asString(), tr.dataInd(), tr.gasInd(), tr.valueInd());
                    TestOutputHelper::get().setCurrentTestInfo(errorInfo);

                    if (!OptionsAllowTransaction(tr) || networkSkip)
                    {
                        tr.markSkipped();
                        continue;
                    }

                    // if expect section is not for this transaction
                    if (!expect.checkIndexes(tr.dataInd(), tr.gasInd(), tr.valueInd()))
                        continue;

                    session.test_modifyTimestamp(_test.Env().currentTimestamp().asDecString());
                    FH32 trHash(session.eth_sendRawTransaction(tr.transaction().getSignedRLP()));
                    session.test_mineBlocks(1);
                    VALUE latestBlockN(session.eth_blockNumber());

                    EthGetBlockBy blockInfo(session.eth_getBlockByNumber(latestBlockN.asDecString(), Request::LESSOBJECTS));
                    ETH_ERROR_REQUIRE_MESSAGE(
                        blockInfo.hasTransaction(trHash), "StateTest::FillTest: TR hash not found in mined block!");
                    tr.markExecuted();

                    if (Options::get().poststate)
                        ETH_STDOUT_MESSAGE("PostState " + TestOutputHelper::get().testInfo().errorDebug() + " : \n" +
                                           blockInfo.stateRoot().asString());

                    if (Options::get().vmtrace)
                        printVmTrace(session, trHash, blockInfo.stateRoot());
                    if (Options::get().fullstate)
                        compareStates(expect.result(), getRemoteState(session));
                    else
                        compareStates(expect.result(), session);

                    DataObject indexes;
                    DataObject transactionResults;
                    indexes["data"] = tr.dataInd();
                    indexes["gas"] = tr.gasInd();
                    indexes["value"] = tr.valueInd();

                    transactionResults["indexes"] = indexes;
                    transactionResults["hash"] = blockInfo.stateRoot().asString();

                    // Fill up the loghash (optional)
                    FH32 logHash(session.test_getLogHash(trHash));
                    if (!logHash.isZero())
                        transactionResults["logs"] = logHash.asString();

                    forkResults.addArrayObject(transactionResults);
                    session.test_rewindToBlock(VALUE(0));
                }
            }
        }
        filledTest["post"].addSubObject(forkResults);
    }

    FillTestAsBlockchain(DataObject());
    checkUnexecutedTransactions(txs);
    return filledTest;
}

/// Read and execute the test file
void RunTest(StateTestInFilled const& _test)
{
    SessionInterface& session = RPCSession::instance(TestOutputHelper::getThreadID());

    // Gather Transactions from general transaction section
    std::vector<TransactionInGeneralSection> txs = _test.GeneralTr().buildTransactions();

    for (auto const& post : _test.Post())
    {
        bool networkSkip = false;
        FORK const& network = post.first;
        Options const& opt = Options::get();

        // If options singlenet select different network or test has network that is not allowed by clinet configs
        if ((!opt.singleTestNet.empty() && FORK(opt.singleTestNet) != network) ||
            !Options::getDynamicOptions().getCurrentConfig().checkForkAllowed(network))
            networkSkip = true;
        else
        {
            DataObject request = prepareChainParams(network, SealEngine::NoReward, _test.Pre(), _test.Env());
            session.test_setChainParams(request);
        }

        // One test could have many transactions on same chainParams
        // It is expected that for a setted chainParams there going to be a transaction
        // Rather then all transactions would be filtered out and not executed at all

        // read all results for a specific fork
        for (StateTestPostResult const& result : post.second)
        {
            bool resultHaveCorrespondingTransaction = false;
            // look for a transaction with this indexes and execute it on a client
            for (TransactionInGeneralSection& tr : txs)
            {
                TestInfo errorInfo(network.asString(), tr.dataInd(), tr.gasInd(), tr.valueInd());
                TestOutputHelper::get().setCurrentTestInfo(errorInfo);
                bool checkIndexes = result.checkIndexes(tr.dataInd(), tr.gasInd(), tr.valueInd());
                if (checkIndexes)
                    resultHaveCorrespondingTransaction = true;

                if (!OptionsAllowTransaction(tr) || networkSkip)
                {
                    tr.markSkipped();
                    continue;
                }

                if (checkIndexes)
                {
                    session.test_modifyTimestamp(_test.Env().currentTimestamp().asDecString());
                    FH32 trHash(session.eth_sendRawTransaction(tr.transaction().getSignedRLP()));
                    session.test_mineBlocks(1);

                    VALUE latestBlockN(session.eth_blockNumber());
                    EthGetBlockBy blockInfo(session.eth_getBlockByNumber(latestBlockN.asDecString(), Request::LESSOBJECTS));
                    ETH_ERROR_REQUIRE_MESSAGE(
                        blockInfo.hasTransaction(trHash), "StateTest::RunTest: TR hash not found in mined block!");
                    tr.markExecuted();

                    // Validate post state
                    FH32 const& expectedPostHash = result.hash();
                    if (Options::get().vmtrace && !Options::get().filltests)
                        printVmTrace(session, trHash, blockInfo.stateRoot());

                    FH32 const& actualHash = blockInfo.stateRoot();
                    if (actualHash != expectedPostHash)
                    {
                        if (Options::get().logVerbosity >= 5)
                            ETH_LOG("\nState Dump: \n" + getRemoteState(session).asDataObject().asJson(), 5);
                        ETH_ERROR_MESSAGE("Post hash mismatch remote: " + actualHash.asString() +
                                          ", expected: " + expectedPostHash.asString());
                    }

                    // Validate log hash
                    FH32 const& expectedLogHash = result.logs();
                    FH32 remoteLogHash(session.test_getLogHash(trHash));
                    if (remoteLogHash != expectedLogHash)
                        ETH_ERROR_MESSAGE("Logs hash mismatch: '" + remoteLogHash.asString() + "', expected: '" +
                                          expectedLogHash.asString() + "'");

                    session.test_rewindToBlock(0);
                    if (Options::get().logVerbosity >= 5)
                        ETH_LOG("Executed: d: " + to_string(tr.dataInd()) + ", g: " + to_string(tr.gasInd()) +
                                    ", v: " + to_string(tr.valueInd()) + ", fork: " + network.asString(),
                            5);
                }
            } //ForTransactions

            ETH_ERROR_REQUIRE_MESSAGE(resultHaveCorrespondingTransaction,
                "Test `post` section has expect section without corresponding transaction!" + result.asDataObject().asJson());
        }
    }

    checkUnexecutedTransactions(txs);
}
}  // namespace closed

#include <retesteth/testStructures/Common.h>
using namespace test::teststruct;

namespace test
{
DataObject StateTestSuite::doTests(DataObject const& _input, TestSuiteOptions& _opt) const
{
    TestOutputHelper::get().setCurrentTestInfo(TestInfo("StateTestSuite::doTests init"));
    if (_opt.doFilling)
    {
        DataObject filledTest;
        GeneralStateTestFiller filler(_input);

        ETH_ERROR_REQUIRE_MESSAGE(filler.tests().size() == 1,
            "StateTest filler must have only one test: " + TestOutputHelper::get().testFile().string());

        StateTestInFiller const& test = filler.tests().at(0);
        checkTestNameIsEqualToFileName(test.testName());
        if (Options::get().fillchain)
            filledTest.addSubObject(test.testName(), FillTestAsBlockchain(test));
        else
            filledTest.addSubObject(test.testName(), FillTest(test));

        TestOutputHelper::get().registerTestRunSuccess();
        return filledTest;
    }
    else
    {
        if (Options::get().fillchain)
        {
            BlockchainTestValidSuite bcTestSuite;
            bcTestSuite.doTests(_input, _opt);
        }
        else
        {
            GeneralStateTest filledTest(_input);
            for (auto const& test : filledTest.tests())
                RunTest(test);
        }
        TestOutputHelper::get().registerTestRunSuccess();
    }
    return DataObject();
}
}// Namespace Close
