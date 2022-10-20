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

#include "StateTests.h"
#include "StateTestsHelper.h"
#include <retesteth/Options.h>
#include <retesteth/TestOutputHelper.h>
#include <retesteth/session/Session.h>
#include <retesteth/testStructures/PrepareChainParams.h>
#include <retesteth/testStructures/structures.h>
#include <retesteth/testSuites/Common.h>
#include <retesteth/testSuites/blockchain/BlockchainTests.h>

using namespace std;
using namespace dev;
using namespace test;
using namespace test::debug;
using namespace test::teststruct;
using namespace test::session;
using namespace test::statetests;
namespace fs = boost::filesystem;

namespace
{
/// Generate a blockchain test from state test filler
spDataObject FillTestAsBlockchain(StateTestInFiller const& _test)
{
    spDataObject filledTest;
    SessionInterface& session = RPCSession::instance(TestOutputHelper::getThreadID());
    std::vector<TransactionInGeneralSection> txs = _test.GeneralTr().buildTransactions();

    // run transactions on all networks that we need
    for (FORK const& fork : _test.getAllForksFromExpectSections())
    {
        // run transactions for defined expect sections only
        for (auto const& expect : _test.Expects())
        {
            // if expect section for this networks
            if (expect.hasFork(fork))
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

                    session.test_setChainParams(
                        prepareChainParams(fork, SealEngine::NoProof, _test.Pre(), _test.Env(), ParamsContext::StateTests));
                    session.test_modifyTimestamp(_test.Env().firstBlockTimestamp());
                    modifyTransactionChainIDByNetwork(tr.transaction(), fork);
                    FH32 trHash(session.eth_sendRawTransaction(tr.transaction()->getRawBytes(), tr.transaction()->getSecret()));

                    // Mine a block, execute transaction
                    MineBlocksResult const mRes = session.test_mineBlocks(1);
                    string const& testException = expect.getExpectException(fork);
                    compareTransactionException(tr.transaction(), mRes, testException);

                    VALUE latestBlockN(session.eth_blockNumber());
                    EthGetBlockBy remoteBlock(session.eth_getBlockByNumber(latestBlockN, Request::FULLOBJECTS));
                    if (!remoteBlock.hasTransaction(trHash) && testException.empty())
                        ETH_ERROR_MESSAGE("StateTest::FillTest: " + c_trHashNotFound);
                    tr.markExecuted();

                    // Mining reward
                    spDataObject expectCopy;
                    (*expectCopy).copyFrom(expect.result().rawData());
                    StateIncomplete mexpect = StateIncomplete(dataobject::move(expectCopy));
                    ClientConfig const& cfg = Options::getDynamicOptions().getCurrentConfig();
                    ETH_ERROR_REQUIRE_MESSAGE(cfg.getRewardMap().count(fork),
                        "Network '" + fork.asString() + "' not found in correct mining info config (" +
                            cfg.getRewardMapPath().string() + ") Client: " + cfg.cfgFile().name());
                    VALUE const& balanceCorrection = cfg.getRewardMap().at(fork).getCContent();
                    mexpect.correctMiningReward(_test.Env().currentCoinbase(), balanceCorrection);

                    spDataObject aBlockchainTest;
                    if (_test.hasInfo())
                        (*aBlockchainTest).atKeyPointer("_info") = _test.Info().rawData();
                    EthGetBlockBy genesisBlock(session.eth_getBlockByNumber(0, Request::FULLOBJECTS));
                    (*aBlockchainTest).atKeyPointer("genesisBlockHeader") = genesisBlock.header()->asDataObject();
                    (*aBlockchainTest).atKeyPointer("pre") = _test.Pre().asDataObject();

                    try
                    {
                        State postState(getRemoteState(session));
                        compareStates(mexpect, postState);
                        (*aBlockchainTest).atKeyPointer("postState") = postState.asDataObject();
                    }
                    catch (StateTooBig const&)
                    {
                        compareStates(mexpect, session);
                        (*aBlockchainTest)["postStateHash"] = remoteBlock.header()->stateRoot().asString();
                    }

                    (*aBlockchainTest)["network"] = fork.asString();
                    (*aBlockchainTest)["sealEngine"] = sealEngineToStr(SealEngine::NoProof);
                    (*aBlockchainTest)["lastblockhash"] = remoteBlock.header()->hash().asString();
                    (*aBlockchainTest)["genesisRLP"] = genesisBlock.getRLPHeaderTransactions().asString();

                    spDataObject block;
                    (*block)["rlp"] = remoteBlock.getRLPHeaderTransactions().asString();
                    (*block).atKeyPointer("blockHeader") = remoteBlock.header()->asDataObject();
                    (*block).atKeyPointer("transactions") = spDataObject(new DataObject(DataType::Array));
                    if (testException.empty())
                    {
                        (*block)["transactions"].addArrayObject(tr.transaction()->asDataObject());
                        auto sender = _test.GeneralTr().getSender();
                        if (!sender.isEmpty())
                            (*block)["transactions"].atLastElementUnsafe()["sender"] = sender->asString();
                    }
                    (*block).atKeyPointer("uncleHeaders") = spDataObject(new DataObject(DataType::Array));

                    if (!testException.empty())
                    {
                        spDataObject trInfo;
                        (*trInfo)["valid"] = "false";
                        (*trInfo)["rawBytes"] = tr.transaction()->getRawBytes().asString();
                        (*trInfo)["exception"] = testException;
                        (*block)["transactionSequence"].addArrayObject(trInfo);
                    }
                    (*aBlockchainTest)["blocks"].addArrayObject(block);

                    string dataPostfix = "_d" + tr.dataIndS() + "g" + tr.gasIndS() + "v" + tr.valueIndS();
                    dataPostfix += "_" + fork.asString();

                    if (filledTest->count(_test.testName() + dataPostfix))
                        ETH_ERROR_MESSAGE("The test filler contain redundant expect section: " + _test.testName() +
                                          dataPostfix + " (" + tr.transaction()->dataLabel() + ")");

                    verifyFilledTest(_test.unitTestVerifyBC(), aBlockchainTest, fork);
                    (*filledTest).atKeyPointer(_test.testName() + dataPostfix) = aBlockchainTest;
                    session.test_rewindToBlock(0);
                }  // txs
            }      // if expect.count(fork)
        }          // expects
    }              // for each forks
    checkUnexecutedTransactions(txs);
    return filledTest;
}

