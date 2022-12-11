#include "StateTestsHelper.h"
#include "TestOutputHelper.h"
#include "Options.h"
#include "session/Session.h"
#include "testSuites/Common.h"
#include "testStructures/PrepareChainParams.h"

using namespace test;
using namespace std;
using namespace test::debug;
using namespace test::session;
using namespace test::statetests;

namespace test::statetests
{

/// Generate a blockchain test from state test filler
spDataObject FillTestAsBlockchain(StateTestInFiller const& _test)
{
    spDataObject filledTest;
    SessionInterface& session = RPCSession::instance(TestOutputHelper::getThreadID());
    std::vector<TransactionInGeneralSection> txs = _test.GeneralTr().buildTransactions();


    auto const allforks = _test.getAllForksFromExpectSections();
    if (hasSkipFork(allforks))
        return spDataObject(new DataObject(DataType::Null));

    // run transactions on all networks that we need
    for (FORK const& fork : allforks)
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
                    VALUE const& balanceCorrection = Options::getCurrentConfig().getRewardForFork(fork);
                    mexpect.correctMiningReward(_test.Env().currentCoinbase(), balanceCorrection);

                    spDataObject aBlockchainTest;
                    if (_test.hasInfo())
                        (*aBlockchainTest).atKeyPointer("_info") = _test.Info().rawData();
                    EthGetBlockBy genesisBlock(session.eth_getBlockByNumber(0, Request::FULLOBJECTS));
                    (*aBlockchainTest).atKeyPointer("genesisBlockHeader") = genesisBlock.header()->asDataObject();
                    (*aBlockchainTest).atKeyPointer("pre") = _test.Pre().asDataObject();

                    try
                    {
                        spState postState = getRemoteState(session);
                        compareStates(mexpect, postState);
                        (*aBlockchainTest).atKeyPointer("postState") = postState->asDataObject();
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
}
