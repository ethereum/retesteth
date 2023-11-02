#include "TestBlockchain.h"
#include <retesteth/Options.h>
#include <retesteth/helpers/TestHelper.h>
#include <retesteth/testStructures/PrepareChainParams.h>
#include <retesteth/testSuites/Common.h>
#include "../Common.h"

using namespace std;
using namespace test::debug;
using namespace test::session;

namespace test::blockchainfiller
{
TestBlockchain::TestBlockchain(BlockchainTestFillerEnv const& _testEnv, State const& _genesisState, SealEngine _engine,
    FORK const& _network, string const& _chainName, RegenerateGenesis _regenerateGenesis)
  : m_session(RPCSession::instance(TestOutputHelper::getThreadID())),
    m_network(_network),
    m_sealEngine(_engine),
    m_testEnv(_testEnv),
    m_genesisState(_genesisState),
    m_chainName(_chainName)
{
    if (_regenerateGenesis == RegenerateGenesis::TRUE)
        resetChainParams();

    EthGetBlockBy latestBlock(m_session.eth_getBlockByNumber(0, Request::LESSOBJECTS));
    TestBlock genesisBlock(latestBlock.getRLPHeaderTransactions(), "genesis", m_network, 0);
    genesisBlock.registerTestHeader(latestBlock.header());
    genesisBlock.setNextBlockForked(mineNextBlockAndRevert());
    m_blocks.emplace_back(genesisBlock);
}

bool clientSupportWithdrawalsRPC()
{
    return Options::getCurrentConfig().cfgFile().socketType() == ClientConfgSocketType::TransitionTool;
}

void TestBlockchain::_mineBlock_importWithdrawals(BlockchainTestFillerBlock const& _blockInTest)
{
    if (clientSupportWithdrawalsRPC() && _blockInTest.getExpectException(m_network).empty())
    {
        if (_blockInTest.withdrawals().size() > 0)
        {
            for (auto const& wt : _blockInTest.withdrawals())
            {
                auto const str = dev::toHexPrefixed(wt.withdrawal()->asRLPStream().out());
                BYTES rlp(str);
                m_session.test_registerWithdrawal(rlp);
            }
        }
    }
}

GCP_SPointer<EthGetBlockBy> TestBlockchain::mineBlock(
    BlockchainTestFillerBlock const& _blockInTest, vectorOfSchemeBlock const& _preparedUncleBlocks, BYTES& _rawRLP, bool _isTest)
{
    ETH_DC_MESSAGEC(DC::RPC, "MINE BLOCK: " + m_sDebugString, LogColor::YELLOW);

    _mineBlock_importWithdrawals(_blockInTest);
    MineBlocksResult const miningRes = m_session.test_mineBlocks(1);
    VALUE latestBlockNumber(m_session.eth_blockNumber());


    spFH32 minedBlockHash;
    if (_blockInTest.hasBlockHeaderOverwrite(m_network)
        || _blockInTest.uncles().size() > 0
        || !clientSupportWithdrawalsRPC()
        || miningRes.rejectedTxCount() > 0)
    {
        // Need to overwrite the blockheader of a mined block with either blockHeader or uncles
        // Then import it again and see what client says, because mining with uncles not supported
        // And because blockchain test filler can override blockheader for testing purposes
        ETH_DC_MESSAGE(DC::TESTLOG, "Postmine blockheader: " + m_sDebugString);
        FH32 const hash = postmineBlockHeader(_blockInTest, latestBlockNumber, _preparedUncleBlocks, _rawRLP);
        minedBlockHash = spFH32(new FH32(hash.asString()));
    }

    // Expected exception for this block
    string const& sBlockException = _blockInTest.getExpectException(m_network);
    checkBlockException(m_session, sBlockException);  // Check any impoprt failure exceptions here

    GCP_SPointer<EthGetBlockBy> remoteBlock;
    if (!minedBlockHash.isEmpty())
    {
        // If after modifications block import succeed
        if (minedBlockHash.getCContent() != FH32::zero())
            remoteBlock = m_session.eth_getBlockByHash(minedBlockHash.getContent(), Request::FULLOBJECTS);
        else
            return remoteBlock;  // after modifications block import failed. but that was expected.
    }
    else
    {
        // There was no block modifications, it is just a regular mining
        remoteBlock = m_session.eth_getBlockByNumber(latestBlockNumber, Request::FULLOBJECTS);
        _rawRLP = remoteBlock->getRLPHeaderTransactions();
    }

    if (_isTest)
        return remoteBlock;

    // check that transactions are good
    for (auto const& trInTest : _blockInTest.transactions())
    {
        auto const& container = remoteBlock->transactions();
        auto result = std::find_if(container.begin(), container.end(),
            [&trInTest](EthGetBlockByTransaction const& el) { return el.hash() == trInTest.tr().hash(); });

        string const& exception = trInTest.getExpectException(m_network);
        if (result == container.end())
        {
            string const& reason = miningRes.getTrException(trInTest.tr().hash());
            if (exception.empty())
                ETH_WARNING(
                    "TestBlockchain::mineBlock transaction has unexpectedly failed to be mined (see logs --verbosity 6): \n" +
                    trInTest.tr().asDataObject()->asJson() + "\nReason: `" + reason + "`");
            else
            {
               string const& expectedReason = Options::getCurrentConfig().translateException(exception);
               if (reason.find(expectedReason) == string::npos || expectedReason.empty())
               {
                   ETH_WARNING(trInTest.tr().asDataObject()->asJson());
                   ETH_ERROR_MESSAGE(string("Transaction rejecetd but due to a different reason: \n") +
                      "Expected reason: `" + expectedReason + "` (" + exception + ")\n" +
                      "Client reason: `" + reason
                     );
               }
            }
        }
        else
        {
            if (!exception.empty())
                ETH_WARNING("TestBlockchain::mineBlock transaction expected to failed with `"+ exception +"` but mined good: \n" +
                            trInTest.tr().asDataObject()->asJson());
        }
    }

    auto remoteTr = remoteBlock->transactions().size();
    auto testblTr = _blockInTest.transactions().size();
    auto expectFails = _blockInTest.invalidTransactionCount(m_network);
    if (remoteTr != testblTr - expectFails)
        ETH_ERROR_MESSAGE("BlockchainTest transaction execution failed! (remote " + fto_string(remoteTr) + " != test " +
                          fto_string(testblTr) + ", allowedToFail = " + fto_string(expectFails) + " )");

    return remoteBlock;
}

// Ask remote client to generate a blockheader that will later used for uncles
spBlockHeader TestBlockchain::mineNextBlockAndRevert()
{
    ETH_DC_MESSAGEC(DC::RPC, "Mine uncle block (next block) and revert: " + m_sDebugString, LogColor::YELLOW);
    {
        VALUE latestBlockNumber(m_session.eth_blockNumber());
        EthGetBlockBy const latestBlock(m_session.eth_getBlockByNumber(latestBlockNumber, Request::LESSOBJECTS));
        m_session.test_modifyTimestamp(latestBlock.header()->timestamp() + 1000);
    }

    m_session.test_mineBlocks(1);
    VALUE latestBlockNumber(m_session.eth_blockNumber());
    EthGetBlockBy const nextBlock(m_session.eth_getBlockByNumber(latestBlockNumber, Request::LESSOBJECTS));
    m_session.test_rewindToBlock(nextBlock.header()->number() - 1);  // rewind to the previous block

    // assign a random coinbase for an uncle block to avoid UncleIsAncestor exception
    // otherwise this uncle would be similar to a block mined
    spDataObject head = nextBlock.header()->asDataObject();
    (*head)["coinbase"] = "0xb94f5374fce5ed0000000097c15331677e6ebf0b";  // FH20::random().asString();
    return readBlockHeader(head);
}

string TestBlockchain::prepareDebugInfoString(string const& _newBlockChainName)
{
    size_t newBlockNumber = m_blocks.size();
    TestInfo errorInfo(m_network.asString(), newBlockNumber, _newBlockChainName);
    TestOutputHelper::get().setCurrentTestInfo(errorInfo);

    string sBlockNumber;
    if (test::debug::Debug::get().flag(DC::TESTLOG))
        sBlockNumber = fto_string(newBlockNumber);  // very heavy
    m_sDebugString = "(bl: " + sBlockNumber + ", ch: " + _newBlockChainName + ", net: " + m_network.asString() + ")";
    ETH_DC_MESSAGEC(DC::RPC, "Generating a test block: " + m_sDebugString, LogColor::YELLOW);
    return m_sDebugString;
}


// Restore this chain on remote client up to < _number block
// Restore chain up to _number of blocks. if _number is 0 restore the whole chain
void TestBlockchain::restoreUpToNumber(SessionInterface& _session, VALUE const& _number, bool _samechain)
{
    size_t firstBlock;
    if (_samechain)
    {
        if (_number == 0)
            return;
        firstBlock = (size_t)_number.asBigInt();
        assert(firstBlock > 0);
        _session.test_rewindToBlock(firstBlock - 1);
    }
    else
    {
        firstBlock = 0;
        _session.test_rewindToBlock(0);  // Rewind to the begining
    }

    if (_number == 0)
    {
        // We are NOT on the same chain. Restore the whole history
        size_t actNumber = 0;
        for (auto const& block : m_blocks)
        {
            if (actNumber++ == 0)  // Skip Genesis
                continue;
            _session.test_importRawBlock(block.getRawRLP());
        }
        return;
    }

    size_t actNumber = 0;
    size_t popUpCount = 0;
    for (auto const& block : m_blocks)
    {
        if (actNumber == 0)  // Skip Genesis
        {
            actNumber++;
            continue;
        }

        if (actNumber < firstBlock)  // we are on the same chain. lets move to the rewind point
        {
            actNumber++;
            continue;
        }

        if (actNumber < _number.asBigInt())
            _session.test_importRawBlock(block.getRawRLP());
        else
        {
            popUpCount++;
            // std::cerr << "popup " << std::endl;
        }
        actNumber++;
    }

    // Restore blocks up to `number` forgetting the rest of history
    for (size_t i = 0; i < popUpCount; i++)
        m_blocks.pop_back();  // blocks are now at m_knownBlocks
}


// Because remote client does not work with uncles, manipulate the response record with uncle information
// Manually here imitating a block with retesteth manipulations
FH32 TestBlockchain::postmineBlockHeader(BlockchainTestFillerBlock const& _blockInTest, VALUE const& _latestBlockNumber,
    std::vector<spBlockHeader> const& _uncles, BYTES& _rawRLP)
{
    // if blockHeader is defined in test Filler, rewrite the last block header fields with info from
    // test and reimport it to the client in order to trigger an exception in the client
    EthGetBlockBy remoteBlock(m_session.eth_getBlockByNumber(_latestBlockNumber, Request::FULLOBJECTS));
    EthereumBlock managedBlock(remoteBlock.header());
    for (auto const& tr : remoteBlock.transactions())
        managedBlock.addTransaction(tr->transaction());

    // Register invalid transactions in postmine
    for (auto const& tr : _blockInTest.transactions())
    {
        if (!managedBlock.hasTransaction(tr.tr().hash()))
            managedBlock.addTransaction(tr.trPointer());
    }

    // Attach test-generated uncle to a block and then reimport it again
    for (auto const& un : _uncles)
        managedBlock.addUncle(un);

    if (_blockInTest.withdrawals().size() > 0)
        managedBlock.forceWithdrawalsRLP();
    for (auto const& wt : _blockInTest.withdrawals())
        managedBlock.addWithdrawal(wt.withdrawal());

    bool weOverwriteHashFields = false;
    if (_blockInTest.hasBlockHeaderOverwrite(m_network))
    {
        BlockHeaderOverwrite const& headerOverwrite = _blockInTest.getHeaderOverwrite(m_network);

        if (headerOverwrite.forceNoWithdrawalsRLP())
            managedBlock.forceNoWithdrawalsRLP();

        if (headerOverwrite.hasBlockHeader())
            managedBlock.replaceHeader(headerOverwrite.header().overwriteBlockHeader(managedBlock.header()));

        // Overwrite blockheader if defined in the test filler
        if (headerOverwrite.hasRelTimeStamp())
        {
            EthGetBlockBy previousBlock(m_session.eth_getBlockByNumber(_latestBlockNumber - 1, Request::LESSOBJECTS));
            managedBlock.headerUnsafe().getContent().setTimestamp(
                previousBlock.header()->timestamp() + headerOverwrite.relTimeStamp());
        }

        // replace block with overwritten header
        if (headerOverwrite.hasBlockHeader())
        {
            if (headerOverwrite.header().hasUncleHash() || headerOverwrite.header().hasTransactionHash())
                weOverwriteHashFields = true;
        }
    }

    if (!weOverwriteHashFields)
        managedBlock.recalculateUncleHash();
    m_session.test_rewindToBlock(_latestBlockNumber - 1);
    _rawRLP = BYTES(managedBlock.getRLP().asString());
    return FH32(m_session.test_importRawBlock(_rawRLP));
}

// Returns true if the block is valid
bool TestBlockchain::checkBlockException(SessionInterface const& _session, string const& _sBlockException)
{
    // Check malicious block import exception
    // Relies on that previous block import was exactly this block !!!
    if (_sBlockException.empty())
    {
        ETH_ERROR_REQUIRE_MESSAGE(_session.getLastRPCError().empty(),
            "Postmine block tweak expected no exception! Client errors with: '" + _session.getLastRPCError().message() + "'");
    }
    else
    {
        auto const& cfg = Options::get().getCurrentConfig();
        string const& clientExceptionString = cfg.translateException(_sBlockException);
        size_t pos = _session.getLastRPCError().message().find(clientExceptionString);
        if (clientExceptionString.empty())
            pos = string::npos;
        ETH_ERROR_REQUIRE_MESSAGE(pos != string::npos,
            cYellow + _sBlockException + cRed + " Not found in client response to postmine block tweak!" +
                "\nImport result of postmine block: \n'" + cYellow + _session.getLastRPCError().message() + cRed +
                "',\n Test Expected: \n'" + cYellow + clientExceptionString + cRed + "'\n");
        return false;  // block is not valid
    }
    return true;  // block is valid
}

// Need to call resetChainParams because TestBLockchainManager could have chains with different networks
void TestBlockchain::resetChainParams() const
{
    m_session.test_setChainParams(prepareChainParams(m_network, m_sealEngine, m_genesisState, m_testEnv));
}

void TestBlockchain::performOptionCommandsOnGenesis()
{
    auto const& poststate = Options::get().poststate;
    if (poststate.initialized() && poststate.isBlockSelected && poststate.blockNumber == 0)
    {
        auto const genesis = m_session.eth_getBlockByNumber(0, Request::LESSOBJECTS);
        auto const& testname = TestOutputHelper::get().testName();
        TxContext const ctx(m_session, testname, spTransaction(0), genesis->header(), m_network, 0, 0);
        performPostState(ctx);
    }

    auto const& statediff = Options::get().statediff;
    if (statediff.initialized() && statediff.isBlockSelected && statediff.firstBlock == 0)
    {
        m_triedStateDiff = true;
        m_stateDiffStateA = getRemoteState(m_session);
    }
}

TestBlockchain::~TestBlockchain()
{
    if (m_triedStateDiff)
        showWarningIfStatediffNotFound(m_stateDiffStateA, m_stateDiffStateB);
}


}  // namespace blockchainfiller
