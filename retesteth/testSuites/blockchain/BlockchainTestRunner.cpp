#include "BlockchainTestRunner.h"
#include "Common.h"
#include <retesteth/EthChecks.h>
#include <retesteth/ExitHandler.h>
#include <retesteth/Options.h>
#include <retesteth/TestOutputHelper.h>
#include <retesteth/testStructures/PrepareChainParams.h>
#include <retesteth/testSuites/Common.h>
#include <retesteth/testSuites/blockchain/fillers/TestBlockchainManager.h>

using namespace std;
using namespace test;
using namespace test::debug;
using namespace test::session;

BlockchainTestRunner::BlockchainTestRunner(BlockchainTestInFilled const& _test, TestSuite::TestSuiteOptions const& _opt)
  : m_test(_test), m_opt(_opt), m_session(RPCSession::instance(TestOutputHelper::getThreadID()))
{
    ETH_DC_MESSAGE(DC::TESTLOG, "Running " + _test.testName());
}

bool BlockchainTestRunner::validateFork(string const& _testName, FORK const& _net)
{
    ClientConfig const& cfg = Options::get().getDynamicOptions().getCurrentConfig();
    bool fillerSkipFork = cfg.checkForkSkipOnFiller(_net);
    if (!cfg.validateForkAllowed(_net, false) || fillerSkipFork)
    {
        if (fillerSkipFork)
            ETH_WARNING("Skipping unsupported fork: " + _net.asString() + " in " + _testName);
        return false;
    }
    return true;
}

bool BlockchainTestRunner::abortBlock() const
{
    auto const& opt = Options::get();
    return (opt.blockLimit != 0 && m_blockNumber + 1 >= opt.blockLimit);
}

void BlockchainTestRunner::setChainParams()
{
    TestOutputHelper::get().setCurrentTestName(m_test.testName());
    TestOutputHelper::get().setUnitTestExceptions(m_test.unitTestExceptions());
    TestOutputHelper::get().setCurrentTestInfo(TestInfo(m_test.network().asString(), 0));
    m_session.test_setChainParams(prepareChainParams(m_test.network(), m_test.sealEngine(), m_test.Pre(), m_test.Env()));
}

void BlockchainTestRunner::incrementBlockAndSetTestInfo()
{
    m_blockNumber++;
    TestOutputHelper::get().setCurrentTestInfo(TestInfo(m_test.network().asString(), m_blockNumber));
}

void BlockchainTestRunner::validateTransactionSequence(BlockchainTestBlock const& _tblock)
{
    ETH_DC_MESSAGEC(DC::LOWLOG, "PERFORM TRANSACTION SEQUENCE VALIDATION...", LogColor::YELLOW);

    bool atLeastOnceSequence = false;
    typedef std::tuple<spTransaction, string> SpTrException;
    std::map<FH32, SpTrException> expectedExceptions;
    if (_tblock.transactionSequence().size())
    {
        if (_tblock.header()->number() > m_session.eth_blockNumber())
        {
            atLeastOnceSequence = true;
            m_session.test_modifyTimestamp(_tblock.header()->timestamp());
        }
        else
            ETH_WARNING(
                "Skipping invalid transaction exception check in reorg block! (calling rewind not good when running the test, "
                "use filltests instead)");
    }
    for (auto const& el : _tblock.transactionSequence())
    {
        spTransaction const& tr = std::get<0>(el);
        string const& sException = std::get<1>(el);
        modifyTransactionChainIDByNetwork(tr, m_test.network());
        m_session.eth_sendRawTransaction(tr->getRawBytes(), tr->getSecret());
        SpTrException info = {tr, sException};
        expectedExceptions.emplace(tr->hash(), info);
    }
    if (atLeastOnceSequence)
    {
        VALUE const& origNumber = m_session.eth_blockNumber();
        MineBlocksResult const miningRes = m_session.test_mineBlocks(1);
        for (auto const& el : expectedExceptions)
        {
            SpTrException const& res = el.second;
            spTransaction resTransaction = std::get<0>(res);
            string const& testException = std::get<1>(res);
            compareTransactionException(resTransaction, miningRes, testException);
        }
        m_session.test_rewindToBlock(origNumber);
    }
}

teststruct::FH32 BlockchainTestRunner::mineBlock(BYTES const& _rlp)
{
    return m_session.test_importRawBlock(_rlp);
}

teststruct::EthGetBlockBy BlockchainTestRunner::requestBlock(teststruct::FH32 const& _hash)
{
    return m_session.eth_getBlockByHash(_hash, Request::FULLOBJECTS);
}

teststruct::EthGetBlockBy BlockchainTestRunner::requestBlock(teststruct::VALUE const& _number)
{
    return m_session.eth_getBlockByNumber(_number, Request::LESSOBJECTS);
}

