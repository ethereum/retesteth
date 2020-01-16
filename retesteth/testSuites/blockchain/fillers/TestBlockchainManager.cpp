#include "TestBlockchainManager.h"
#include <string>
using namespace std;

// Generate block using a client from the filler information
TestBlockchainManager::parseBlockFromJson(
    test::scheme_blockchainTestFiller::blockSection const& _block)
{
    reorgChains(_block.getChainName(), _block.getNumber());

    TestBlockchain& currentChainMining = mapOfKnownChain.at(m_sCurrentChainName);

    // Prepare uncles using all chains and current debug info
    string sDebug = currentChainMining.prepareDebugInfoString(_block.getChainName());
    vectorOfSchemeBlock unclesPrepared = prepareUncles(_block, sDebug);

    currentChainMining.generateBlock(_block, unclesPrepared);
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


struct TestBlockchainManager::TestBlockchain
{
    // Remember new block
    void registerBlockRLP(std::string const& _blockRLP) { m_blockRlps.push_back(_blockRLP); }

    // Restore this chain on remote client up to < _number block
    // Restore chain up to _number of blocks. if _number is 0 restore the whole chain
    void restoreUpToNumber(RPCSession& _session, size_t _number, bool _samechain = false)
    {
        if (_samechain && _number == 0)
            return;

        size_t firstBlock = _samechain ? _number - 1 : 0;
        _session.test_rewindToBlock(firstBlock);  // Rewind to the begining

        if (_number == 0)
        {
            // we are not on the same chain. restore the whole history
            for (auto const& rlp : m_blockRlps)
                _session.test_importRawBlock(rlp);
            return;
        }

        size_t actNumber = 0;
        size_t popUpCount = 0;
        for (auto const& rlp : m_blockRlps)
        {
            if (actNumber++ < firstBlock)  // we are on the same chain. lets move to the rewind
                                           // point
                continue;

            if (actNumber < _number)
                _session.test_importRawBlock(rlp);
            else
            {
                m_knownBlockRlps.push_back(rlp);  // Remember the fork
                popUpCount++;
                std::cerr << "popup " << std::endl;
            }
        }

        // Restore blocks up to `number` forgetting the rest of history
        for (size_t i = 0; i < popUpCount; i++)
            m_blockRlps.pop_back();
    }

    void importKnownBlocks(RPCSession& _session) const
    {
        // import previously generated blocks that been rewind
        for (auto const& rlp : m_knownBlockRlps)
            _session.test_importRawBlock(rlp);
    }
    std::vector<std::string> const& getBlockRlps() const { return m_blockRlps; }
    vectorOfSchemeBlock const& getUnpreparedUncles() const { return m_uncles; }
    vectorOfSchemeBlock& getUnpreparedUnclesUnsafe() { return m_uncles; }
    std::map<size_t, vectorOfSchemeBlock> const& getMapBlocksToPreparedUncles() const
    {
        return m_mapBlocksToPreparedUncles;
    }
    std::map<size_t, vectorOfSchemeBlock>& getMapBlocksToPreparedUnclesUnsafe()
    {
        return m_mapBlocksToPreparedUncles;
    }

private:
    std::vector<std::string> m_blockRlps;       // List of Imported block RLPs
    std::vector<std::string> m_knownBlockRlps;  // List of fork block RLPs
    vectorOfSchemeBlock m_uncles;  // unprepared uncles mined in paralel from the blocks
    std::map<size_t, vectorOfSchemeBlock> m_mapBlocksToPreparedUncles;  // prepared uncles for each
                                                                        // block
};

TestBlockchainManager::reorgChains(string const& _newBlockChainName, size_t _newBLockNumber)
{
    // if a new chain, initialize
    if (!mapOfKnownChain.count(_newBlockChainName))
        mapOfKnownChain.emplace(_newBlockChainName, TestBlockchain());

    // Chain reorg conditions
    bool blockNumberHasDecreased =
        (_newBLockNumber != 0 &&
            mapOfKnownChain.at(_newBlockChainName).getBlockRlps().size() >= _newBLockNumber);
    bool sameChain = (m_sCurrentChainName == _newBlockChainName);

    if (!blockNumberHasDecreased && sameChain && _newBLockNumber != 0)
        ETH_ERROR_REQUIRE_MESSAGE(
            mapOfKnownChain.at(_newBlockChainName).getBlockRlps().size() == _newBLockNumber,
            "Require a blocknumber = previous blocknumber + 1");

    // if we switch the chain or have to remine one of blocknumbers
    if (m_sCurrentChainName != _newBlockChainName || blockNumberHasDecreased)
    {
        ETH_LOGC("PERFORM REWIND HISTORY: ", 6, LogColor::YELLOW);
        TestBlockchain& chain = mapOfKnownChain.at(_newBlockChainName);
        chain.restoreUpToNumber(m_session, _newBLockNumber, sameChain && blockNumberHasDecreased);
        m_sCurrentChainName = _newBlockChainName;
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
    TestBlockchain const& currentChainMining = mapOfKnownChain.at(m_sCurrentChainName);

    scheme_uncleHeader::typeOfUncleSection typeOfSection = _uncleOverwrite.getTypeOfUncleSection();
    switch (typeOfSection)
    {
    case scheme_uncleHeader::typeOfUncleSection::SameAsPreviousBlockUncle:
    {
        size_t sameAsPreviuousBlockUncle = _uncleOverwrite.getSameAsPreviousBlockUncle();
        ETH_ERROR_REQUIRE_MESSAGE(
            currentChainMining.getMapBlocksToPreparedUncles().count(sameAsPreviuousBlockUncle),
            "Trying to copy uncle from unregistered block with sameAsPreviuousBlockUncle!");
        tmpRefToSchemeBlock =
            &currentChainMining.getMapBlocksToPreparedUncles().at(sameAsPreviuousBlockUncle).at(0);
        break;
    }
    case scheme_uncleHeader::typeOfUncleSection::PopulateFromBlock:
    {
        // _existingUncles (ind 0 - from genesis, ind 1 - form first block)
        origIndex = _uncleOverwrite.getPopulateFrom();
        if (!_uncleOverwrite.getChainName().empty())
        {
            ETH_ERROR_REQUIRE_MESSAGE(mapOfKnownChain.count(_uncleOverwrite.getChainName()),
                "Uncle is populating from non-existent chain!");
            TestBlockchain const& chain = mapOfKnownChain.at(_uncleOverwrite.getChainName());
            ETH_ERROR_REQUIRE_MESSAGE(chain.getUnpreparedUncles().size() > origIndex,
                "Trying to populate uncle from future block in another chain that has not been "
                "generated yet!");
            tmpRefToSchemeBlock = &chain.getUnpreparedUncles().at(origIndex);
        }
        else
        {
            ETH_ERROR_REQUIRE_MESSAGE(currentChainMining.getUnpreparedUncles().size() > origIndex,
                "Trying to populate uncle from future block that has not been generated yet!");
            tmpRefToSchemeBlock = &currentChainMining.getUnpreparedUncles().at(origIndex);
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
        origIndex = _uncleOverwrite.getSameAsPreviousSibling() - 1;
        ETH_ERROR_REQUIRE_MESSAGE(_currentBlockPreparedUncles.size() > origIndex,
            "Trying to get uncle from current block that has not been generated yet!");
        tmpRefToSchemeBlock = &_currentBlockPreparedUncles.at(origIndex);
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
    string const& shift = _uncleOverwrite.getRelTimestampFromPopulateBlock();
    if (!shift.empty())
    {
        BlockNumber populateFromBlockNumber(origIndex);
        test::scheme_block populateFromBlock =
            m_session.eth_getBlockByNumber(populateFromBlockNumber, false);

        //  std::cerr << populateFromBlockNumber.getBlockNumberAsString() << " tmstmp: " <<
        //  populateFromBlock.getBlockHeader().atKey("timestamp").asString() << std::endl;
        BlockNumber timestamp(populateFromBlock.getBlockHeader().atKey("timestamp").asString());
        timestamp.applyShift(shift);
        //  std::cerr << headerOrig.asJson() << std::endl;
        headerOrig.atKeyUnsafe("timestamp") = timestamp.getBlockNumberAsString();
        //  std::cerr << headerOrig.asJson() << std::endl;
    }

    uncleBlock.overwriteBlockHeader(headerOrig);
    std::cerr << uncleBlock.getBlockHeader().asJson() << std::endl;
    return uncleBlock;
}
