#include "StateTestChainRunner.h"
#include "StateTestsHelper.h"
#include <retesteth/Options.h>
#include <retesteth/session/Session.h>
#include <retesteth/helpers/TestOutputHelper.h>
#include <retesteth/testSuites/Common.h>
#include <retesteth/testStructures/PrepareChainParams.h>
#include <retesteth/testSuites/blockchain/fillers/TestBlock.h>
#include <retesteth/Constants.h>

using namespace std;
using namespace test;
using namespace test::debug;
using namespace test::teststruct;
using namespace test::session;
namespace fs = boost::filesystem;


namespace test::statetests
{

StateTestChainRunner::StateTestChainRunner(StateTestInFiller const& _test, TestSuite::TestSuiteOptions& _opt)
  : StateTestFillerRunner(_test, RPCSession::instance(TestOutputHelper::getThreadID()), _opt),
    m_testSuiteOpt(_opt)
{
    TestOutputHelper::get().setCurrentTestName(_test.testName());
    m_txs = _test.GeneralTr().buildTransactions();
    m_filledBlockchainTest = sDataObject(DataType::Object);
}

void StateTestChainRunner::prepareChainParams(FORK const& _network)
{
    TestInfo errorInfo("test_setChainParams: " + _network.asString(), m_test.testName());
    TestOutputHelper::get().setCurrentTestInfo(errorInfo);



    std::vector<spAccountBase> additionalAccounts;
    if (compareFork(_network, CMP::ge, FORK("Cancun")))
        makeCancunPrecompiledAccounts(m_test.Pre(), m_test.Env().currentTimestamp(), additionalAccounts);

    if (compareFork(_network, CMP::ge, FORK("Prague")))
        makePraguePrecompiledAccounts(m_test.Pre(), additionalAccounts);

    if (!additionalAccounts.empty())
    {
        m_statePreModified = spState(new State(m_test.Pre()));
        for (auto account : additionalAccounts)
            (*m_statePreModified).addAccount(account);
    }


    auto const p = test::teststruct::prepareChainParams(_network, SealEngine::NoProof, m_statePreModified.isEmpty() ? m_test.Pre() : m_statePreModified, m_test.Env(), ParamsContext::StateTests);
    m_session.test_setChainParams(p);
}

void StateTestChainRunner::performTransactionOnExpect(TransactionInGeneralSection& _tr, StateTestFillerExpectSection const& _expect, FORK const& _network)
{
    auto const& ethTr = _tr.transaction();
    m_session.test_modifyTimestamp(m_test.Env().firstBlockTimestamp());
    modifyTransactionChainIDByNetwork(ethTr, _network);
    FH32 trHash(m_session.eth_sendRawTransaction(ethTr->getRawBytes(), ethTr->getSecret()));

    MineBlocksResult const mRes = m_session.test_mineBlocks(1);
    string const& testException = _expect.getExpectException(_network);
    compareTransactionException(ethTr, mRes, testException);

    VALUE latestBlockN(m_session.eth_blockNumber());
    EthGetBlockBy remoteBlock(m_session.eth_getBlockByNumber(latestBlockN, Request::FULLOBJECTS));
    if (!remoteBlock.hasTransaction(trHash) && testException.empty())
        ETH_ERROR_MESSAGE("StateTest::FillTest: " + c_trHashNotFound);

    _tr.markExecuted();
    _tr.assignTransactionHash(trHash);

    // Fill in blockchain test data
    (*m_aBlockchainTest)["network"] = _network.asString();
    (*m_aBlockchainTest)["postStateHash"] = remoteBlock.header()->stateRoot().asString();
    (*m_aBlockchainTest)["lastblockhash"] = remoteBlock.header()->hash().asString();
    if (m_test.hasConfig())
    {
        (*m_aBlockchainTest).atKeyPointer("config") = m_test.Config().asDataObject(_network);
        (*m_aBlockchainTest)["config"]["network"] = _network.asString();
    }


    spDataObject block;
    (*block)["rlp"] = remoteBlock.getRLPHeaderTransactions().asString();
    if (_tr.transaction()->hasBigInt())
        (*block)["hasBigInt"] = "true";
    (*block).atKeyPointer("blockHeader") = remoteBlock.header()->asDataObject();
    (*block).atKeyPointer("transactions") = spDataObject(new DataObject(DataType::Array));
    if (testException.empty())
    {
        (*block)["transactions"].addArrayObject(ethTr->asDataObject());
        auto sender = m_test.GeneralTr().getSender();
        if (!sender.isEmpty())
            (*block)["transactions"].atLastElementUnsafe()["sender"] = sender->asString();
    }
    (*block).atKeyPointer("uncleHeaders") = sDataObject(DataType::Array);
    if (isBlockExportWithdrawals(remoteBlock.header()))
        (*block).atKeyPointer("withdrawals") = sDataObject(DataType::Array);

    if (!testException.empty())
    {
        (*block).clearSubobjects();
        spDataObject trInfo;
        (*trInfo)["valid"] = "false";
        (*trInfo)["rawBytes"] = ethTr->getRawBytes().asString();
        (*trInfo)["exception"] = testException;
        (*block)["transactionSequence"].addArrayObject(trInfo);
        (*block)["expectException"] = testException;
        if (_tr.transaction()->hasBigInt())
            (*block)["hasBigInt"] = "true";

        EthereumBlock managedBlock(remoteBlock.header());
        managedBlock.addTransaction(_tr.transaction());
        (*block)["rlp"] = managedBlock.getRLP().asString();
        blockchainfiller::TestBlock::exportRLPDecodedToData(managedBlock.getRLP(), *block);
        (*m_aBlockchainTest)["blocks"].addArrayObject(block);

        // BUT POST INFO must be of block number 0
        m_session.test_rewindToBlock(0);
        EthGetBlockBy remoteBlock(m_session.eth_getBlockByNumber(0, Request::LESSOBJECTS));
        (*m_aBlockchainTest)["postStateHash"] = remoteBlock.header()->stateRoot().asString();
        (*m_aBlockchainTest)["lastblockhash"] = remoteBlock.header()->hash().asString();
    }
    else
        (*m_aBlockchainTest)["blocks"].addArrayObject(block);
}

spStateIncomplete StateTestChainRunner::correctMiningReward(StateTestFillerExpectSection const& _expect, FORK const& _network)
{
    spDataObject expectCopy = _expect.result().asDataObject();
    spStateIncomplete mexpect(new StateIncomplete(dataobject::move(expectCopy)));
    VALUE const& balanceCorrection = Options::getCurrentConfig().getRewardForFork(_network);
    (*mexpect).correctMiningReward(m_test.Env().currentCoinbase(), balanceCorrection);
    return mexpect;
}

void StateTestChainRunner::initBlockchainTestData()
{
    m_aBlockchainTest = spDataObject();
    if (m_test.hasInfo())
        (*m_aBlockchainTest).atKeyPointer("_info") = m_test.Info().rawData();
    EthGetBlockBy genesisBlock(m_session.eth_getBlockByNumber(0, Request::FULLOBJECTS));
    (*m_aBlockchainTest).atKeyPointer("genesisBlockHeader") = genesisBlock.header()->asDataObject();
    (*m_aBlockchainTest).atKeyPointer("pre") = m_statePreModified.isEmpty() ? m_test.Pre().asDataObject() : m_statePreModified->asDataObject();
    (*m_aBlockchainTest)["sealEngine"] = sealEngineToStr(SealEngine::NoProof);
    (*m_aBlockchainTest)["genesisRLP"] = genesisBlock.getRLPHeaderTransactions().asString();
}

void StateTestChainRunner::finalizeBlockchainTestData(TransactionInGeneralSection& _tr, FORK const& _network)
{
    string testName;
    auto const& opt = Options::get();
    if (opt.filleest || true)
    {
        testName = m_testSuiteOpt.relativePathToFilledTest.string();
        testName += "::" + m_test.testName() + "-fork_" + _network.asString();
        testName += "-d" + _tr.dataIndS() + "g" + _tr.gasIndS() + "v" + _tr.valueIndS();
        // <relative-path-of-fixture-json>::<fixture-name>-fork_<fork-name>-d<X>g<Y>v<Z>
    }
    else
    {
        string dataPostfix = "_d" + _tr.dataIndS() + "g" + _tr.gasIndS() + "v" + _tr.valueIndS();
        dataPostfix += "_" + _network.asString();
        testName = m_test.testName() + dataPostfix;
    }
    if (m_filledBlockchainTest->count(testName))
        ETH_ERROR_MESSAGE("Test filler read redundant expect section: " + testName +
                          " (" + _tr.transaction()->dataLabel() + ")");

    verifyFilledTest(m_test.unitTestVerifyBC(), m_aBlockchainTest, _network);
    (*m_filledBlockchainTest).atKeyPointer(testName) = m_aBlockchainTest;
    m_session.test_rewindToBlock(0);
}

spDataObject StateTestChainRunner::getFilledTest() const
{
    return m_filledBlockchainTest;
}


void StateTestChainRunner::comparePostState(StateTestFillerExpectSection const& _expect, FORK const& _network)
{
    auto mexpect = correctMiningReward(_expect, _network);
    try
    {
        spState postState = getRemoteState(m_session);
        compareStates(mexpect, postState);
        (*m_aBlockchainTest).atKeyPointer("postState") = postState->asDataObject();
        (*m_aBlockchainTest).removeKey("postStateHash");
    }
    catch (StateTooBig const&)
    {
        compareStates(mexpect, m_session);
    }
}

}
