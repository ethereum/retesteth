#include "TestBlockchain.h"

void TestBlockchain::generateBlock(
    blockSection const& _block, vectorOfSchemeBlock const& _uncles, bool _generateUncles)
{
    TestBlock newBlock;
    DataObject& blockJson = newBlock.getDataForTestUnsafe();

    blockJson["chainname"] = _block.getChainName();
    blockJson["blocknumber"] = toString(m_blocks.size());

    // Prepare transactions for the block
    blockJson["transactions"] = importTransactions(_block);

    // Put prepared uncles into dataobject
    blockJson["uncleHeaders"] = DataObject(DataType::Array);
    for (auto const& uncle : _uncles)
    {
        blockJson["uncleHeaders"].addArrayObject(uncle.getBlockHeader());
        newBlock.addUncle(uncle);
    }

    // Remote client generate block with transactions
    // An if it has uncles or blockheader overwrite, then perform postmine
    test::scheme_block latestBlock = mineBlock(_block, _uncles);
    if (!latestBlock.isValid())
    {
        blockJson.removeKey("transactions");
        blockJson.removeKey("uncleHeaders");
        blockJson["expectException"] =
            _block.getException(m_network);  // must be checked on canon chain as well
        // but we can only check on canon chain after the whole chains are imported
    }
    else
    {
        blockJson["blockHeader"] = latestBlock.getBlockHeader();
        if (_block.getData().count("invalidTransactionsCount"))
            blockJson["invalidTransactionsCount"] =
                _block.getData().atKey("invalidTransactionsCount").asString();
    }
    blockJson["rlp"] = latestBlock.getBlockRLP();

    if (_generateUncles)
    {
        // Ask remote client to generate a parallel blockheader that will later be used for uncles
        newBlock.setNextBlockForked(mineNextBlockAndRewert());
    }

    m_blocks.push_back(newBlock);
}

test::scheme_block TestBlockchain::mineBlock(
    blockSection const& _block, vectorOfSchemeBlock const& _preparedUncleBlocks)
{
    ETH_LOGC("MINE BLOCK: " + m_sDebugString, 6, LogColor::YELLOW);
    BlockNumber latestBlockNumber(m_session.test_mineBlocks(1));
    bool isUnclesInTest = _block.getData().count("uncleHeaders") ?
                              _block.getData().atKey("uncleHeaders").getSubObjects().size() > 0 :
                              false;

    auto checkTransactions = [](size_t _trInBlocks, size_t _trInTest, size_t _trAllowedToFail) {
        ETH_ERROR_REQUIRE_MESSAGE(_trInBlocks == _trInTest - _trAllowedToFail,
            "BlockchainTest transaction execution failed! (remote " + toString(_trInBlocks) +
                " != test " + toString(_trInTest) +
                ", allowedToFail = " + toString(_trAllowedToFail) + " )");
    };

    // Need to overwrite the blockheader of a mined block
    if (_block.getData().count("blockHeader") || isUnclesInTest)
    {
        ETH_LOG("Postmine blockheader: " + m_sDebugString, 6);
        scheme_block latestBlock =
            postmineBlockHeader(_block, latestBlockNumber, _preparedUncleBlocks);
        checkTransactions(latestBlock.getTransactionCount(), _block.getTransactions().size(),
            _block.getInvalidTransactionCount());
        return latestBlock;
    }
    else
    {
        scheme_block latestBlock = m_session.eth_getBlockByNumber(latestBlockNumber, true);
        checkTransactions(latestBlock.getTransactionCount(), _block.getTransactions().size(),
            _block.getInvalidTransactionCount());
        return latestBlock;
    }
}

// Import transactions on remote client
DataObject TestBlockchain::importTransactions(blockSection const& _block)
{
    DataObject transactionsArray = DataObject(DataType::Array);
    ETH_LOGC("Import transactions: " + m_sDebugString, 6, LogColor::YELLOW);
    for (auto const& tr : _block.getTransactions())
    {
        m_session.eth_sendRawTransaction(tr.getSignedRLP());
        transactionsArray.addArrayObject(tr.getDataForBCTest());
    }
    return transactionsArray;
}

// Ask remote client to generate a blockheader that will later used for uncles
test::scheme_block TestBlockchain::mineNextBlockAndRewert()
{
    ETH_LOGC("Mine uncle block (next block) and revert: " + m_sDebugString, 6, LogColor::YELLOW);
    BlockNumber latestBlockNumber(m_session.test_mineBlocks(1));
    test::scheme_block next = m_session.eth_getBlockByNumber(latestBlockNumber, false);
    latestBlockNumber.applyShift(-1);
    m_session.test_rewindToBlock(latestBlockNumber.getBlockNumberAsInt());  // rewind to the
                                                                            // previous block
    m_session.test_modifyTimestamp(1000);  // Shift block timestamp relative to previous block

    // assign a random coinbase for an uncle block to avoid UncleIsAncestor exception
    // otherwise this uncle would be similar to a block mined
    next.randomizeCoinbase();
    return next;
}

