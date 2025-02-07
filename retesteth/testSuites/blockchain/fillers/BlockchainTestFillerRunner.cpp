#include "TestBlockchainManager.h"
#include "BlockchainTestFillerRunner.h"
#include <retesteth/helpers/TestOutputHelper.h>
#include <retesteth/EthChecks.h>
#include <retesteth/session/Session.h>
#include <retesteth/Options.h>
#include <retesteth/testSuites/Common.h>
#include <retesteth/testStructures/Common.h>
#include <retesteth/Constants.h>
using namespace test;
using namespace test::session;
using namespace test::debug;
using namespace teststruct;
using namespace std;
using namespace test::blockchainfiller;

BlockchainTestFillerRunner::BlockchainTestFillerRunner(BlockchainTestInFiller const& _test)
  : m_test(_test), m_session(RPCSession::instance(TestOutputHelper::getThreadID()))
{
    ETH_DC_MESSAGE(DC::TESTLOG, "Filling " + _test.testName());
}

spDataObject BlockchainTestFillerRunner::makeNewBCTestForNet(FORK const& _net)
{
    // Construct filled blockchain test fields

    spDataObject _filledTest;
    DataObject& filledTest = _filledTest.getContent();

    string const newtestname = m_test.testName() + "_" + _net.asString();
    TestOutputHelper::get().setCurrentTestName(newtestname);

    filledTest.setKey(newtestname);
    if (m_test.hasInfo())
        (*_filledTest).atKeyPointer("_info") = m_test.Info().rawData();
    filledTest["sealEngine"] = sealEngineToStr(m_test.sealEngine());
    filledTest["network"] = _net.asString();
    if (m_test.hasConfig())
    {
        filledTest.atKeyPointer("config") = m_test.Config().asDataObject(_net);
        filledTest["config"]["network"] = _net.asString();
    }
    return _filledTest;
}

const int HISTORY_BUFFER_LENGTH = 8191;
TestBlockchainManager BlockchainTestFillerRunner::makeTestChainManager(teststruct::FORK const& _net)
{
    ETH_DC_MESSAGEC(DC::RPC, "FILL GENESIS INFO: ", LogColor::LIME);
    std::vector<spAccountBase> additionalAccounts;

    if (test::compareFork(_net, test::CMP::ge, FORK("Cancun"))
        && !m_test.Pre().hasAccount(teststruct::C_FH20_BEACON))
    {
        ETH_DC_MESSAGE(DC::RPC, "Retesteth inserts beacon root account into the pre state!");
        auto beaconAcc = makeBeaconAccount();
        Storage& str = const_cast<Storage&>(beaconAcc->storage());

        // header.timestamp % HISTORY_BUFFER_LENGTH to be header.timestamp
        spVALUE key = sVALUE(m_test.Env().currentTimestamp().asBigInt() % HISTORY_BUFFER_LENGTH);
        spVALUE val = sVALUE(m_test.Env().currentTimestamp().asBigInt());
        str.addRecord({key, val});

        additionalAccounts.emplace_back(beaconAcc);
    }

    auto blockchains = TestBlockchainManager(m_test.Env(), m_test.Pre(), m_test.sealEngine(), _net, additionalAccounts);
    return blockchains;
}

void BlockchainTestFillerRunner::makeGenesis(spDataObject& _filledTest, TestBlockchainManager& _testchain) const
{
    TestBlock const& genesis = _testchain.getLastBlock();
    (*_filledTest).atKeyPointer("genesisBlockHeader") = genesis.getTestHeader()->asDataObject();
    (*_filledTest)["genesisRLP"] = genesis.getRawRLP().asString();
}

void BlockchainTestFillerRunner::setTestInfoAndExpectExceptions(FORK const& _net) const
{
    auto& helper = TestOutputHelper::get();
    helper.setUnitTestExceptions(m_test.unitTestExceptions());
    helper.setCurrentTestInfo(TestInfo(_net.asString(), 0));
}

bool BlockchainTestFillerRunner::optionsLimitBlock(size_t _block) const
{
    auto const& opt = Options::get();
    return (opt.blockLimit != 0 && _block >= opt.blockLimit);
}

void BlockchainTestFillerRunner::performOptionsOnFinalState(EthGetBlockBy const& _finalBlock)
{
    auto const& opt = Options::get();

    // Perform --statediff without selector
    if (opt.statediff.initialized() && !opt.statediff.isBlockSelected)
    {
        auto const diff = test::stateDiff(m_test.Pre(), getRemoteState(m_session))->asJson();
        ETH_DC_MESSAGE(DC::STATE,
            "\nFilling BC test State Diff:" + TestOutputHelper::get().testInfo().errorDebug() + cDefault + " \n" + diff);
    }

    // Perform --poststate without selector
    if (opt.poststate && !opt.poststate.isBlockSelected)
        ETH_DC_MESSAGE(DC::STATE, "PostState " + TestOutputHelper::get().testInfo().errorDebug() + " : \n" +
                                   cDefault + "Hash: " + _finalBlock.header()->stateRoot().asString());
}

void BlockchainTestFillerRunner::compareFinalState(spDataObject& _filledTest, StateIncomplete const& _expectState, EthGetBlockBy const& _finalBlock)
{
    try
    {
        spState remoteState = getRemoteState(m_session);
        compareStates(_expectState, remoteState);
        (*_filledTest).atKeyPointer("postState") = remoteState->asDataObject();
    }
    catch (StateTooBig const&)
    {
        compareStates(_expectState, m_session);
        (*_filledTest)["postStateHash"] = _finalBlock.header()->stateRoot().asString();
    }

    (*_filledTest)["lastblockhash"] = _finalBlock.header()->hash().asString();
}

EthGetBlockBy BlockchainTestFillerRunner::getLastBlock()
{
    return m_session.eth_getBlockByNumber(m_session.eth_blockNumber(), Request::LESSOBJECTS);
}
