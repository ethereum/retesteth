#include "TestBlockchainManager.h"
#include <retesteth/helpers/TestHelper.h>
using namespace std;
using namespace test::debug;
using namespace test::session;

namespace test
{
namespace blockchainfiller
{
// Initialize blockchain manager with first chain information
// _env, _pre, _engine, _network
TestBlockchainManager::TestBlockchainManager(
    BlockchainTestFillerEnv const& _genesisEnv, State const& _genesisPre, SealEngine _engine, FORK const& _network)
  : m_session(RPCSession::instance(TestOutputHelper::getThreadID())),
    m_sDefaultChainName(BlockchainTestFillerBlock::defaultChainName()),
    m_genesisEnv(_genesisEnv),
    m_genesisPre(_genesisPre),
    m_sealEngine(_engine),
    m_sDefaultChainNet(_network)
{
    m_wasAtLeastOneFork = false;
    // m_sCurrentChainName is unknown at this point. the first block of the test defines it
    // but we want genesis to be generated anyway before that
    m_sCurrentChainName = m_sDefaultChainName;
    m_mapOfKnownChain.emplace(m_sCurrentChainName,
        TestBlockchain(_genesisEnv, _genesisPre, _engine, _network, m_sCurrentChainName, RegenerateGenesis::TRUE));
}

// Generate the block
void TestBlockchainManager::_makeTheFilledBlockFromFiller(BlockchainTestFillerBlock const& _block, vectorOfSchemeBlock const& _unclesPrepared, bool _generateUncles)
{
    TestBlockchain& currentChainMining = getCurrentChain();
    currentChainMining.generateBlock(_block, _unclesPrepared, _generateUncles);

    // Remeber the generated block in exact order as in the test
    TestBlock const& lastBlock = getLastBlock();

    // Get this block exception on canon chain to later verify it
    FORK const& canonNet = getDefaultChain().getNetwork();
    m_testBlockRLPs.emplace_back(std::make_tuple(lastBlock.getRawRLP(), _block.getExpectException(canonNet)));
}

std::vector<spDataObject> TestBlockchainManager::_generateBlocksFromFillerTestBlock(BlockchainTestFillerBlock const& _block, vectorOfSchemeBlock const& _unclesPrepared, bool _generateUncles)
{
    std::vector<spDataObject> blockJsons;
    TestBlockchain& currentChainMining = getCurrentChain();
    std::vector<BlockchainTestFillerTransaction const*> validTransactions;

    for (BlockchainTestFillerTransaction const& tr : _block.transactions())
    {
        // If not exception on current chain network, build a valid transacions
        auto const& currentFork = currentChainMining.getNetwork();
        auto const& trException = tr.getExpectException(currentFork);
        if (trException.empty())
        {
            validTransactions.emplace_back(&tr);
        }
        else
        {
            BlockchainTestFillerBlock validBlockSoFar(_block, true);

            if (!_block.isDoNotStackValidTrxs())
                for (auto const& tr : validTransactions)
                    validBlockSoFar.addTransaction(*tr);

            // add next invalid transaction
            validBlockSoFar.addTransaction(tr);
            validBlockSoFar.addException(currentFork, trException);

            _makeTheFilledBlockFromFiller(validBlockSoFar, _unclesPrepared, _generateUncles);

            // If block is not disabled for testing purposes
            // Get the json output of a constructed block for the test (includes rlp)
            if (!getLastBlock().isDoNotExport())
                blockJsons.emplace_back(getLastBlock().asDataObject());
        }
    }

    // Make the last block of only valid transactions
    BlockchainTestFillerBlock validBlockSoFar(_block, true);
    for (auto const& tr : validTransactions)
        validBlockSoFar.addTransaction(*tr);
    _makeTheFilledBlockFromFiller(validBlockSoFar, _unclesPrepared, _generateUncles);

    // If block is not disabled for testing purposes
    // Get the json output of a constructed block for the test (includes rlp)
    if (!getLastBlock().isDoNotExport())
        blockJsons.emplace_back(getLastBlock().asDataObject());
    return blockJsons;
}

// Generate block using a client from the filler information
std::vector<spDataObject> TestBlockchainManager::parseBlockFromFiller(BlockchainTestFillerBlock const& _block, bool _generateUncles)
{
    ETH_DC_MESSAGEC(DC::RPC, "STARTING A NEW BLOCK: ", LogColor::LIME);

    // See if chain reorg is needed. ex: new fork, or remine block
    reorgChains(_block);

    TestBlockchain& currentChainMining = getCurrentChain();

    // Prepare uncles using all chains and current debug info
    string sDebug = currentChainMining.prepareDebugInfoString(_block.chainName());

    // Check that we don't shift the chain after the initialization
    if (_block.hasChainNet())
    {
        ETH_ERROR_REQUIRE_MESSAGE(currentChainMining.getNetwork() == _block.chainNet(),
            "Trying to switch chainname with the following block! (chain: " + currentChainMining.getNetwork().asString() +
                ", block: " + _block.chainNet().asString() + ")");
    }

    // Generate UncleHeaders if we need it
    vectorOfSchemeBlock unclesPrepared = _generateUncles ? prepareUncles(_block, sDebug) : vectorOfSchemeBlock();

    size_t invalidTxs = 0;
    for (auto const& tr : _block.transactions())
    {
        auto const& currentFork = currentChainMining.getNetwork();
        auto const& trException = tr.getExpectException(currentFork);
        if (!trException.empty() && _block.getExpectException(currentFork).empty() && ++invalidTxs == 2)
        {
            ETH_DC_MESSAGE(DC::STATS2, "Block has multiple invalid transactions, will construct many test blocks instead of tr sequence!");
            return _generateBlocksFromFillerTestBlock(_block, unclesPrepared, _generateUncles);
        }
    }

    // Generate the block
    currentChainMining.generateBlock(_block, unclesPrepared, _generateUncles);

    // Remeber the generated block in exact order as in the test
    TestBlock const& lastBlock = getLastBlock();

    // Get this block exception on canon chain to later verify it
    FORK const& canonNet = getDefaultChain().getNetwork();
    m_testBlockRLPs.emplace_back(std::make_tuple(lastBlock.getRawRLP(), _block.getExpectException(canonNet)));
    if (!lastBlock.isDoNotExport())
        return {lastBlock.asDataObject()};
    return {};
}

TestBlockchain& TestBlockchainManager::getDefaultChain()
{
    assert(m_mapOfKnownChain.count(m_sDefaultChainName));
    return m_mapOfKnownChain.at(m_sDefaultChainName);
}

TestBlockchain& TestBlockchainManager::getCurrentChain()
{
    assert(m_mapOfKnownChain.count(m_sCurrentChainName));
    return m_mapOfKnownChain.at(m_sCurrentChainName);
}

TestBlock const& TestBlockchainManager::getLastBlock()
{
    assert(m_mapOfKnownChain.count(m_sCurrentChainName));
    TestBlockchain const& chain = m_mapOfKnownChain.at(m_sCurrentChainName);
    assert(chain.getBlocks().size() > 0);
    return chain.getBlocks().at(chain.getBlocks().size() - 1);
}

// Import all generated blocks at the same order as they are in tests
void TestBlockchainManager::syncOnRemoteClient(DataObject& _exportBlocksSection) const
{
    if (m_wasAtLeastOneFork)
    {
        // !!! RELY ON _exportBlocksSection has the same block order as m_testBlockRLPs
        ETH_DC_MESSAGEC(DC::RPC, "IMPORT KNOWN BLOCKS ", LogColor::LIME);
        TestBlockchain const& chain = m_mapOfKnownChain.at(m_sDefaultChainName);
        chain.resetChainParams();  // restore canon chain of the test
        size_t ind = 0;
        for (auto const& rlpAndException : m_testBlockRLPs)
        {
            TestOutputHelper::get().setCurrentTestInfo(TestInfo(m_sDefaultChainNet.asString(), ind + 1, "AllKnown"));

            m_session.test_importRawBlock(std::get<0>(rlpAndException));
            string const& canonExcept = std::get<1>(rlpAndException);
            bool isValid = chain.checkBlockException(m_session, canonExcept);  // Check on canon exception
            if (!isValid)
            {
                DataObject& testObj = _exportBlocksSection.atUnsafe(ind);
                if (testObj.count("transactions"))
                    testObj.removeKey("transactions");
                if (testObj.count("uncleHeaders"))
                    testObj.removeKey("uncleHeaders");
                if (testObj.count("blockHeader"))
                    testObj.removeKey("blockHeader");
                testObj["expectException"] = canonExcept;
            }
            ind++;
        }
    }
}

vectorOfSchemeBlock TestBlockchainManager::prepareUncles(BlockchainTestFillerBlock const& _block, string const& _debug)
{
    ETH_DC_MESSAGEC(DC::RPC, "Prepare Uncles for the block: " + _debug, LogColor::YELLOW);
    vectorOfSchemeBlock preparedUncleBlocks;  // Prepared uncles for the current block
    // return block header using uncle overwrite section on uncles array from test
    for (auto const& uncle : _block.uncles())
        preparedUncleBlocks.emplace_back(prepareUncle(uncle, preparedUncleBlocks));
    return preparedUncleBlocks;
}

void TestBlockchainManager::reorgChains(BlockchainTestFillerBlock const& _block)
{
    // if a new chain, initialize
    FORK const& newBlockChainNet = _block.hasChainNet() ? _block.chainNet() : m_sDefaultChainNet;
    VALUE const& newBlockNumber = _block.hasNumber() ? _block.number() : getCurrentChain().getBlocks().size();
    string const& newBlockChainName = _block.chainName();
    if (!m_mapOfKnownChain.count(newBlockChainName))
    {
        // Regenerate genesis only if the chain fork has changed
        m_mapOfKnownChain.emplace(newBlockChainName,
            TestBlockchain(m_genesisEnv, m_genesisPre, m_sealEngine, newBlockChainNet, newBlockChainName,
                m_sDefaultChainNet != newBlockChainNet ? RegenerateGenesis::TRUE : RegenerateGenesis::FALSE));
    }

    // Chain reorg conditions
    assert(m_mapOfKnownChain.count(newBlockChainName));
    const int blocksInChain = m_mapOfKnownChain.at(newBlockChainName).getBlocks().size() - 1;
    bool blockNumberHasDecreased = (newBlockNumber.asBigInt() != 0 && blocksInChain >= newBlockNumber.asBigInt());
    bool sameChain = (m_sCurrentChainName == newBlockChainName);

    if (!blockNumberHasDecreased && sameChain && newBlockNumber != 0)
    {
        // 0 is genesis. Check the block order:
        ETH_ERROR_REQUIRE_MESSAGE(newBlockNumber == blocksInChain + 1,
            "Require a `new blocknumber` == `previous blocknumber` + 1 has (" + newBlockNumber.asDecString() + " vs " +
                fto_string(blocksInChain + 1) + ")");

        VALUE actualNumberOnTheClient(m_session.eth_blockNumber() + 1);
        if (newBlockNumber != actualNumberOnTheClient)
            ETH_WARNING("Test mining blocknumber `" + newBlockNumber.asDecString() + "`, but client actually mine number: `" +
                        actualNumberOnTheClient.asDecString() + "`");
    }

    // if we switch the chain or have to remine one of blocknumbers
    if (!sameChain || blockNumberHasDecreased)
    {
        m_wasAtLeastOneFork = true;
        ETH_DC_MESSAGEC(DC::RPC,
            "PERFORM REWIND HISTORY:  (current: " + m_sCurrentChainName + ", new: " + newBlockChainName + ")",
            LogColor::YELLOW);

        TestBlockchain& chain = m_mapOfKnownChain.at(newBlockChainName);
        if (!sameChain)
        {
            if (getCurrentChain().getNetwork() != chain.getNetwork())
                chain.resetChainParams();  // Reset genesis because chains have different config
            m_sCurrentChainName = newBlockChainName;
        }
        chain.restoreUpToNumber(m_session, newBlockNumber, sameChain && blockNumberHasDecreased);
    }

    {
        VALUE latestBlockNumber(m_session.eth_blockNumber());
        EthGetBlockBy const latestBlock(m_session.eth_getBlockByNumber(latestBlockNumber, Request::LESSOBJECTS));
        m_session.test_modifyTimestamp(latestBlock.header()->timestamp() + 1000);
    }
}

// Read test filler uncle section in block _uncleOverwrite
// And make uncle header out of it using information of currentChain and _currentBlockPreparedUncles
spBlockHeader TestBlockchainManager::prepareUncle(
    BlockchainTestFillerUncle _uncleSectionInTest, vectorOfSchemeBlock const& _currentBlockPreparedUncles)
{
    size_t origIndex = 0;
    BlockHeader const* tmpRefToSchemeBlock = NULL;
    TestBlockchain const& currentChainMining = getCurrentChain();

    UncleType typeOfSection = _uncleSectionInTest.uncleType();
    switch (typeOfSection)
    {
    case UncleType::SameAsPreviousBlockUncle:
    {
        // Make this uncle same as int(`sameAsPreviuousBlockUncle`) block's uncle
        size_t sameAsPreviuousBlockUncle = _uncleSectionInTest.sameAsPreviousBlockUncle();
        ETH_ERROR_REQUIRE_MESSAGE(currentChainMining.getBlocks().size() > sameAsPreviuousBlockUncle,
            "Trying to copy uncle from unregistered block with sameAsPreviuousBlockUncle!");

        if (!currentChainMining.getBlocks().at(sameAsPreviuousBlockUncle).isThereTestHeader())
            ETH_FAIL_MESSAGE("SameAsPreviousBlockUncle::Trying to get Uncles from invalid block#" +  test::fto_string(sameAsPreviuousBlockUncle));

        ETH_ERROR_REQUIRE_MESSAGE(
            currentChainMining.getBlocks().at(sameAsPreviuousBlockUncle).getUncles().size() > 0,
            "Previous block has no uncles!");
        tmpRefToSchemeBlock = &currentChainMining.getBlocks().at(sameAsPreviuousBlockUncle).getUncles().at(0).getCContent();
        break;
    }
    case UncleType::PopulateFromBlock:
    {
        // _existingUncles (chain.getBlocks() ind 0 genesis, ind 1 - first block, ind 2 - second block)
        //  uncle populated from origIndex is stored at the next block as ForkBlock
        origIndex = _uncleSectionInTest.populateFromBlock();
        if (!_uncleSectionInTest.chainname().empty())
        {
            ETH_ERROR_REQUIRE_MESSAGE(
                m_mapOfKnownChain.count(_uncleSectionInTest.chainname()), "Uncle is populating from non-existent chain!");
            TestBlockchain const& chain = m_mapOfKnownChain.at(_uncleSectionInTest.chainname());
            ETH_ERROR_REQUIRE_MESSAGE(chain.getBlocks().size() > origIndex,
                "Trying to populate uncle from future block in another chain that has not been generated yet!");
            tmpRefToSchemeBlock = &chain.getBlocks().at(origIndex).getNextBlockForked().getCContent();
        }
        else
        {
            ETH_ERROR_REQUIRE_MESSAGE(currentChainMining.getBlocks().size() > origIndex,
                "Trying to populate uncle from future block that has not been generated yet!");
            tmpRefToSchemeBlock = &currentChainMining.getBlocks().at(origIndex).getNextBlockForked().getCContent();
        }
        break;
    }
    case UncleType::SameAsBlock:
    {
        EthGetBlockBy asBlock(m_session.eth_getBlockByNumber(_uncleSectionInTest.sameAsBlock(), Request::LESSOBJECTS));
        return asBlock.header();
    }
    case UncleType::SameAsPreviousSibling:
    {
        size_t siblingNumber = _uncleSectionInTest.sameAsPreviousSibling() - 1;  // 1 is first sib, 2 is next,...
        ETH_ERROR_REQUIRE_MESSAGE(siblingNumber < _currentBlockPreparedUncles.size(),
            "Trying to get uncle that has not been generated yet from current block!");
        tmpRefToSchemeBlock = &_currentBlockPreparedUncles.at(siblingNumber).getCContent();
        break;
    }
    default:
        ETH_ERROR_MESSAGE("Unhandled typeOfUncleSection!");
        break;
    }

    if (tmpRefToSchemeBlock == NULL)
        ETH_ERROR_MESSAGE("tmpRefToSchemeBlock is NULL!");
    spBlockHeader uncleBlockHeader = readBlockHeader((*tmpRefToSchemeBlock).asDataObject());

    // Perform uncle header modifications according to the uncle section in blockchain test filler block
    // If there is a field that is being overwritten in the uncle header
    if (_uncleSectionInTest.hasOverwriteHeader())
        uncleBlockHeader = _uncleSectionInTest.overwriteHeader().overwriteBlockHeader(uncleBlockHeader);

    // If uncle timestamp is shifted relative to the block that it's populated from
    if (typeOfSection == UncleType::PopulateFromBlock)
    {
        if (_uncleSectionInTest.hasRelTimestampFromPopulateBlock())
        {
            // Get the Timestamp of that block (which uncle is populated from)
            assert(currentChainMining.getBlocks().size() > origIndex);
            VALUE timestamp(currentChainMining.getBlocks().at(origIndex).getTestHeader()->timestamp());
            (*uncleBlockHeader).setTimestamp(timestamp + _uncleSectionInTest.relTimestampFromPopulateBlock());
        }
    }

    // Recalculate uncleHash because we will be checking which uncle hash will be returned by the client
    uncleBlockHeader.getContent().recalculateHash();
    return uncleBlockHeader;
}

void TestBlockchainManager::performOptionCommandsOnGenesis()
{
    TestBlockchain& currentChainMining = getCurrentChain();
    currentChainMining.performOptionCommandsOnGenesis();
}

}  // namespace blockchainfiller
}  // namespace test
