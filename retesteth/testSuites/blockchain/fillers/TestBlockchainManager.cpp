#include "TestBlockchainManager.h"
#include <string>
using namespace std;

// Generate block using a client from the filler information
void TestBlockchainManager::parseBlockFromJson(blockSection const& _block, bool _generateUncles)
{
    ETH_LOGC("STARTING A NEW BLOCK: ", 6, LogColor::LIME);

    // See if chain reorg is needed. ex: new fork, or remine block
    reorgChains(_block);

    TestBlockchain& currentChainMining = m_mapOfKnownChain.at(m_sCurrentChainName);

    // Prepare uncles using all chains and current debug info
    string sDebug = currentChainMining.prepareDebugInfoString(_block.getChainName());

    // Check that we don't shift the chain after the initialization
    if (!_block.getChainNetwork().empty())
    {
        ETH_ERROR_REQUIRE_MESSAGE(currentChainMining.getNetwork() == _block.getChainNetwork(),
            "Trying to switch chainname with the following block! (chain: " +
                currentChainMining.getNetwork() + ", block: " + _block.getChainNetwork() + ")");
    }

    vectorOfSchemeBlock unclesPrepared =
        _generateUncles ? prepareUncles(_block, sDebug) : vectorOfSchemeBlock();
    currentChainMining.generateBlock(_block, unclesPrepared, _generateUncles);

    // Remeber the generated block in exact order as in the test
    TestBlock const& lastBlock = getLastBlock();

    // Get this block exception on canon chain to later verify it
    string const& canonNet = m_mapOfKnownChain.at(m_sDefaultChainName).getNetwork();
    m_testBlockRLPs.push_back(std::make_tuple(lastBlock.getRLP(), _block.getException(canonNet)));
}

