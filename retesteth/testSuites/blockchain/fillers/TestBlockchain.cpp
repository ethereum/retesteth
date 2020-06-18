#include "TestBlockchain.h"
#include <retesteth/Options.h>
#include <retesteth/testStructures/PrepareChainParams.h>

namespace test
{
namespace blockchainfiller
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
    m_blocks.push_back(genesisBlock);
}

void TestBlockchain::generateBlock(
    BlockchainTestFillerBlock const& _block, vectorOfSchemeBlock const& _uncles, bool _generateUncles)
{
    // If block is raw RLP block
    if (_block.isRawRLP())
    {
        FH32 const blHash(m_session.test_importRawBlock(_block.rawRLP()));
        checkBlockException(_block.getExpectException(m_network));
        if (!blHash.isZero())
            ETH_ERROR_MESSAGE("rawBlock rlp appered to be valid. Unable to contruct objects from RLP!");
        else
        {
            TestBlock newBlock(_block.rawRLP(), m_chainName, m_network, m_blocks.size());
            newBlock.registerTestExceptios(_block.getExpectException(m_network));
            m_blocks.push_back(newBlock);
        }
        return;
    }

    // Import known transactions to remote client
    ETH_LOGC("Import transactions: " + m_sDebugString, 6, LogColor::YELLOW);
    for (auto const& tr : _block.transactions())
        m_session.eth_sendRawTransaction(tr.tr().getSignedRLP());

    // Remote client generate block with transactions
    // And if it has uncles or blockheader overwrite we perform manual overwrite and reimport block again
    // Then construct this RLP of a modifed block (with new header and uncles)
    // if block mining failed this will be the rawRLP of import
    BYTES rawRLP(DataObject("0x"));
    GCP_SPointer<EthGetBlockBy> minedBlock = mineBlock(_block, _uncles, rawRLP);

    if (minedBlock.isEmpty())
    {
        // if block mining failed, the block is invalid
        FORK const& newBlockNet = _block.hasChainNet() ? _block.chainNet() : m_network;
        TestBlock newBlock(rawRLP, _block.chainName(), newBlockNet, m_blocks.size());
        newBlock.registerTestExceptios(_block.getExpectException(m_network));
        newBlock.setDoNotExport(_block.isDoNotImportOnClient());
        m_blocks.push_back(newBlock);
    }
    else
    {
        // if block mining succeed. the block is valid.
        FORK const& newBlockNet = _block.hasChainNet() ? _block.chainNet() : m_network;
        TestBlock newBlock(rawRLP, _block.chainName(), newBlockNet, m_blocks.size());
        newBlock.registerTestExceptios(_block.getExpectException(m_network));
        newBlock.registerTestHeader(minedBlock.getCContent().header());
        newBlock.setDoNotExport(_block.isDoNotImportOnClient());

        // -------
        // Register all test transactions (the one described in test)
        // In the same order as on remote block has returned after mining
        typedef std::tuple<spTransaction, bool> testTrInfo;
        std::map<FH32, testTrInfo> testTransactionMap;
        for (auto const& tr : _block.transactions())
            testTransactionMap[tr.tr().hash()] = {spTransaction(new Transaction(tr.tr())), tr.isMarkedInvalid()};

        for (auto const& remoteTr : minedBlock.getCContent().transactions())
        {
            if (testTransactionMap.count(remoteTr.hash()))
            {
                bool isMarkedInvalid = std::get<1>(testTransactionMap.at(remoteTr.hash()));
                if (!isMarkedInvalid)
                {
                    spTransaction const& tr = std::get<0>(testTransactionMap.at(remoteTr.hash()));
                    newBlock.registerTestTransaction(tr.getCContent());
                }
                testTransactionMap.erase(remoteTr.hash());
            }
            else
                ETH_ERROR_MESSAGE("Remote client returned block with transaction that is not registered in test!");
        }

        // Register all the transactions that have not been repoted by remote client
        // Perhaps it dropped some of the valid transactions..
        for (auto const& leftoverTr : testTransactionMap)
        {
            bool isMarkedInvalid = std::get<1>(leftoverTr.second);
            if (!isMarkedInvalid)
                newBlock.registerTestTransaction(std::get<0>(leftoverTr.second).getCContent());
        }
        // -------

        // Register all test uncles (the one described in test)
        for (auto const& uncle : _uncles)
            newBlock.registerTestUncle(uncle);

        // Ask remote client to generate a parallel blockheader that will later be used for uncles
        if (_generateUncles)
            newBlock.setNextBlockForked(mineNextBlockAndRevert());

        m_blocks.push_back(newBlock);
    }
}

