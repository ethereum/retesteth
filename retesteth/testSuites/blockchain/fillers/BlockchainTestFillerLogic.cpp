#include "BlockchainTestFillerLogic.h"
#include "TestBlockchainManager.h"

struct ChainNameBlocks;
test::scheme_block postmineBlockHeader(RPCSession& _session,
    scheme_blockchainTestFiller::blockSection const& _block, BlockNumber const& _latestBlockNumber,
    string const& _network, std::vector<scheme_block> const& _uncles);

test::scheme_block prepareUncle(RPCSession& _session, scheme_uncleHeader _uncleOverwrite,
    ChainNameBlocks const& _chainInformation,
    vectorOfSchemeBlock const& _currentBlockPreparedUncles,
    std::map<std::string, ChainNameBlocks> const& _allBlocksMap);


namespace test
{
/// Generate blockchain test from filler
void FillTest(scheme_blockchainTestFiller const& _testObject, string const& _network,
    TestSuite::TestSuiteOptions const&, DataObject& _testOut)
{
    // Construct filled blockchain test
    _testOut["sealEngine"] = _testObject.getSealEngine();
    _testOut["network"] = _network;
    _testOut["pre"] = _testObject.getPre().getData();
    if (_testObject.getData().count("_info"))
        _testOut["_info"] = _testObject.getData().atKey("_info");

    RPCSession& session = RPCSession::instance(TestOutputHelper::getThreadID());
    DataObject genesisObject = _testObject.getGenesisForRPC(_network, _testObject.getSealEngine());
    session.test_setChainParams(genesisObject.asJson());

    test::scheme_block latestBlock = session.eth_getBlockByNumber(BlockNumber("0"), false);
    _testOut["genesisBlockHeader"] = latestBlock.getBlockHeader();
    _testOut["genesisBlockHeader"].removeKey("transactions");
    _testOut["genesisBlockHeader"].removeKey("uncles");
    _testOut["genesisRLP"] = latestBlock.getBlockRLP();

    size_t number = 0;
    TestBlockchainManager testchain(session, _network);
    for (auto const& block : _testObject.getBlocks())
    {
        DataObject blockSection;
        blockSection["chainname"] = block.getChainName();
        if (block.getNumber() != 0)
            blockSection["blocknumber"] =
                block.getData().atKey("blocknumber");  // heavy conversion alert!

        ETH_LOGC("STARTING A NEW BLOCK: ", 6, LogColor::LIME);
        testchain.parseBlockFromJson(block);


        ETH_ERROR_REQUIRE_MESSAGE(
            latestBlock.getTransactionCount() ==
                block.getTransactions().size() - block.getInvalidTransactionCount(),
            "BlockchainTest transaction execution failed! (remote " +
                toString(latestBlock.getTransactionCount()) + " != test " +
                toString(block.getTransactions().size()) + ")");

        blockSection["rlp"] = latestBlock.getBlockRLP();
        if (latestBlock.isValid())
            blockSection["blockHeader"] = latestBlock.getBlockHeader();
        else
            blockSection["expectException"] = block.getException(_network);

        ChainNameBlocks& chain = mapOfKnownChain[s_currentChainName];
        chain.registerBlockRLP(blockSection["rlp"].asString());
        _testOut["blocks"].addArrayObject(blockSection);
        if (!latestBlock.isValid())
        {
            latestBlockNumber = session.eth_blockNumber();
            latestBlock = session.eth_getBlockByNumber(latestBlockNumber, false);
        }
    }

    // Import blocks that have been rewinded with the chain switch
    ETH_LOGC("IMPORT KNOWN BLOCKS ", 6, LogColor::LIME);
    for (auto const& chain : mapOfKnownChain)
        chain.second.importKnownBlocks(session);

    latestBlock = session.eth_getBlockByNumber(session.eth_blockNumber(), false);
    scheme_state remoteState = getRemoteState(session, latestBlock);
    if (remoteState.isHash())
        compareStates(_testObject.getExpectSection().getExpectSectionFor(_network).getExpectState(),
            session, latestBlock);
    else
        compareStates(_testObject.getExpectSection().getExpectSectionFor(_network).getExpectState(),
            remoteState);

    if (remoteState.isHash())
        _testOut["postStateHash"] = remoteState.getData();
    else
        _testOut["postState"] = remoteState.getData();
    _testOut["lastblockhash"] = latestBlock.getBlockHash();
}

}  // namespace test