// Import all generated blocks at the same order as they are in tests
void TestBlockchainManager::syncOnRemoteClient(DataObject& _exportBlocksSection) const
{
    if (m_wasAtLeastOneFork)
    {
        // !!! RELY ON _exportBlocksSection has the same block order as m_testBlockRLPs
        ETH_LOGC("IMPORT KNOWN BLOCKS ", 6, LogColor::LIME);
        TestBlockchain const& chain = m_mapOfKnownChain.at(m_sDefaultChainName);
        chain.resetChainParams();  // restore canon chain of the test
        size_t ind = 0;
        for (auto const& rlpAndException : m_testBlockRLPs)
        {
            TestInfo errorInfo(m_sDefaultChainNet, ind + 1, "AllKnown");
            TestOutputHelper::get().setCurrentTestInfo(errorInfo);

            m_session.test_importRawBlock(std::get<0>(rlpAndException));
            string const& canonExcept = std::get<1>(rlpAndException);
            bool isValid = chain.checkBlockException(canonExcept);  // Check on canon exception
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

vectorOfSchemeBlock TestBlockchainManager::prepareUncles(
    blockSection const& _block, string const& _debug)
{
    ETH_LOGC("Prepare Uncles for the block: " + _debug, 6, LogColor::YELLOW);
    vectorOfSchemeBlock preparedUncleBlocks;  // Prepared uncles for the current block
    // return block header using uncle overwrite section on uncles array from test
    for (auto const& uncle : _block.getUncles())
        preparedUncleBlocks.push_back(prepareUncle(uncle, preparedUncleBlocks));
    return preparedUncleBlocks;
}

void TestBlockchainManager::reorgChains(blockSection const& _block)
{
    // if a new chain, initialize
    size_t newBlockNumber = _block.getNumber();
    string const& newBlockChainName = _block.getChainName();
    string const& newBlockChainNet = _block.getChainNetwork();
    if (!m_mapOfKnownChain.count(newBlockChainName))
    {
        // regenerate genesis only if the chain fork has changed
        m_mapOfKnownChain.emplace(newBlockChainName,
            TestBlockchain(m_session, m_testObject,
                newBlockChainNet.empty() ? m_sDefaultChainNet : newBlockChainNet,
                m_sDefaultChainNet != newBlockChainNet ? TestBlockchain::RegenerateGenesis::TRUE :
                                                         TestBlockchain::RegenerateGenesis::FALSE));
    }

    // Chain reorg conditions
    bool blockNumberHasDecreased =
        (newBlockNumber != 0 &&
            m_mapOfKnownChain.at(newBlockChainName).getBlocks().size() - 1 >= newBlockNumber);
    bool sameChain = (m_sCurrentChainName == newBlockChainName);

    if (!blockNumberHasDecreased && sameChain && newBlockNumber != 0)
    {
        // 0 is genesis. Check the block order
        ETH_ERROR_REQUIRE_MESSAGE(
            newBlockNumber == m_mapOfKnownChain.at(newBlockChainName).getBlocks().size(),
            "Require a `new blocknumber` = `previous blocknumber` + 1 has (" +
                toString(newBlockNumber) + " vs " +
                toString(m_mapOfKnownChain.at(newBlockChainName).getBlocks().size()) + ")");
        BlockNumber actualNumberOnTheClient = m_session.eth_blockNumber();
        actualNumberOnTheClient.applyShift(1);  // next block number to be mined
        if (newBlockNumber != (size_t)actualNumberOnTheClient.getBlockNumberAsInt())
            ETH_WARNING("Test mining blocknumber `" + to_string(newBlockNumber) +
                        "`, but client actually mine number: `" +
                        actualNumberOnTheClient.getBlockNumberAsString() + "`");
    }

    // if we switch the chain or have to remine one of blocknumbers
    if (!sameChain || blockNumberHasDecreased)
    {
        m_wasAtLeastOneFork = true;
        ETH_LOGC("PERFORM REWIND HISTORY:  (current: " + m_sCurrentChainName +
                     ", new: " + newBlockChainName + ")",
            6, LogColor::YELLOW);
        TestBlockchain& chain = m_mapOfKnownChain.at(newBlockChainName);
        if (!sameChain)
        {
            if (m_mapOfKnownChain.at(m_sCurrentChainName).getNetwork() != chain.getNetwork())
                chain.resetChainParams();  // Reset genesis because chains have different config
            m_sCurrentChainName = newBlockChainName;
        }
        chain.restoreUpToNumber(m_session, newBlockNumber, sameChain && blockNumberHasDecreased);
    }

    m_session.test_modifyTimestamp(1000);  // Shift block timestamp relative to previous block
}

// _session is RPC connection to the client
// _uncleOverwrite is read from the test filler
// _existingUncles are blocks mined normally from the client as if it were uncles
// _preparedUncles is a map blNumber -> uncleHeaders where blNumber are existing blocks
// return a blockHeader info for _existingUncle overwritten by _uncleOverwrite
// prepareUncle(uncle, preparedUncleBlocks);
test::scheme_block TestBlockchainManager::prepareUncle(
    scheme_uncleHeader _uncleOverwrite, vectorOfSchemeBlock const& _currentBlockPreparedUncles)
{
    size_t origIndex = 0;
    test::scheme_block const* tmpRefToSchemeBlock = NULL;
    TestBlockchain const& currentChainMining = m_mapOfKnownChain.at(m_sCurrentChainName);

    scheme_uncleHeader::typeOfUncleSection typeOfSection = _uncleOverwrite.getTypeOfUncleSection();
    switch (typeOfSection)
    {
    case scheme_uncleHeader::typeOfUncleSection::SameAsPreviousBlockUncle:
    {
        size_t sameAsPreviuousBlockUncle = _uncleOverwrite.getSameAsPreviousBlockUncle();
        ETH_ERROR_REQUIRE_MESSAGE(currentChainMining.getBlocks().size() > sameAsPreviuousBlockUncle,
            "Trying to copy uncle from unregistered block with sameAsPreviuousBlockUncle!");
        ETH_ERROR_REQUIRE_MESSAGE(
            currentChainMining.getBlocks().at(sameAsPreviuousBlockUncle).getUncles().size() > 0,
            "Previous block has no uncles!");
        tmpRefToSchemeBlock =
            &currentChainMining.getBlocks().at(sameAsPreviuousBlockUncle).getUncles().at(0);
        break;
    }
    case scheme_uncleHeader::typeOfUncleSection::PopulateFromBlock:
    {
        // _existingUncles (chain.getBlocks() ind 0 genesis, ind 1 - first block, ind 2 - second
        // block) uncle populated from origIndex is stored at the next block as ForkBlock
        origIndex = _uncleOverwrite.getPopulateFrom();
        if (!_uncleOverwrite.getChainName().empty())
        {
            ETH_ERROR_REQUIRE_MESSAGE(m_mapOfKnownChain.count(_uncleOverwrite.getChainName()),
                "Uncle is populating from non-existent chain!");
            TestBlockchain const& chain = m_mapOfKnownChain.at(_uncleOverwrite.getChainName());
            ETH_ERROR_REQUIRE_MESSAGE(chain.getBlocks().size() > origIndex,
                "Trying to populate uncle from future block in another chain that has not been "
                "generated yet!");
            tmpRefToSchemeBlock = &chain.getBlocks().at(origIndex).getNextBlockForked();
        }
        else
        {
            ETH_ERROR_REQUIRE_MESSAGE(currentChainMining.getBlocks().size() > origIndex,
                "Trying to populate uncle from future block that has not been generated yet!");
            tmpRefToSchemeBlock =
                &currentChainMining.getBlocks().at(origIndex).getNextBlockForked();
        }
        break;
    }
    case scheme_uncleHeader::typeOfUncleSection::SameAsBlock:
    {
        BlockNumber sameAsBlockNumber(_uncleOverwrite.getSameAsBlock());
        return m_session.eth_getBlockByNumber(sameAsBlockNumber, false);
    }
    case scheme_uncleHeader::typeOfUncleSection::SameAsPreviousSibling:
    {
        size_t siblingNumber =
            _uncleOverwrite.getSameAsPreviousSibling() - 1;  // 1 is first sib, 2 is next,...
        ETH_ERROR_REQUIRE_MESSAGE(siblingNumber < _currentBlockPreparedUncles.size(),
            "Trying to get uncle from current block that has not been generated yet!");
        tmpRefToSchemeBlock = &_currentBlockPreparedUncles.at(siblingNumber);
        break;
    }
    default:
        ETH_ERROR_MESSAGE("Unhandled typeOfUncleSection!");
        break;
    }

    if (tmpRefToSchemeBlock == NULL)
        ETH_ERROR_MESSAGE("tmpRefToSchemeBlock is NULL!");
    test::scheme_block uncleBlock = *tmpRefToSchemeBlock;
    DataObject headerOrig = uncleBlock.getBlockHeader();

    // If there is a field that is being overwritten in the uncle header
    if (_uncleOverwrite.getOverwrite().size())
    {
        for (auto const& overwriteField : _uncleOverwrite.getOverwrite())
            headerOrig.atKeyUnsafe(overwriteField) =
                _uncleOverwrite.getData().atKey(overwriteField).asString();
    }

    // If uncle timestamp is shifted relative to the block that it's populated from
    if (typeOfSection == scheme_uncleHeader::typeOfUncleSection::PopulateFromBlock)
    {
        string const& shift = _uncleOverwrite.getRelTimestampFromPopulateBlock();
        if (!shift.empty())
        {
            DataObject const& blockDataForTest =
                currentChainMining.getBlocks().at(origIndex).getDataForTest();
            BlockNumber timestamp(
                blockDataForTest.atKey("blockHeader").atKey("timestamp").asString());

            timestamp.applyShift(shift);
            headerOrig.atKeyUnsafe("timestamp") = timestamp.getBlockNumberAsString();
            //  std::cerr << headerOrig.asJson() << std::endl;
        }
    }

    uncleBlock.overwriteBlockHeader(headerOrig);
    // std::cerr << uncleBlock.getBlockHeader().asJson() << std::endl;
    return uncleBlock;
}