GCP_SPointer<EthGetBlockBy> TestBlockchain::mineBlock(
    BlockchainTestFillerBlock const& _blockInTest, vectorOfSchemeBlock const& _preparedUncleBlocks, BYTES& _rawRLP)
{
    ETH_LOGC("MINE BLOCK: " + m_sDebugString, 6, LogColor::YELLOW);
    m_session.test_mineBlocks(1);
    VALUE latestBlockNumber(m_session.eth_blockNumber());

    auto checkTransactions = [](size_t _trInBlocks, size_t _trInTest, size_t _trAllowedToFail) {
        ETH_ERROR_REQUIRE_MESSAGE(_trInBlocks == _trInTest - _trAllowedToFail,
            "BlockchainTest transaction execution failed! (remote " + fto_string(_trInBlocks) + " != test " +
                fto_string(_trInTest) + ", allowedToFail = " + fto_string(_trAllowedToFail) + " )");
    };

    spFH32 minedBlockHash;
    if (_blockInTest.hasBlockHeader() || _blockInTest.hasRelTimeStamp() || _blockInTest.uncles().size() > 0)
    {
        // Need to overwrite the blockheader of a mined block with either blockHeader or uncles
        // Then import it again and see what client says, because mining with uncles not supported
        // And because blockchain test filler can override blockheader for testing purposes
        ETH_LOG("Postmine blockheader: " + m_sDebugString, 6);
        minedBlockHash = spFH32(new FH32(postmineBlockHeader(_blockInTest, latestBlockNumber, _preparedUncleBlocks, _rawRLP)));
    }

    // Expected exception for this block
    string const& sBlockException = _blockInTest.getExpectException(m_network);
    checkBlockException(sBlockException);  // Check any impoprt failure exceptions here

    GCP_SPointer<EthGetBlockBy> remoteBlock;
    if (!minedBlockHash.isEmpty())
    {
        if (minedBlockHash.getCContent() != FH32::zero())
        {
            // If after modifications block import succeed
            remoteBlock = GCP_SPointer<EthGetBlockBy>(
                new EthGetBlockBy((m_session.eth_getBlockByHash(minedBlockHash.getContent(), Request::FULLOBJECTS))));
        }
        else
            return remoteBlock;  // after modifications block import failed. but that was expected.
    }
    else
    {
        // There was no block modifications, it is just a regular mining
        remoteBlock = GCP_SPointer<EthGetBlockBy>(
            new EthGetBlockBy((m_session.eth_getBlockByNumber(latestBlockNumber, Request::FULLOBJECTS))));
        _rawRLP = remoteBlock.getCContent().getRLPHeaderTransactions();
    }

    checkTransactions(remoteBlock.getContent().transactions().size(), _blockInTest.transactions().size(),
        _blockInTest.invalidTransactionCount());
    return remoteBlock;
}

// Ask remote client to generate a blockheader that will later used for uncles
BlockHeader TestBlockchain::mineNextBlockAndRevert()
{
    ETH_LOGC("Mine uncle block (next block) and revert: " + m_sDebugString, 6, LogColor::YELLOW);
    m_session.test_modifyTimestamp(1000);
    m_session.test_mineBlocks(1);
    VALUE latestBlockNumber(m_session.eth_blockNumber());
    EthGetBlockBy nextBlock(m_session.eth_getBlockByNumber(latestBlockNumber, Request::LESSOBJECTS));
    m_session.test_rewindToBlock(nextBlock.header().number().asU256() - 1);  // rewind to the previous block

    //m_session.test_modifyTimestamp(1000);  // Shift block timestamp relative to previous block

    // assign a random coinbase for an uncle block to avoid UncleIsAncestor exception
    // otherwise this uncle would be similar to a block mined
    DataObject head = nextBlock.header().asDataObject();
    head["coinbase"] = "0xb94f5374fce5ed0000000097c15331677e6ebf0b";  // FH20::random().asString();
    return BlockHeader(head);
}