test::scheme_block postmineBlockHeader(RPCSession& _session,
    scheme_blockchainTestFiller::blockSection const& _block, BlockNumber const& _latestBlockNumber,
    string const& _network, std::vector<scheme_block> const& _uncles)
{
    // if blockHeader is defined in test Filler, rewrite the last block header fields with info from
    // test and reimport it to the client in order to trigger an exception in the client
    test::scheme_block remoteBlock = _session.eth_getBlockByNumber(_latestBlockNumber, true);
    DataObject header = remoteBlock.getBlockHeader();

    // Overwrite blockheader if defined in the test filler
    if (_block.getData().count("blockHeader"))
    {
        for (auto const& replace : _block.getData().atKey("blockHeader").getSubObjects())
        {
            if (replace.getKey() == "updatePoW" || replace.getKey() == "expectException")
                continue;
            if (replace.getKey() == "RelTimestamp")
            {
                BlockNumber previousBlockNumber(_latestBlockNumber);
                previousBlockNumber.applyShift(-1);

                test::scheme_block previousBlock =
                    _session.eth_getBlockByNumber(previousBlockNumber, false);
                string previousBlockTimestampString =
                    previousBlock.getBlockHeader().atKey("timestamp").asString();
                BlockNumber previousBlockTimestamp(previousBlockTimestampString);
                previousBlockTimestamp.applyShift(replace.asString());
                header["timestamp"] = previousBlockTimestamp.getBlockNumberAsString();
                continue;
            }
            if (header.count(replace.getKey()))
                header[replace.getKey()] = replace.asString();
            else
                ETH_STDERROR_MESSAGE(
                    "blockHeader field in test filler tries to overwrite field that is not found "
                    "in "
                    "blockheader: '" +
                    replace.getKey() + "'");
        }
    }

    // Attach test-generated uncle to a block and reimport it again
    if (_block.getData().count("uncleHeaders"))
    {
        for (auto const& bl : _uncles)
            remoteBlock.addUncle(bl);
    }

    // replace block with overwritten header
    remoteBlock.overwriteBlockHeader(header);
    _session.test_rewindToBlock(_latestBlockNumber.getBlockNumberAsInt() - 1);
    _session.test_importRawBlock(remoteBlock.getBlockRLP());

    // check malicious block import exception
    if (_block.getException(_network) == "NoException")
        ETH_ERROR_REQUIRE_MESSAGE(_session.getLastRPCErrorMessage().empty(),
            "Postmine block tweak expected no exception! Client errors with: '" +
                _session.getLastRPCErrorMessage() + "'");
    else
    {
        std::string const& clientExceptionString =
            Options::get().getDynamicOptions().getCurrentConfig().getExceptionString(
                _block.getException(_network));
        size_t pos = _session.getLastRPCErrorMessage().find(clientExceptionString);
        if (clientExceptionString.empty())
            pos = string::npos;
        ETH_ERROR_REQUIRE_MESSAGE(pos != string::npos,
            "'" + clientExceptionString + "' (" + _block.getException(_network) +
                ") not found in client response to postmine block tweak! Import result of postmine "
                "block: '" +
                _session.getLastRPCErrorMessage() + "', Test Expected: '" + clientExceptionString +
                "'");
        remoteBlock.setValid(false);
    }
    return remoteBlock;  // malicious block must be written to the filled test
}