string TestBlockchain::prepareDebugInfoString(string const& _newBlockChainName)
{
    string sBlockNumber;
    size_t newBlockNumber = m_blocks.size();
    TestInfo errorInfo(m_network, newBlockNumber, _newBlockChainName);
    if (Options::get().logVerbosity >= 6)
        sBlockNumber = toString(newBlockNumber);  // very heavy
    TestOutputHelper::get().setCurrentTestInfo(errorInfo);
    m_sDebugString =
        "(bl: " + sBlockNumber + ", ch: " + _newBlockChainName + ", net: " + m_network + ")";
    ETH_LOGC("Generating a test block: " + m_sDebugString, 6, LogColor::YELLOW);
    return m_sDebugString;
}


// Restore this chain on remote client up to < _number block
// Restore chain up to _number of blocks. if _number is 0 restore the whole chain
void TestBlockchain::restoreUpToNumber(RPCSession& _session, size_t _number, bool _samechain)
{
    size_t firstBlock;
    if (_samechain)
    {
        if (_number == 0)
            return;
        firstBlock = _number;
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
            _session.test_importRawBlock(block.getRLP());
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

        if (actNumber < _number)
            _session.test_importRawBlock(block.getRLP());
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


test::scheme_block TestBlockchain::postmineBlockHeader(blockSection const& _blockInTest,
    BlockNumber const& _latestBlockNumber, std::vector<scheme_block> const& _uncles)
{
    // if blockHeader is defined in test Filler, rewrite the last block header fields with info from
    // test and reimport it to the client in order to trigger an exception in the client
    test::scheme_block remoteBlock = m_session.eth_getBlockByNumber(_latestBlockNumber, true);
    bool changeDoneToTheBlock = false;

    // Attach test-generated uncle to a block and then reimport it again
    if (_blockInTest.getData().count("uncleHeaders") &&
        _blockInTest.getData().atKey("uncleHeaders").getSubObjects().size() > 0)
    {
        for (auto const& bl : _uncles)
            remoteBlock.addUncle(bl);
        remoteBlock.recalculateUncleHash();
        changeDoneToTheBlock = true;
    }

    DataObject header = remoteBlock.getBlockHeader();

    // Overwrite blockheader if defined in the test filler
    if (_blockInTest.getData().count("blockHeader"))
    {
        for (auto const& replace : _blockInTest.getData().atKey("blockHeader").getSubObjects())
        {
            if (replace.getKey() == "updatePoW" || replace.getKey() == "expectException")
                continue;
            if (replace.getKey() == "RelTimestamp")
            {
                BlockNumber previousBlockNumber(_latestBlockNumber);
                previousBlockNumber.applyShift(-1);

                test::scheme_block previousBlock =
                    m_session.eth_getBlockByNumber(previousBlockNumber, false);
                string previousBlockTimestampString =
                    previousBlock.getBlockHeader().atKey("timestamp").asString();
                BlockNumber previousBlockTimestamp(previousBlockTimestampString);
                previousBlockTimestamp.applyShift(replace.asString());
                header["timestamp"] = previousBlockTimestamp.getBlockNumberAsString();
                changeDoneToTheBlock = true;
                continue;
            }
            if (header.count(replace.getKey()))
            {
                header[replace.getKey()] = replace.asString();
                changeDoneToTheBlock = true;
            }
            else
                ETH_STDERROR_MESSAGE(
                    "blockHeader field in test filler tries to overwrite field that is not found "
                    "in "
                    "blockheader: '" +
                    replace.getKey() + "'");
        }
    }

    // replace block with overwritten header
    if (changeDoneToTheBlock)
    {
        remoteBlock.overwriteBlockHeader(header);
        m_session.test_rewindToBlock(_latestBlockNumber.getBlockNumberAsInt() - 1);
        m_session.test_importRawBlock(remoteBlock.getBlockRLP());
        // remoteBlock = m_session.eth_getBlockByHash(hash, true); // get the exact block info after
        // postmine operations
    }

    string const& sBlockException = _blockInTest.getException(m_network);
    bool blockIsValid = checkBlockException(sBlockException);
    remoteBlock.setValid(blockIsValid);
    return remoteBlock;  // malicious block must be written to the filled test
}

// Returns true if the block is valid
bool TestBlockchain::checkBlockException(string const& _sBlockException) const
{
    // Check malicious block import exception
    // Relies on that previous block import was exactly this block !!!
    if (_sBlockException == "NoException")
        ETH_ERROR_REQUIRE_MESSAGE(m_session.getLastRPCErrorMessage().empty(),
            "Postmine block tweak expected no exception! Client errors with: '" +
                m_session.getLastRPCErrorMessage() + "'");
    else
    {
        std::string const& clientExceptionString =
            Options::get().getDynamicOptions().getCurrentConfig().getExceptionString(
                _sBlockException);
        size_t pos = m_session.getLastRPCErrorMessage().find(clientExceptionString);
        if (clientExceptionString.empty())
            pos = string::npos;
        ETH_ERROR_REQUIRE_MESSAGE(pos != string::npos,
            "'" + clientExceptionString + "' (" + _sBlockException +
                ") not found in client response to postmine block tweak! Import result of postmine "
                "block: '" +
                m_session.getLastRPCErrorMessage() + "', Test Expected: '" + clientExceptionString +
                "'");
        return false;  // block is not valid
    }
    return true;  // block is valid
}