bool BlockchainTestRunner::checkLastRPCBlockException(BlockchainTestBlock const& _tblock)
{
    if (!m_session.getLastRPCError().empty())
    {
        // and options does NOT allow invalid blocks or this block was expected to be valid
        // BCValidBlocks does not allow invalid blocks!
        if (!m_opt.allowInvalidBlocks || !_tblock.expectedInvalid())
            ETH_ERROR_MESSAGE(
                "Importing raw RLP block, block was expected to be valid! (if it was intended, check that it is not in Valid "
                "blocks test suite) " +
                m_session.getLastRPCError().message());

        if (Options::getDynamicOptions().getCurrentConfig().cfgFile().socketType() == ClientConfgSocketType::TransitionTool)
        {
            string const& sBlockException = _tblock.getExpectException();
            if (!sBlockException.empty())
                blockchainfiller::TestBlockchain::checkBlockException(m_session, sBlockException);
        }
        return true;
    }
    else
        ETH_ERROR_REQUIRE_MESSAGE(!_tblock.expectedInvalid(),
            "Running blockchain test: Block #" + to_string(m_blockNumber) + " expected to be invalid!");
    return false;
}

void BlockchainTestRunner::performOptionCommands(BlockchainTestBlock const& _tblock, EthGetBlockBy const& _latestBlock)
{
    size_t txIndex = 0;
    for (auto const& tr : _tblock.transactions())
    {
        TxContext const ctx(m_session, m_test.testName(), tr, _latestBlock.header(), m_test.network(), m_blockNumber, txIndex);
        performVMTrace(ctx);
        txIndex++;
    }
    TxContext const ctx(
        m_session, m_test.testName(), spTransaction(0), _latestBlock.header(), m_test.network(), m_blockNumber, 0);
    performPostStateBlockOnly(ctx);
    this->performStatediffBlockOnly(m_blockNumber);
    this->performStatediffBlockOnly(m_blockNumber);
}

void BlockchainTestRunner::validateBlockHeader(BlockchainTestBlock const& _tblock, EthGetBlockBy const& _latestBlock)
{
    spDataObject remoteHeader = _latestBlock.header()->asDataObject();
    spDataObject testHeader = _tblock.header()->asDataObject();
    bool condition = remoteHeader->asJson(0, false) == testHeader->asJson(0, false);
    /*if (_opt.isLegacyTests)
        {
            inTestHeader = bdata.atKey("blockHeader");  // copy!!!
            if (latestBlock.getBlockHeader().atKey("nonce").asString() == "0x0000000000000000")
            {
                // because on NoProof mixHash is returned as 0x00 by other client, even if block
                // imported with mixHash/nonce ??? who returned 0x00 as mixHash??? wtf
                inTestHeader["mixHash"] =
                    "0x0000000000000000000000000000000000000000000000000000000000000000";
                inTestHeader["nonce"] = "0x0000000000000000";
            }
            inTestHeader["hash"] = latestBlock.getBlockHeader().atKey("hash");
            condition = latestBlock.getBlockHeader() == inTestHeader;
        }*/

    // string const jsonHeader = inTestHeader.type() == DataType::Null ?
    //                              bdata.atKey("blockHeader").asJson() :
    //                              "(adjusted)" + inTestHeader.asJson();
    string message;
    if (!condition)
    {
        string errField;
        message = "Client return HEADER vs Test HEADER: \n";
        message += compareBlockHeaders(remoteHeader.getCContent(), testHeader.getCContent(), errField);
    }
    ETH_ERROR_REQUIRE_MESSAGE(
        condition, "Client report different blockheader after importing the rlp than expected by test! \n" + message);
}

void BlockchainTestRunner::validateUncles(BlockchainTestBlock const& _tblock, EthGetBlockBy const& _latestBlock)
{
    // Verify uncles to one described in the fields
    size_t ind = 0;
    string message = "Client return UNCLES: " + to_string(_latestBlock.uncles().size()) + " vs " +
                     "Test UNCLES: " + to_string(_tblock.uncles().size());
    ETH_ERROR_REQUIRE_MESSAGE(_latestBlock.uncles().size() == _tblock.uncles().size(),
        "Client report different uncle count after importing the rlp than expected by test! \n" + message);
    for (spBlockHeader const& tuncle : _tblock.uncles())
    {
        FH32 clientUncleHash = _latestBlock.uncles().at(ind++);  // EthGetBlockBy return only hashes
        if (clientUncleHash != tuncle->hash())
            ETH_ERROR_MESSAGE("Remote client returned block with unclehash that is not expected by test! " + message);
    }
}

