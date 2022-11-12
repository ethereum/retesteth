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
        {
            auto const filled = FillTest(test);
            if (filled->type() != DataType::Null)
                (*filledTest).addSubObject(test.testName(), filled);
            else
                return filled;
        }

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