/// Rewrite the test file. Fill General State Test
spDataObject FillTest(StateTestInFiller const& _test)
{
    spDataObject filledTest;
    TestOutputHelper::get().setCurrentTestName(_test.testName());

    SessionInterface& session = RPCSession::instance(TestOutputHelper::getThreadID());

    if (_test.hasInfo())
        (*filledTest).atKeyPointer("_info") = _test.Info().rawData();
    (*filledTest).atKeyPointer("env") = _test.Env().asDataObject();
    (*filledTest).atKeyPointer("pre") = _test.Pre().asDataObject();
    (*filledTest).atKeyPointer("transaction") = _test.GeneralTr().asDataObject();

    // Gather Transactions from general transaction section
    std::vector<TransactionInGeneralSection> txs = _test.GeneralTr().buildTransactions();
    for (auto const& tx : txs)
    {
        // Fill up the label map to tx.data
        if (!tx.transaction()->dataLabel().empty())
        {
            static string const removePrefix = ":label ";
            string label = tx.transaction()->dataLabel();
            size_t const pos = label.find(removePrefix);
            if (pos != string::npos)
                label.erase(pos, removePrefix.length());
            if (!(*filledTest)["_info"]["labels"].count(tx.dataIndS()))
                (*filledTest)["_info"]["labels"].addSubObject(spDataObject(new DataObject(tx.dataIndS(), label)));
        }
    }

    // run transactions on all networks that we need
    for (auto const& fork : _test.getAllForksFromExpectSections())  // object constructed!!!
    {
        // Skip by --singlenet option
        bool networkSkip = false;
        Options const& opt = Options::get();

        if ((!opt.singleTestNet.empty() && FORK(opt.singleTestNet) != fork) ||
            !Options::getDynamicOptions().getCurrentConfig().checkForkAllowed(fork))
            networkSkip = true;

        spDataObject forkResults;
        (*forkResults).setKey(fork.asString());

        if (!networkSkip)
        {
            auto const p = prepareChainParams(fork, SealEngine::NoReward, _test.Pre(), _test.Env(), ParamsContext::StateTests);
            session.test_setChainParams(p);
        }

        // Run transactions for defined expect sections only
        for (auto const& expect : _test.Expects())
        {
            // if expect section for this networks
            if (expect.hasFork(fork))
            {
                bool expectFoundTransaction = false;
                for (auto& tr : txs)
                {
                    TestInfo errorInfo(fork.asString(), tr.dataInd(), tr.gasInd(), tr.valueInd());
                    if (!tr.transaction()->dataLabel().empty() || !tr.transaction()->dataRawPreview().empty())
                        errorInfo.setTrDataDebug(
                            tr.transaction()->dataLabel() + " " + tr.transaction()->dataRawPreview() + "..");

                    TestOutputHelper::get().setCurrentTestInfo(errorInfo);

                    bool expectChekIndexes = expect.checkIndexes(tr.dataInd(), tr.gasInd(), tr.valueInd());
                    if (!OptionsAllowTransaction(tr) || networkSkip)
                    {
                        tr.markSkipped();

                        if (expectChekIndexes)
                            expectFoundTransaction = true;
                        continue;
                    }

                    // if expect section is not for this transaction
                    if (!expectChekIndexes)
                        continue;

                    expectFoundTransaction = true;
                    session.test_modifyTimestamp(_test.Env().firstBlockTimestamp());

                    modifyTransactionChainIDByNetwork(tr.transaction(), fork);
                    FH32 trHash(session.eth_sendRawTransaction(tr.transaction()->getRawBytes(), tr.transaction()->getSecret()));

                    MineBlocksResult const mRes = session.test_mineBlocks(1);
                    string const& testException = expect.getExpectException(fork);
                    compareTransactionException(tr.transaction(), mRes, testException);

                    VALUE latestBlockN(session.eth_blockNumber());
                    EthGetBlockBy blockInfo(session.eth_getBlockByNumber(latestBlockN, Request::LESSOBJECTS));
                    if (!blockInfo.hasTransaction(trHash) && testException.empty())
                        ETH_ERROR_MESSAGE("StateTest::FillTest: " + c_trHashNotFound);
                    tr.markExecuted();

                    if (Options::get().poststate)
                        ETH_DC_MESSAGE(DC::STATE, "PostState " + TestOutputHelper::get().testInfo().errorDebug() + " : \n" +
                                                      cDefault + "Hash: " + blockInfo.header()->stateRoot().asString());

                    if (Options::get().vmtrace)
                    {
                        string const testNameOut = _test.testName() + "_d" + tr.dataIndS() + "g" + tr.gasIndS() + "v" +
                                                   tr.valueIndS() + "_" + fork.asString() + "_" + trHash.asString() + ".txt";
                        VMtraceinfo info(session, trHash, blockInfo.header()->stateRoot(), testNameOut);
                        printVmTrace(info);
                    }

                    spDataObject transactionResults;
                    try
                    {
                        auto const remState = getRemoteState(session);
                        compareStates(expect.result(), remState);
                        if (Options::get().poststate)
                            (*transactionResults).atKeyPointer("postState") = remState.asDataObject();
                    }
                    catch (StateTooBig const&)
                    {
                        compareStates(expect.result(), session);
                    }

                    spDataObject indexes;
                    (*indexes)["data"] = tr.dataInd();
                    (*indexes)["gas"] = tr.gasInd();
                    (*indexes)["value"] = tr.valueInd();

                    (*transactionResults).atKeyPointer("indexes") = indexes;
                    (*transactionResults)["hash"] = blockInfo.header()->stateRoot().asString();
                    (*transactionResults)["txbytes"] = tr.transaction()->getRawBytes().asString();
                    if (!testException.empty())
                        (*transactionResults)["expectException"] = testException;

                    // Fill up the loghash (optional)
                    if (Options::getDynamicOptions().getCurrentConfig().cfgFile().checkLogsHash())
                    {
                        FH32 logHash(session.test_getLogHash(trHash));
                        if (!logHash.isZero())
                            (*transactionResults)["logs"] = logHash.asString();
                    }

                    (*forkResults).addArrayObject(transactionResults);
                    session.test_rewindToBlock(VALUE(0));
                }  // tx

                if (expectFoundTransaction == false)
                {
                    ETH_ERROR_MESSAGE("Expect section does not cover any transaction: \n" + expect.initialData().asJson() +
                                      "\n" + expect.result().asDataObject()->asJson());
                }
            }  // expect has fork
        }

        if (forkResults->getSubObjects().size() > 0)
            (*filledTest)["post"].addSubObject(forkResults);
    }

    checkUnexecutedTransactions(txs);
    verifyFilledTest(_test.unitTestVerify(), filledTest);
    return filledTest;
}

}  // namespace


namespace test
{
spDataObject StateTestSuite::doTests(spDataObject& _input, TestSuiteOptions& _opt) const
{
    TestOutputHelper::get().setCurrentTestInfo(TestInfo("StateTestSuite::doTests init"));
    if (_opt.doFilling)
    {
        spDataObject filledTest;
        GeneralStateTestFiller filler(_input);
        StateTestInFiller const& test = filler.tests().at(0);
        checkTestNameIsEqualToFileName(test.testName());
        if (Options::get().fillchain)
            filledTest = FillTestAsBlockchain(test);
        else
            (*filledTest).addSubObject(test.testName(), FillTest(test));

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
            TestOutputHelper::get().setCurrentTestInfo(TestInfo("Parsing generalstate test"));
            TestOutputHelper::get().setCurrentTestName("----");
            GeneralStateTest filledTest(_input);

            // Just check the test structure if running with --checkhash
            if (Options::get().checkhash)
                return spDataObject();

            for (auto const& test : filledTest.tests())
            {
                RunTest(test);
                TestOutputHelper::get().registerTestRunSuccess();
            }
        }
    }
    return spDataObject();
}
}  // namespace test
