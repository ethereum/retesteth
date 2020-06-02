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


/// Generate a blockchain test from state test filler
DataObject FillTestAsBlockchain(StateTestInFiller const& _test)
{
    DataObject filledTest;
    SessionInterface& session = RPCSession::instance(TestOutputHelper::getThreadID());
    std::vector<TransactionInGeneralSection> txs = _test.GeneralTr().buildTransactions();

    // run transactions on all networks that we need
    for (FORK const& fork : _test.getAllForksFromExpectSections())
    {
        // run transactions for defined expect sections only
        for (auto const& expect : _test.Expects())
        {
            // if expect section for this networks
            if (expect.forks().count(fork))
            {
                for (auto& tr : txs)
                {
                    TestInfo errorInfo(fork.asString(), tr.dataInd(), tr.gasInd(), tr.valueInd());
                    TestOutputHelper::get().setCurrentTestInfo(errorInfo);

                    if (!OptionsAllowTransaction(tr))
                    {
                        tr.markSkipped();
                        continue;
                    }

                    // if expect section is for this transaction
                    if (!expect.checkIndexes(tr.dataInd(), tr.gasInd(), tr.valueInd()))
                        continue;

                    DataObject request = prepareChainParams(fork, SealEngine::NoProof, _test.Pre(), _test.Env());
                    session.test_setChainParams(request);
                    session.test_modifyTimestamp(_test.Env().firstBlockTimestamp().asDecString());
                    FH32 trHash(session.eth_sendRawTransaction(tr.transaction().getSignedRLP()));

                    // Mine a block, execute transaction
                    session.test_mineBlocks(1);
                    VALUE latestBlockN(session.eth_blockNumber());
                    EthGetBlockBy remoteBlock(session.eth_getBlockByNumber(latestBlockN.asDecString(), Request::FULLOBJECTS));
                    ETH_ERROR_REQUIRE_MESSAGE(
                        remoteBlock.hasTransaction(trHash), "StateTest::FillTest: TR hash not found in mined block!");
                    tr.markExecuted();

                    // Mining reward
                    StateIncomplete mexpect(expect.result().asDataObject());  // make a copy of the state
                    ClientConfig const& cfg = Options::getDynamicOptions().getCurrentConfig();
                    ETH_ERROR_REQUIRE_MESSAGE(cfg.getRewardMap().count(fork),
                        "Network '" + fork.asString() + "' not found in correct mining info config (" +
                            cfg.getRewardMapPath().string() + ") Client: " + cfg.cfgFile().name());
                    VALUE balanceCorrection = cfg.getRewardMap().at(fork).getCContent();
                    mexpect.correctMiningReward(_test.Env().currentCoinbase(), balanceCorrection);

                    State postState(getRemoteState(session));
                    if (Options::get().fullstate)
                        compareStates(mexpect, postState);
                    else
                        compareStates(mexpect, session);

                    DataObject aBlockchainTest;
                    if (_test.hasInfo())
                        aBlockchainTest["_info"]["comment"] = _test.Info().comment();

                    EthGetBlockBy genesisBlock(session.eth_getBlockByNumber(0, Request::FULLOBJECTS));
                    aBlockchainTest["genesisBlockHeader"] = genesisBlock.header().asDataObject();
                    aBlockchainTest["pre"] = _test.Pre().asDataObject();
                    aBlockchainTest["postState"] = postState.asDataObject();
                    aBlockchainTest["network"] = fork.asString();
                    aBlockchainTest["sealEngine"] = sealEngineToStr(SealEngine::NoProof);
                    aBlockchainTest["lastblockhash"] = remoteBlock.header().hash().asString();
                    aBlockchainTest["genesisRLP"] = genesisBlock.getRLPHeaderTransactions().asString();

                    DataObject block;
                    block["rlp"] = remoteBlock.getRLPHeaderTransactions().asString();
                    block["blockHeader"] = remoteBlock.header().asDataObject();
                    block["transactions"].addArrayObject(tr.transaction().asDataObject());
                    block["uncleHeaders"] = DataObject(DataType::Array);
                    aBlockchainTest["blocks"].addArrayObject(block);

                    string dataPostfix =
                        "_d" + toString(tr.dataInd()) + "g" + toString(tr.gasInd()) + "v" + toString(tr.valueInd());
                    dataPostfix += "_" + fork.asString();

                    if (filledTest.count(_test.testName() + dataPostfix))
                        ETH_ERROR_MESSAGE("The test filler contain redundunt expect section: ");

                    filledTest[_test.testName() + dataPostfix] = aBlockchainTest;
                    session.test_rewindToBlock(0);
                }  // txs
            }      // if expect.count(fork)
        }          // expects
    }              // for each forks
    checkUnexecutedTransactions(txs);
    return filledTest;
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

                    session.test_modifyTimestamp(_test.Env().firstBlockTimestamp().asDecString());
                    FH32 trHash(session.eth_sendRawTransaction(tr.transaction().getSignedRLP()));
                    session.test_mineBlocks(1);
                    VALUE latestBlockN(session.eth_blockNumber());

                    EthGetBlockBy blockInfo(session.eth_getBlockByNumber(latestBlockN.asDecString(), Request::LESSOBJECTS));
                    ETH_ERROR_REQUIRE_MESSAGE(
                        blockInfo.hasTransaction(trHash), "StateTest::FillTest: TR hash not found in mined block!");
                    tr.markExecuted();

                    if (Options::get().poststate)
                        ETH_STDOUT_MESSAGE("PostState " + TestOutputHelper::get().testInfo().errorDebug() + " : \n" +
                                           blockInfo.header().stateRoot().asString());

                    if (Options::get().vmtrace)
                        printVmTrace(session, trHash, blockInfo.header().stateRoot());
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
                    transactionResults["hash"] = blockInfo.header().stateRoot().asString();

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
                    session.test_modifyTimestamp(_test.Env().firstBlockTimestamp().asDecString());
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
                        printVmTrace(session, trHash, blockInfo.header().stateRoot());

                    FH32 const& actualHash = blockInfo.header().stateRoot();
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


namespace test
{
DataObject StateTestSuite::doTests(DataObject const& _input, TestSuiteOptions& _opt) const
{
    TestOutputHelper::get().setCurrentTestInfo(TestInfo("StateTestSuite::doTests init"));
    if (_opt.doFilling)
    {
        DataObject filledTest;
        GeneralStateTestFiller filler(_input);
        StateTestInFiller const& test = filler.tests().at(0);
        checkTestNameIsEqualToFileName(test.testName());
        if (Options::get().fillchain)
            filledTest = FillTestAsBlockchain(test);
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
            if (_opt.isLegacyTests)
            {
                // Change the tests instead??
                DataObject& _inputRef = const_cast<DataObject&>(_input);
                DataObject& _infoRef = _inputRef.getSubObjectsUnsafe().at(0).atKeyUnsafe("_info");
                _infoRef.renameKey("filledwith", "filling-rpc-server");
                _infoRef["filling-tool-version"] = "testeth";
            }
            GeneralStateTest filledTest(_input);
            for (auto const& test : filledTest.tests())
            {
                RunTest(test);
                TestOutputHelper::get().registerTestRunSuccess();
            }
        }
    }
    return DataObject();
}
}// Namespace Close
