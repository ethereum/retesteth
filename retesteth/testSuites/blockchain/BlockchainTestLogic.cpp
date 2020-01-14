#include "BlockchainTestLogic.h"
#include <retesteth/EthChecks.h>
#include <retesteth/RPCSession.h>
#include <retesteth/testSuites/Common.h>

struct ChainNameBlocks;
test::scheme_block postmineBlockHeader(RPCSession& _session,
    scheme_blockchainTestFiller::blockSection const& _block, BlockNumber const& _latestBlockNumber,
    string const& _network, std::vector<scheme_block> const& _uncles);

typedef std::vector<test::scheme_block> vectorOfSchemeBlock;
test::scheme_block prepareUncle(RPCSession& _session, scheme_uncleHeader _uncleOverwrite,
    ChainNameBlocks const& _chainInformation,
    vectorOfSchemeBlock const& _currentBlockPreparedUncles,
    std::map<std::string, ChainNameBlocks> const& _allBlocksMap);

struct ChainNameBlocks
{
    // Remember new block
    void registerBlockRLP(std::string const& _blockRLP) { m_blockRlps.push_back(_blockRLP); }
    // Restore this chain on remote client
    void restoreOnRemoteClient(RPCSession& _session, size_t number = 0)
    {
        _session.test_rewindToBlock(0);  // Rewind to the begining
        // import all blocks for the new test mining
        size_t actualNumber = 1;
        for (auto const& rlp : m_blockRlps)
        {
            if (actualNumber < number)
                _session.test_importRawBlock(rlp);
            else
                break;
        }

        // Restore blocks up to `number` forgetting the rest of history
        if (number != 0)
        {
            for (int i = 0; i <= (int)(m_blockRlps.size() - number); i++)
                m_blockRlps.pop_back();
        }
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
    std::vector<std::string> m_blockRlps;  // List of Imported block RLPs
    vectorOfSchemeBlock m_uncles;          // unprepared uncles mined in paralel from the blocks
    std::map<size_t, vectorOfSchemeBlock> m_mapBlocksToPreparedUncles;  // prepared uncles for each
                                                                        // block
};

namespace test
{
/// Generate blockchain test from filler
void FillTest(scheme_blockchainTestFiller const& _testObject, string const& _network,
    TestSuite::TestSuiteOptions const&, DataObject& _testOut)
{
    // construct filled blockchain test
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

    size_t number = 0;
    std::map<std::string, ChainNameBlocks> mapOfKnownChain;  // Memory of test chains
    std::string s_currentChainName =
        "default";  // Current chain name that is mining on remote client
    mapOfKnownChain.emplace(s_currentChainName, ChainNameBlocks());
    for (auto const& block : _testObject.getBlocks())
    {
        ETH_LOGC("STARTING A NEW BLOCK: ", 6, LogColor::LIME);
        string const sChainName = block.getChainName().empty() ? "default" : block.getChainName();
        {
            // restore chain on remote client
            if (!mapOfKnownChain.count(sChainName))
                mapOfKnownChain.emplace(sChainName, ChainNameBlocks());

            // if we switch the chain or have to remine one of blocknumbers
            bool remineOneOfThePreviousBlocksOnTheSameChain =
                (block.getNumber() != 0 &&
                    mapOfKnownChain.at(sChainName).getBlockRlps().size() >= block.getNumber());
            if (s_currentChainName != sChainName || remineOneOfThePreviousBlocksOnTheSameChain)
            {
                ETH_LOGC("PERFORM REWIND HISTORY: ", 6, LogColor::YELLOW);
                ChainNameBlocks& chain = mapOfKnownChain.at(sChainName);
                chain.restoreOnRemoteClient(session, block.getNumber());
                s_currentChainName = sChainName;
                number = chain.getBlockRlps().size();
            }
        }
        ChainNameBlocks& currentChainMining = mapOfKnownChain.at(s_currentChainName);
        session.test_modifyTimestamp(1000);  // Shift block timestamp relative to previous block

        string sBlockDebugInfo;
        {
            // Debug information
            string sBlockNumber;
            TestInfo errorInfo(_network, ++number, sChainName);
            if (Options::get().logVerbosity >= 6)
                sBlockNumber = toString(number);  // very heavy
            TestOutputHelper::get().setCurrentTestInfo(errorInfo);
            sBlockDebugInfo = "(bl: " + sBlockNumber + ", ch: " + sChainName + ")";
            ETH_LOGC("Generating a test block: " + sBlockDebugInfo, 6, LogColor::YELLOW);
        }

        string latestBlockNumber;
        {
            ETH_LOGC("Populate clean uncle from: " + sBlockDebugInfo, 6, LogColor::YELLOW);
            latestBlockNumber = session.test_mineBlocks(1);  // populate uncle block from genesis
            currentChainMining.getUnpreparedUnclesUnsafe().push_back(
                session.eth_getBlockByNumber(latestBlockNumber, false));
            session.test_rewindToBlock(number - 1);
            session.test_modifyTimestamp(1000);  // Shift block timestamp relative to previous block
            ETH_LOGC("Finish populate clean uncle from: " + sBlockDebugInfo, 6, LogColor::YELLOW);
        }

        ETH_LOGC("Import transactions: " + sBlockDebugInfo, 6, LogColor::YELLOW);
        DataObject blockSection;
        blockSection["transactions"] = DataObject(DataType::Array);
        for (auto const& tr : block.getTransactions())
        {
            session.eth_sendRawTransaction(tr.getSignedRLP());
            blockSection["transactions"].addArrayObject(tr.getDataForBCTest());
        }
        if (!block.getChainName().empty())
            blockSection["chainname"] = block.getChainName();
        if (block.getNumber() != 0)
            blockSection["blocknumber"] =
                block.getData().atKey("blocknumber");  // heavy conversion alert!

        ETH_LOGC("Prepare Uncles for the block: " + sBlockDebugInfo, 6, LogColor::YELLOW);
        vectorOfSchemeBlock preparedUncleBlocks;  // Prepared uncles for the current block
        for (auto const& uncle : block.getUncles())
        {
            // return block using uncle overwrite on uncles array
            scheme_block uncleBlock = prepareUncle(
                session, uncle, currentChainMining, preparedUncleBlocks, mapOfKnownChain);
            preparedUncleBlocks.push_back(uncleBlock);
            blockSection["uncleHeaders"].addArrayObject(uncleBlock.getBlockHeader());
        }
        currentChainMining.getMapBlocksToPreparedUnclesUnsafe().emplace(
            number, preparedUncleBlocks);

        ETH_LOGC("MINE BLOCK: " + sBlockDebugInfo, 6, LogColor::YELLOW);
        latestBlockNumber = session.test_mineBlocks(1);
        bool uncleHeaderCount =
            block.getData().count("uncleHeaders") ?
                block.getData().atKey("uncleHeaders").getSubObjects().size() > 0 :
                false;
        if (block.getData().count("blockHeader") || uncleHeaderCount)
        {
            ETH_LOG("Postmine blockheader: " + sBlockDebugInfo, 6);
            latestBlock = postmineBlockHeader(
                session, block, latestBlockNumber, _network, preparedUncleBlocks);
            if (!latestBlock.isValid())
            {
                blockSection.removeKey("transactions");
                blockSection.removeKey("uncleHeaders");
            }
        }
        else
        {
            // SOME TRANSACTIONS MIGHT BE EXPECTED TO FAIL
            latestBlock = session.eth_getBlockByNumber(latestBlockNumber, true);
        }

        ETH_ERROR_REQUIRE_MESSAGE(
            latestBlock.getTransactionCount() == block.getTransactions().size(),
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

/// Read and execute the test from the file
void RunTest(DataObject const& _testObject, TestSuite::TestSuiteOptions const& _opt)
{
    {
        TestInfo errorInfo("RunTestInit");
        TestOutputHelper::get().setCurrentTestInfo(errorInfo);
    }
    if (Options::get().logVerbosity > 1)
        std::cout << "Running " << TestOutputHelper::get().testName() << std::endl;
    scheme_blockchainTest inputTest(_testObject);
    RPCSession& session = RPCSession::instance(TestOutputHelper::getThreadID());

    // Info for genesis
    TestInfo errorInfo (inputTest.getNetwork(), 0);
    TestOutputHelper::get().setCurrentTestInfo(errorInfo);
    session.test_setChainParams(
        inputTest.getGenesisForRPC(inputTest.getNetwork(), inputTest.getEngine()).asJson());

    // for all blocks
    size_t blockNumber = 0;
    for (auto const& brlp : inputTest.getBlockRlps())
    {
        TestInfo errorInfo(inputTest.getNetwork(), blockNumber++);
        TestOutputHelper::get().setCurrentTestInfo(errorInfo);
        session.test_importRawBlock(brlp);
        if (session.getLastRPCError().type() != DataType::Null)
        {
            if (!_opt.allowInvalidBlocks)
                ETH_ERROR_MESSAGE("Running blockchain test: " + session.getLastRPCError().atKey("message").asString());
        }
    }

    // wait for blocks to process
    // std::this_thread::sleep_for(std::chrono::seconds(10));

    scheme_block latestBlock = session.eth_getBlockByNumber(session.eth_blockNumber(), false);
    if (inputTest.getPost().isHash())
        validatePostHash(session, inputTest.getPost().getHash(), latestBlock);
    else
        compareStates(scheme_expectState(inputTest.getPost().getData()), session, latestBlock);

    if (inputTest.getLastBlockHash() != latestBlock.getBlockHash())
        ETH_ERROR_MESSAGE("lastblockhash does not match! remote: " + latestBlock.getBlockHash() +
                          ", test: " + inputTest.getLastBlockHash());
}

DataObject DoTests(DataObject const& _input, TestSuite::TestSuiteOptions& _opt)
{
    checkDataObject(_input);
    checkAtLeastOneTest(_input);

    DataObject tests;
    // A blockchain test file contains many tests in one .json file
    for (auto const& i : _input.getSubObjects())
    {
        string const& testname = i.getKey();
        TestOutputHelper::get().setCurrentTestName(testname);

        if (_opt.doFilling)
        {
            {
                TestInfo errorInfo("TestFillerInit");
                TestOutputHelper::get().setCurrentTestInfo(errorInfo);
            }
            scheme_blockchainTestFiller testFiller(i);
            // Create a blockchain test for each network described in expect section
            for (auto& network : testFiller.getExpectSection().getAllNetworksFromExpectSection())
            {
                // select expect section corresponding to the network (if any)
                for (auto const& expect : testFiller.getExpectSection().getExpectSections())
                {
                    if (expect.getNetworks().count(network))
                    {
                        string const newtestname = testname + "_" + network;
                        TestOutputHelper::get().setCurrentTestName(newtestname);

                        DataObject testOutput;
                        FillTest(testFiller, network, _opt, testOutput);
                        if (testFiller.getData().count("_info"))
                            testOutput["_info"] = testFiller.getData().atKey("_info");
                        tests[newtestname] = testOutput;
                    }
                }
            }
        }
        else
        {
            // Select test by name if --singletest and --singlenet is set
            if (Options::get().singleTest)
            {
                if (!Options::get().singleSubTestName.empty() &&
                    testname != Options::get().singleSubTestName)
                    continue;
            }

            if (!Options::get().singleTestNet.empty())
            {
                if (i.count("network") &&
                    i.atKey("network").asString() != Options::get().singleTestNet)
                    continue;
            }

            RunTest(i, _opt);
        }
    }
    TestOutputHelper::get().registerTestRunSuccess();
    return tests;
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

// _session is RPC connection to the client
// _uncleOverwrite is read from the test filler
// _existingUncles are blocks mined normally from the client as if it were uncles
// _preparedUncles is a map blNumber -> uncleHeaders where blNumber are existing blocks
// return a blockHeader info for _existingUncle overwritten by _uncleOverwrite
test::scheme_block prepareUncle(RPCSession& _session, scheme_uncleHeader _uncleOverwrite,
    ChainNameBlocks const& _chainInformation,
    vectorOfSchemeBlock const& _currentBlockPreparedUncles,
    std::map<std::string, ChainNameBlocks> const& _allBlocksMap)
{
    size_t origIndex = 0;
    test::scheme_block const* tmpRefToSchemeBlock = NULL;
    scheme_uncleHeader::typeOfUncleSection typeOfSection = _uncleOverwrite.getTypeOfUncleSection();
    switch (typeOfSection)
    {
    case scheme_uncleHeader::typeOfUncleSection::SameAsPreviousBlockUncle:
    {
        size_t sameAsPreviuousBlockUncle = _uncleOverwrite.getSameAsPreviousBlockUncle();
        ETH_ERROR_REQUIRE_MESSAGE(
            _chainInformation.getMapBlocksToPreparedUncles().count(sameAsPreviuousBlockUncle),
            "Trying to copy uncle from unregistered block with sameAsPreviuousBlockUncle!");
        tmpRefToSchemeBlock =
            &_chainInformation.getMapBlocksToPreparedUncles().at(sameAsPreviuousBlockUncle).at(0);
        break;
    }
    case scheme_uncleHeader::typeOfUncleSection::PopulateFromBlock:
    {
        // _existingUncles (ind 0 - from genesis, ind 1 - form first block)
        origIndex = _uncleOverwrite.getPopulateFrom();
        if (!_uncleOverwrite.getChainName().empty())
        {
            ETH_ERROR_REQUIRE_MESSAGE(_allBlocksMap.count(_uncleOverwrite.getChainName()),
                "Uncle is populating from non-existent chain!");
            ChainNameBlocks const& chain = _allBlocksMap.at(_uncleOverwrite.getChainName());
            ETH_ERROR_REQUIRE_MESSAGE(chain.getUnpreparedUncles().size() > origIndex,
                "Trying to populate uncle from future block in another chain that has not been "
                "generated yet!");
            tmpRefToSchemeBlock = &chain.getUnpreparedUncles().at(origIndex);
        }
        else
        {
            ETH_ERROR_REQUIRE_MESSAGE(_chainInformation.getUnpreparedUncles().size() > origIndex,
                "Trying to populate uncle from future block that has not been generated yet!");
            tmpRefToSchemeBlock = &_chainInformation.getUnpreparedUncles().at(origIndex);
        }
        break;
    }
    case scheme_uncleHeader::typeOfUncleSection::SameAsBlock:
    {
        BlockNumber sameAsBlockNumber(_uncleOverwrite.getSameAsBlock());
        return _session.eth_getBlockByNumber(sameAsBlockNumber, false);
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
    string const& overwriteField = _uncleOverwrite.getOverwrite();
    if (!overwriteField.empty())
        headerOrig.atKeyUnsafe(overwriteField) =
            _uncleOverwrite.getData().atKey(overwriteField).asString();

    // If uncle timestamp is shifted relative to the block that it's populated from
    string const& shift = _uncleOverwrite.getRelTimestampFromPopulateBlock();
    if (!shift.empty())
    {
        BlockNumber populateFromBlockNumber(origIndex);
        test::scheme_block populateFromBlock =
            _session.eth_getBlockByNumber(populateFromBlockNumber, false);

        //  std::cerr << populateFromBlockNumber.getBlockNumberAsString() << " tmstmp: " <<
        //  populateFromBlock.getBlockHeader().atKey("timestamp").asString() << std::endl;
        BlockNumber timestamp(populateFromBlock.getBlockHeader().atKey("timestamp").asString());
        timestamp.applyShift(shift);
        //  std::cerr << headerOrig.asJson() << std::endl;
        headerOrig.atKeyUnsafe("timestamp") = timestamp.getBlockNumberAsString();
        //  std::cerr << headerOrig.asJson() << std::endl;
    }

    uncleBlock.overwriteBlockHeader(headerOrig);
    return uncleBlock;
}