void BlockchainTestRunner::validateTransactions(BlockchainTestBlock const& _tblock, EthGetBlockBy const& _latestBlock)
{
    // Check Transaction count
    string message = "Client return TRANSACTIONS: " + to_string(_latestBlock.transactions().size()) + " vs " +
                     "Test TRANSACTIONS: " + to_string(_tblock.transactions().size());
    ETH_ERROR_REQUIRE_MESSAGE(_latestBlock.transactions().size() == _tblock.transactions().size(),
        "Client report different transaction count after importing the rlp than expected by test! \n" + message);

    // Verify transactions to one described in the fields
    size_t ind = 0;
    for (spTransaction const& tr : _tblock.transactions())
    {
        if (ExitHandler::receivedExitSignal())
            return;
        EthGetBlockByTransaction const& clientTr = _latestBlock.transactions().at(ind++);
        ETH_ERROR_REQUIRE_MESSAGE(clientTr.blockHash() == _tblock.header()->hash(),
            "Error checking remote transaction, remote tr `blockHash` is different to one described in test block! "
            "(" +
                clientTr.blockHash().asString() + " != " + _tblock.header()->hash().asString() + ")");

        ETH_ERROR_REQUIRE_MESSAGE(clientTr.blockNumber() == _tblock.header()->number(),
            "Error checking remote transaction, remote tr `blockNumber` is different to one described in test block! "
            "(" +
                clientTr.blockNumber().asDecString() + " != " + _tblock.header()->number().asDecString() + ")");

        BYTES const testTr = tr->getRawBytes();
        BYTES const remoteTr = clientTr.transaction()->getRawBytes();

        ETH_ERROR_REQUIRE_MESSAGE(remoteTr == testTr, "Error checking remote transaction, remote tr `" + remoteTr.asString() +
                                                          "` is different to test tr `" + testTr.asString() + "`)");
    }
}

void BlockchainTestRunner::checkPostState(EthGetBlockBy const& _latestBlock)
{
    performPostState(m_session, m_test.testName(), m_test.network().asString(), _latestBlock.header()->stateRoot());
    performFinalStateDiff();

    if (m_test.isFullState())
        compareStates(m_test.Post(), m_session);
    else
        ETH_ERROR_REQUIRE_MESSAGE(m_test.PostHash() == _latestBlock.header()->stateRoot(),
            "postStateHash mismatch! remote: " + _latestBlock.header()->stateRoot().asString() + " != test " =
                m_test.PostHash().asString());

    if (m_test.lastBlockHash() != _latestBlock.header()->hash())
        ETH_ERROR_MESSAGE("lastblockhash does not match! remote: '" + _latestBlock.header()->hash().asString() + "', test: '" +
                          m_test.lastBlockHash().asString() + "'");
}

void BlockchainTestRunner::checkGenesis()
{
    bool skipGenesisCheck = false;
    if (m_opt.isLegacyTests)
    {
        // Skip old generated tests (Legacy) genesis check on aleth
        // because mixHash and nonce return is different to geth
        if (Options::getDynamicOptions().getCurrentConfig().cfgFile().path().parent_path().filename() == "aleth")
            skipGenesisCheck = true;

        if (m_test.genesisRLP().asString() == "0x00")
            skipGenesisCheck = true;
    }

    if (!skipGenesisCheck)
    {
        EthGetBlockBy genesis(m_session.eth_getBlockByNumber(0, Request::LESSOBJECTS));
        if (m_test.genesisRLP() != genesis.getRLPHeaderTransactions())
            ETH_ERROR_MESSAGE("genesisRLP in test != genesisRLP on remote client! (" + m_test.genesisRLP().asString() +
                              "' != '" + genesis.getRLPHeaderTransactions().asString() + "'");
    }
}

void BlockchainTestRunner::performStatediffBlockOnly(size_t _blockNumber)
{
    auto const& statediff = Options::get().statediff;
    if (statediff.initialized() && statediff.isBlockSelected && m_stateDiffStateB.isEmpty())
    {
        m_triedStateDiff = true;
        if (statediff.firstBlock == _blockNumber && m_stateDiffStateA.isEmpty())
            m_stateDiffStateA = getRemoteState(m_session);
        else if (statediff.seconBlock == _blockNumber && m_stateDiffStateB.isEmpty() && !m_stateDiffStateA.isEmpty())
        {
            m_stateDiffStateB = getRemoteState(m_session);
            auto const diff = test::stateDiff(m_stateDiffStateA, m_stateDiffStateB)->asJson();
            ETH_DC_MESSAGE(DC::STATE,
                "\nRunning BC test State Diff:" + TestOutputHelper::get().testInfo().errorDebug() + cDefault + " \n" + diff);
        }
    }
}

void BlockchainTestRunner::performFinalStateDiff()
{
    auto const& statediff = Options::get().statediff;
    if (statediff.initialized() && !statediff.isBlockSelected)
    {
        auto const diff = test::stateDiff(m_test.Pre(), getRemoteState(m_session))->asJson();
        ETH_DC_MESSAGE(DC::STATE,
            "\nRunning BC test State Diff:" + TestOutputHelper::get().testInfo().errorDebug() + cDefault + " \n" + diff);
    }
}

BlockchainTestRunner::~BlockchainTestRunner()
{
    if (m_triedStateDiff)
        showWarningIfStatediffNotFound(m_stateDiffStateA, m_stateDiffStateB);
}