string TestBlockchain::prepareDebugInfoString(string const& _newBlockChainName)
{
    string sBlockNumber = string();
    size_t newBlockNumber = m_blocks.size();
    TestInfo errorInfo(m_network.asString(), newBlockNumber, _newBlockChainName);
    if (Options::get().logVerbosity >= 6)
        sBlockNumber = fto_string(newBlockNumber);  // very heavy
    TestOutputHelper::get().setCurrentTestInfo(errorInfo);
    m_sDebugString = "(bl: " + sBlockNumber + ", ch: " + _newBlockChainName + ", net: " + m_network.asString() + ")";
    ETH_LOGC("Generating a test block: " + m_sDebugString, 6, LogColor::YELLOW);
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
        firstBlock = (size_t)_number.asU256();
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

        if (actNumber < _number.asU256())
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
    std::vector<BlockHeader> const& _uncles, BYTES& _rawRLP)
{
    // if blockHeader is defined in test Filler, rewrite the last block header fields with info from
    // test and reimport it to the client in order to trigger an exception in the client
    EthGetBlockBy remoteBlock(m_session.eth_getBlockByNumber(_latestBlockNumber, Request::FULLOBJECTS));
    EthereumBlock managedBlock(remoteBlock.header());
    for (auto const& tr : remoteBlock.transactions())
        managedBlock.addTransaction(tr.transaction());

    // Attach test-generated uncle to a block and then reimport it again
    for (auto const& un : _uncles)
        managedBlock.addUncle(un);

    if (_blockInTest.hasBlockHeader())
        managedBlock.replaceHeader(_blockInTest.blockHeader().overwriteBlockHeader(managedBlock.header()));

    // Overwrite blockheader if defined in the test filler
    if (_blockInTest.hasRelTimeStamp())
    {
        EthGetBlockBy previousBlock(m_session.eth_getBlockByNumber(_latestBlockNumber - 1, Request::LESSOBJECTS));
        managedBlock.headerUnsafe().setTimestamp(previousBlock.header().timestamp().asU256() + _blockInTest.relTimeStamp());
    }

    // replace block with overwritten header
    bool weOverwriteHashFields = false;
    if (_blockInTest.hasBlockHeader())
    {
        if (_blockInTest.blockHeader().hasUncleHash() || _blockInTest.blockHeader().hasTransactionHash())
            weOverwriteHashFields = true;
    }

    if (!weOverwriteHashFields)
        managedBlock.recalculateUncleHash();

    m_session.test_rewindToBlock(_latestBlockNumber - 1);
    _rawRLP = BYTES(managedBlock.getRLP().asString());
    return FH32(m_session.test_importRawBlock(_rawRLP));
}

// Returns true if the block is valid
bool TestBlockchain::checkBlockException(string const& _sBlockException) const
{
    // Check malicious block import exception
    // Relies on that previous block import was exactly this block !!!
    if (_sBlockException.empty())
        ETH_ERROR_REQUIRE_MESSAGE(m_session.getLastRPCError().empty(),
            "Postmine block tweak expected no exception! Client errors with: '" + m_session.getLastRPCError().message() + "'");
    else
    {
        std::string const& clientExceptionString =
            Options::get().getDynamicOptions().getCurrentConfig().translateException(_sBlockException);
        size_t pos = m_session.getLastRPCError().message().find(clientExceptionString);
        if (clientExceptionString.empty())
            pos = string::npos;
        ETH_ERROR_REQUIRE_MESSAGE(pos != string::npos,
            cYellow + _sBlockException + cRed + " Not found in client response to postmine block tweak!" +
                "\nImport result of postmine block: \n'" + cYellow + m_session.getLastRPCError().message() + cRed +
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

}  // namespace blockchainfiller
}  // namespace test
