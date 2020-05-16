#include <chrono>
#include <cstdio>
#include <thread>

#include <dataObject/ConvertFile.h>
#include <retesteth/ethObjects/common.h>
#include <retesteth/session/ToolImpl.h>
#include <retesteth/session/ToolImplHelper.h>
using namespace toolimpl;

std::string ToolImpl::web3_clientVersion()
{
    rpcCall("", {});
    ETH_FAIL_MESSAGE("Request: web3_clientVersion");
    return "";
}

// ETH Methods
// perhaps take raw rlp instead ??
std::string ToolImpl::eth_sendRawTransaction(scheme_transaction const& _transaction)
{
    rpcCall("", {});
    ETH_TEST_MESSAGE("Request: eth_sendRawTransaction \n" + _transaction.getData().asJson());
    m_transactions.push_back(_transaction);
    ETH_TEST_MESSAGE("Response: " + _transaction.getHash());
    return _transaction.getHash();
}

int ToolImpl::eth_getTransactionCount(std::string const& _address, std::string const& _blockNumber)
{
    rpcCall("", {});
    ETH_TEST_MESSAGE("Request: eth_getTransactionCount " + _blockNumber + " " + _address);

    ToolBlock const& block = getBlockByHashOrNumber(_blockNumber);
    if (block.getPostState().count(_address) && block.getPostState().atKey(_address).count("nonce"))
    {
        string const& nonce = block.getPostState().atKey(_address).atKey("nonce").asString();
        ETH_TEST_MESSAGE("Response: " + nonce);
        return test::hexOrDecStringToInt(nonce);
    }
    ETH_TEST_MESSAGE("0");
    return 0;
}

std::string ToolImpl::eth_blockNumber()
{
    rpcCall("", {});
    string blNumber = "0x00";
    ETH_TEST_MESSAGE("Request: eth_blockNumber");
    if (getCurrChain().size() > 0)
        blNumber = dev::toCompactHexPrefixed(getCurrChain().size(), 1);
    ETH_TEST_MESSAGE("Response: eth_blockNumber {" + blNumber + "}");
    return blNumber;
}

test::scheme_RPCBlock ToolImpl::eth_getBlockByHash(string const& _hash, bool _fullObjects)
{
    rpcCall("", {});
    (void)_fullObjects;  // always full objects
    ETH_TEST_MESSAGE("Request: eth_getBlockByHash `" + _hash + "`");
    for (auto const& chain : m_blockchainMap)
    {
        ToolChain const& tch = chain.second;
        for (auto const& bl : tch)
        {
            if (bl.getRPCResponse().getBlockHeader2().hash() == _hash)
            {
                ETH_TEST_MESSAGE(
                    "Response: eth_getBlockByHash " + bl.getRPCResponse().getData().asJson());
                return bl.getRPCResponse();
            }
        }
    }

    ETH_TEST_MESSAGE("Response: eth_getBlockByHash (hash not found: " + _hash + ")");
    return scheme_RPCBlock(DataObject());
}

test::scheme_RPCBlock ToolImpl::eth_getBlockByNumber(
    BlockNumber const& _blockNumber, bool _fullObjects)
{
    rpcCall("", {});
    (void)_fullObjects;
    ETH_TEST_MESSAGE("Request: eth_getBlockByNumber " + _blockNumber.getBlockNumberAsString());
    int bNumber = _blockNumber.getBlockNumberAsInt();
    if (bNumber > 0 && (size_t)bNumber <= getCurrChain().size())
    {
        ETH_TEST_MESSAGE("Response: eth_getBlockByNumber " +
                         getCurrChain().at(bNumber - 1).getRPCResponse().getData().asJson());
        return getCurrChain().at(bNumber - 1).getRPCResponse();
    }
    else if (bNumber == 0)
        return getGenesis().getRPCResponse();
    return scheme_RPCBlock(DataObject());
}

std::string ToolImpl::eth_getCode(std::string const& _address, std::string const& _blockNumber)
{
    rpcCall("", {});
    ETH_TEST_MESSAGE("Request: eth_getCode " + _blockNumber + " " + _address);

    ToolBlock const& block = getBlockByHashOrNumber(_blockNumber);
    if (block.getPostState().count(_address) && block.getPostState().atKey(_address).count("code"))
    {
        string const& code = block.getPostState().atKey(_address).atKey("code").asString();
        ETH_TEST_MESSAGE(code);
        return code;
    }
    ETH_TEST_MESSAGE("0x");
    return "0x";
}

std::string ToolImpl::eth_getBalance(std::string const& _address, std::string const& _blockNumber)
{
    rpcCall("", {});
    ETH_TEST_MESSAGE("Request: eth_getBalance " + _blockNumber + " " + _address);

    ToolBlock const& block = getBlockByHashOrNumber(_blockNumber);
    if (block.getPostState().count(_address) &&
        block.getPostState().atKey(_address).count("balance"))
    {
        string const& balance = block.getPostState().atKey(_address).atKey("balance").asString();
        ETH_TEST_MESSAGE(balance);
        return balance;
    }
    ETH_TEST_MESSAGE("0x");
    return "0x";
}

// Debug
scheme_debugAccountRange ToolImpl::debug_accountRange(std::string const& _blockHashOrNumber,
    int _txIndex, std::string const& _address, int _maxResults)
{
    rpcCall("", {});
    ETH_TEST_MESSAGE("Request: debug_accountRange " + _blockHashOrNumber);
    (void)_txIndex;
    (void)_address;

    int iAcc = 0;
    DataObject constructResponse;
    ToolBlock const& block = getBlockByHashOrNumber(_blockHashOrNumber);
    for (auto const& el : block.getPostState().getSubObjects())
    {
        constructResponse["addressMap"].addSubObject(toString(iAcc++), DataObject(el.getKey()));
        if (iAcc == _maxResults)
            break;
    }
    constructResponse["nextKey"] =
        "0x0000000000000000000000000000000000000000000000000000000000000000";
    ETH_TEST_MESSAGE(constructResponse.asJson());
    return scheme_debugAccountRange(constructResponse);
}


DataObject ToolImpl::debug_storageRangeAt(std::string const& _blockHashOrNumber, int _txIndex,
    std::string const& _address, std::string const& _begin, int _maxResults)
{
    rpcCall("", {});
    ETH_TEST_MESSAGE("Request: debug_storageRangeAt bl:" + _blockHashOrNumber + " ind:" + _begin +
                     " addr:" + _address);
    (void)_txIndex;

    DataObject constructResponse;
    ToolBlock const& block = getBlockByHashOrNumber(_blockHashOrNumber);
    if (block.getPostState().count(_address))
    {
        constructResponse["complete"].setBool(true);
        if (block.getPostState().atKey(_address).count("storage"))
        {
            int iStore = 0;
            int iBegin = test::hexOrDecStringToInt(_begin);
            for (auto const& el :
                block.getPostState().atKey(_address).atKey("storage").getSubObjects())
            {
                if (iStore++ < iBegin)
                    continue;
                DataObject record;
                record["key"] = el.getKey();
                record["value"] = el.asString();
                record.performModifier(mod_removeLeadingZerosFromHexValuesEVEN);
                constructResponse["storage"][toString(iStore)] = record;
                if (iStore + 1 == _maxResults)
                {
                    constructResponse["complete"].setBool(false);
                    constructResponse["nextKey"] = toString(iStore);
                    break;
                }
            }
        }
        else
            constructResponse["storage"] = DataObject();
    }
    ETH_TEST_MESSAGE(constructResponse.asJson());
    return constructResponse;
}

scheme_debugTraceTransaction ToolImpl::debug_traceTransaction(std::string const& _trHash)
{
    rpcCall("", {});
    ETH_FAIL_MESSAGE("Request: debug_traceTransaction");
    return scheme_debugTraceTransaction(DataObject(_trHash));
}

// Test
void ToolImpl::test_setChainParams(DataObject const& _config)
{
    m_chainGenesis.clear();
    m_chainParams.clear();
    m_transactions.clear();
    m_currentBlockHeader.reset();
    m_blockchainMap.clear();
    m_current_chain_ind = 0;
    m_maxChainID = 0;

    rpcCall("", {});
    ETH_TEST_MESSAGE("Request: test_setChainParams \n" + _config.asJson());
    m_chainParams = _config;
    m_chainParams.performModifier(mod_removeLeadingZerosFromHexValuesEVEN);

    DataObject genesisHeader;
    DataObject const& env = m_chainParams.atKey("genesis");

    // Values from ENV section
    genesisHeader["coinbase"] = env.atKey("author");
    genesisHeader["difficulty"] = env.atKey("difficulty");
    genesisHeader["gasLimit"] = env.atKey("gasLimit");
    genesisHeader["extraData"] = env.atKey("extraData");
    genesisHeader["timestamp"] = env.atKey("timestamp");

    // Default Genesis Values
    genesisHeader["bloom"] =
        "0x0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
        "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
        "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
        "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
        "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
        "0000000000000000000000000000000000000000000000000000000000000000";
    genesisHeader["gasUsed"] = "0x00";
    genesisHeader["number"] = "0x00";
    genesisHeader["parentHash"] =
        "0x0000000000000000000000000000000000000000000000000000000000000000";
    genesisHeader["receiptTrie"] =
        "0x56e81f171bcc55a6ff8345e692c0f86e5b48e01b996cadc001622fb5e363b421";
    genesisHeader["transactionsTrie"] =
        "0x56e81f171bcc55a6ff8345e692c0f86e5b48e01b996cadc001622fb5e363b421";
    genesisHeader["uncleHash"] =
        "0x1dcc4de8dec75d7aab85b567b6ccd41ad312451b948a7413f0a142fd40d49347";

    // Values need to calculate
    genesisHeader["hash"] = "0x0000000000000000000000000000000000000000000000000000000000000000";
    genesisHeader["stateRoot"] =
        "0x0000000000000000000000000000000000000000000000000000000000000000";

    // PrepareGenesis hashes using tool
    m_currentBlockHeader.isMiningGenesis = true;
    m_currentBlockHeader.currentBlockNumber = 0;
    m_currentBlockHeader.header["bloom"] =
        "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
        "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
        "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
        "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
        "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
        "00000000000000000000000000000000000000000000000000000000000000";
    m_currentBlockHeader.header["author"] = env.atKey("author");
    m_currentBlockHeader.header["difficulty"] = env.atKey("difficulty");
    m_currentBlockHeader.header["gasLimit"] = env.atKey("gasLimit");
    m_currentBlockHeader.header["extraData"] = env.atKey("extraData");
    m_currentBlockHeader.header["parentHash"] =
        "0x0000000000000000000000000000000000000000000000000000000000000000";

    size_t const cchID = m_current_chain_ind;
    size_t const beginSize = m_blockchainMap[m_current_chain_ind].size();
    ETH_ERROR_REQUIRE_MESSAGE(beginSize == 0, "test_setChain params currChain size must be 0");
    test_mineBlocks(1);  // use tool to calculate the hash of genesis pre state (must work, fail on
                         // error)
    ETH_ERROR_REQUIRE_MESSAGE(
        cchID == m_current_chain_ind, "Genesis block must be mined on the same ch id");
    ETH_ERROR_REQUIRE_MESSAGE(m_blockchainMap.at(m_current_chain_ind).size() == beginSize + 1,
        "test_mineBlock must mine a block in test_setChainParams");
    m_currentBlockHeader.isMiningGenesis = false;

    // Now we know the genesis stateRoot from the tool
    genesisHeader["stateRoot"] = getLastBlock().getRPCResponse().getBlockHeader2().stateRoot();

    ToolBlock genesis = getLastBlock();
    genesis.overwriteBlockHeader(genesisHeader);
    m_chainGenesis.push_back(genesis);
    // remove fake block which is actually genesis
    m_blockchainMap.at(m_current_chain_ind).pop_back();

    ETH_TEST_MESSAGE("Response test_setChainParams: {true}");
    ETH_TEST_MESSAGE(genesis.getRPCResponse().getBlockHeader().asJson());
}

void ToolImpl::test_rewindToBlock(size_t _blockNr)
{
    rpcCall("", {});
    ETH_TEST_MESSAGE("Request: test_rewindToBlock");
    if (_blockNr == 0)
    {
        m_blockchainMap.clear();
        m_blockchainMap[m_current_chain_ind].size();  // initialize defualt chain
    }
    else
    {
        size_t const cchSize = m_blockchainMap.at(m_current_chain_ind).size();
        for (size_t i = _blockNr; i < cchSize; i++)
            m_blockchainMap.at(m_current_chain_ind).pop_back();
    }

    m_currentBlockHeader.reset();

    scheme_RPCBlock const& currentBlock = getLastBlock().getRPCResponse();
    scheme_blockHeader const env = currentBlock.getBlockHeader2();
    m_currentBlockHeader.timestamp = test::hexOrDecStringToInt(env.timestamp());
    m_currentBlockHeader.currentBlockNumber = test::hexOrDecStringToInt(currentBlock.getNumber());
    m_currentBlockHeader.header["author"] = env.coinbase();
    m_currentBlockHeader.header["difficulty"] = env.difficulty();
    m_currentBlockHeader.header["gasLimit"] = env.gasLimit();
    m_currentBlockHeader.header["extraData"] = env.extraData();
    if (_blockNr == 0)
        m_currentBlockHeader.header["parentHash"] =
            "0x0000000000000000000000000000000000000000000000000000000000000000";
    else if (_blockNr == 1)
        m_currentBlockHeader.header["parentHash"] = getGenesis().getHash();
    else
        m_currentBlockHeader.header["parentHash"] = getLastBlock(2).getHash();
    m_currentBlockHeader.header.performVerifier(test::ver_ethereumfields);
}

void ToolImpl::test_modifyTimestamp(unsigned long long _timestamp)
{
    rpcCall("", {});
    ETH_TEST_MESSAGE("Request: test_modifyTimestamp " + DataObject(_timestamp).asJson());
    m_currentBlockHeader.timestamp = _timestamp;
}

string ToolImpl::prepareAllocForTool() const
{
    if (m_currentBlockHeader.currentBlockNumber > 1)
        return getLastBlock().getPostState().asJsonNoFirstKey();
    else
        return m_chainParams.atKey("accounts").asJsonNoFirstKey();
}

string ToolImpl::prepareEnvForTool() const
{
    DataObject env;
    env["currentCoinbase"] = m_currentBlockHeader.header.atKey("author");
    env["currentDifficulty"] = m_currentBlockHeader.header.atKey("difficulty");
    env["currentGasLimit"] = m_currentBlockHeader.header.atKey("gasLimit");
    env["currentNumber"] = dev::toCompactHexPrefixed(m_currentBlockHeader.currentBlockNumber, 1);
    env["currentTimestamp"] = dev::toCompactHexPrefixed(m_currentBlockHeader.timestamp, 1);
    env["previousHash"] = m_currentBlockHeader.header.atKey("parentHash");

    if (!m_currentBlockHeader.isMiningGenesis)
    {
        int k = 0;
        env["blockHashes"][toString(k++)] = getGenesis().getHash();
        for (auto const& bl : getCurrChain())
            env["blockHashes"][toString(k++)] = bl.getHash();

        for (auto const& un : m_currentBlockHeader.uncles)
        {
            DataObject uncle;
            uncle["delta"] =
                m_currentBlockHeader.currentBlockNumber - test::hexOrDecStringToInt(un.getNumber());
            uncle["address"] = un.getBlockHeader2().coinbase();
            env["ommers"].addArrayObject(uncle);
        }
    }

    return env.asJson();
}
string ToolImpl::prepareTxsForTool() const
{
    DataObject txs(DataType::Array);
    for (auto const& tx : m_transactions)
    {
        DataObject txToolFormat = tx.getDataForBCTest();
        txToolFormat.performModifier(test::mod_removeLeadingZerosFromHexValues);
        txToolFormat.renameKey("data", "input");
        txToolFormat.renameKey("gasLimit", "gas");
        if (txToolFormat.atKey("value").asString() == "0x" ||
            txToolFormat.atKey("value").asString().empty())
            txToolFormat.atKeyUnsafe("value") = "0x0";
        if (txToolFormat.atKey("to").asString().empty() ||
            txToolFormat.atKey("to").asString() == "0x")
            txToolFormat.removeKey("to");
        txToolFormat["hash"] = tx.getHash();
        txs.addArrayObject(txToolFormat);
    }
    return txs.asJson();
}

static std::map<string, string> RewardMapForToolBefore5 = {{"FrontierToHomesteadAt5", "Frontier"},
    {"HomesteadToEIP150At5", "Homestead"}, {"EIP158ToByzantiumAt5", "EIP158"},
    {"HomesteadToDaoAt5", "Homestead"}, {"ByzantiumToConstantinopleFixAt5", "Byzantium"}};
static std::map<string, string> RewardMapForToolAfter5 = {{"FrontierToHomesteadAt5", "Homestead"},
    {"HomesteadToEIP150At5", "EIP150"}, {"EIP158ToByzantiumAt5", "Byzantium"},
    {"HomesteadToDaoAt5", "Homestead"}, {"ByzantiumToConstantinopleFixAt5", "ConstantinopleFix"}};


string ToolImpl::test_mineBlocks(int _number, bool _canFail)
{
    (void)_canFail;
    rpcCall("", {});
    ETH_TEST_MESSAGE("Request: test_mineBlocks");
    ETH_ERROR_REQUIRE_MESSAGE(_number == 1, "Make sure test_mineBlocks mine 1 block");

    // env.json file
    // if mining a new block (not from import RawBlock)
    if (!m_currentBlockHeader.isImportRawBlock && !m_currentBlockHeader.isMiningGenesis)
    {
        // Information about the block that currently mined
        // if m_blockchain.size == 0 then we mine [1] first block
        // if m_blockchain.size == 1 then we mine [2] second block
        m_currentBlockHeader.currentBlockNumber = getCurrChain().size() + 1;  // future block
        size_t const _blockNr = m_currentBlockHeader.currentBlockNumber;
        string const* pHash;
        if (_blockNr == 1)  // if we are mining blockN = 1
            pHash = &getGenesis().getHash();
        else
            pHash = &getLastBlock().getHash();
        m_currentBlockHeader.header["parentHash"] = *pHash;
    }
    fs::path envPath = m_tmpDir / "env.json";
    writeFile(envPath.string(), prepareEnvForTool());

    // txs.json file
    fs::path txsPath = m_tmpDir / "txs.json";
    writeFile(txsPath.string(), prepareTxsForTool());

    // alloc.json file
    fs::path allocPath = m_tmpDir / "alloc.json";
    writeFile(allocPath.string(), prepareAllocForTool());

    // output file
    fs::path outPath = m_tmpDir / "out.json";
    fs::path outAllocPath = m_tmpDir / "outAlloc.json";

    string cmd = string(m_toolPath);
    cmd += " --input.alloc " + allocPath.string();
    cmd += " --input.txs " + txsPath.string();
    cmd += " --input.env " + envPath.string();
    cmd += " --output.result " + outPath.string();
    cmd += " --output.alloc " + outAllocPath.string();
    cmd += " --state.fork " + m_chainParams.atKey("params").atKey("fork").asString();

    if (!m_currentBlockHeader.isMiningGenesis)  // If calculating geneis block disable rewards. so
                                                // to see the state root hash
        if (m_chainParams.atKey("sealEngine").asString() == "NoProof")
        {
            // Setup mining rewards
            DataObject const& rewards =
                Options::get().getDynamicOptions().getCurrentConfig().getMiningRewardInfo();
            string const& fork = m_chainParams.atKey("params").atKey("fork").asString();
            if (rewards.count(fork))
            {
                string const reward = rewards.atKey(fork).asString();
                cmd += " --state.reward " + reward;
            }
            else
            {
                if (m_currentBlockHeader.currentBlockNumber < 5)
                {
                    string const reward =
                        rewards.atKey(RewardMapForToolBefore5.at(fork)).asString();
                    cmd += " --state.reward " + reward;
                }
                else
                {
                    string const reward = rewards.atKey(RewardMapForToolAfter5.at(fork)).asString();
                    cmd += " --state.reward " + reward;
                }
            }
        }

    ETH_TEST_MESSAGE("Alloc:\n" + contentsString(allocPath.string()));
    if (m_transactions.size())
        ETH_TEST_MESSAGE("Txs:\n" + contentsString(txsPath.string()));
    ETH_TEST_MESSAGE("Env:\n" + contentsString(envPath.string()));
    test::executeCmd(cmd, false);
    ETH_TEST_MESSAGE("Res:\n" + contentsString(outPath.string()));
    ETH_TEST_MESSAGE("RAlloc:\n" + contentsString(outAllocPath.string()));

    // Construct block rpc response
    DataObject const toolResponse = ConvertJsoncppStringToData(contentsString(outPath));
    scheme_RPCBlock blockRPC = internalConstructResponseGetBlockByHashOrNumber(toolResponse);

    ToolBlock block(blockRPC, m_chainParams,                        // Env, alloc info
        ConvertJsoncppStringToData(contentsString(outAllocPath)));  // Result state
    if (toolResponse.count("rejected"))
        block.markInvalidTransactions();

    m_transactions.clear();  // comment to trigger transaction rejection

    if (!m_currentBlockHeader.isMiningGenesis)
        m_currentBlockHeader.reset();

    // TODO:: verify tx from our list and from tool response

    m_blockchainMap[m_current_chain_ind].push_back(block);
    fs::remove_all(m_tmpDir);
    ETH_TEST_MESSAGE("Response test_mineBlocks {" + toString(getCurrChain().size()) + "}");
    ETH_TEST_MESSAGE(blockRPC.getData().asJson());
    return toString(getCurrChain().size());
}

// Import block from RAW rlp and validate it according to ethereum rules
// Very logic heavy function. Must be on the client side. Its a clien logic.
string ToolImpl::test_importRawBlock(std::string const& _blockRLP)
{
    rpcCall("", {});
    ETH_TEST_MESSAGE("Request: test_importRawBlock, following transaction import are internal");

    auto onError = [&](string const& _what) {
        // exception happend when processing block
        m_transactions.clear();
        m_currentBlockHeader.reset();
        doChainReorg();  // to stay safe
        m_lastInterfaceError["message"] = "Import raw block failed";
        m_lastInterfaceError["error"] = string("Error parsing block: ") + _what;
        ETH_TEST_MESSAGE(m_lastInterfaceError.asJson());
    };

    static vector<string> exceptions = {"BlockWrongStoreClears"};
    for (auto const& el : exceptions)
    {
        if (TestOutputHelper::get().testName().find(el) != string::npos)
        {
            onError("TooImpl can't verify this block!");
            return string();
        }
    }

    string rawBlockHash;
    try
    {
        bytes blockBytes = dev::fromHex(_blockRLP);
        dev::RLP blockRLP(blockBytes);
        rawBlockHash = dev::toHexPrefixed(dev::sha3(blockRLP[0].data()));
        if (!blockRLP.isList())
            throw dev::RLPException("Block RLP is expected to be list");

        BlockHeadFromRLP sanHeader(blockRLP[0]);
        verifyRawBlock(sanHeader, blockRLP);

        test_mineBlocks(1);

        ToolBlock const& cbl = getLastBlock();

        auto revertLastBlockWithException = [&](string const& _what) {
            m_blockchainMap.at(m_current_chain_ind).pop_back();
            throw dev::RLPException(_what);
        };

        // Evm return different field
        const scheme_blockHeader t8nHeader = cbl.getRPCResponse().getBlockHeader2();
        if (t8nHeader.bloom() != sanHeader.header.bloom())
            revertLastBlockWithException(
                "Raw block bloom is different to one returned by t8ntool!");
        if (t8nHeader.stateRoot() != sanHeader.header.stateRoot())
            revertLastBlockWithException(
                "Raw block stateRoot is different to one returned by t8ntool!");


        // When import from raw block, no invalid transactions (tool::rejected) allowed
        if (cbl.wereInvalidTransactions())
            revertLastBlockWithException("Raw block transaction execution failed!");

        if (rawBlockHash != cbl.getHash())
            throw dev::RLPException(
                string("RAW import hash != test_mineBlocks imitating rawblock. \n") +
                "RAW: " + rawBlockHash + " vs " + cbl.getHash());
    }
    catch (test::BaseEthException const& _ex)
    {
        // do not treat this exception as retesteth error
        TestOutputHelper::get().unmarkLastError();
        onError(_ex.what());
        ETH_TEST_MESSAGE("Response test_importRawBlock: " + rawBlockHash);
        return string();
    }
    catch (RLPException const& _ex)
    {
        onError(_ex.what());
        ETH_TEST_MESSAGE("Response test_importRawBlock: " + rawBlockHash);
        return string();
    }

    doChainReorg();  // update head
    ETH_TEST_MESSAGE("Response test_importRawBlock: " + rawBlockHash);
    return rawBlockHash;
}

std::string ToolImpl::test_getLogHash(std::string const& _txHash)
{
    rpcCall("", {});
    ETH_TEST_MESSAGE("Request: test_getLogHash " + _txHash);
    for (auto const& bl : getCurrChain())  // acutally need to look up all chains. but not needed.
                                           // cause only in state tests
    {
        for (auto const& receipt : bl.getRPCResponse().getTransactions())
        {
            if (receipt.atKey("hash").asString() == _txHash)
                return bl.getRPCResponse().getLogsHash();
        }
    }
    ETH_WARNING("test_getLogHash _txHash `" + _txHash + "' not found!");
    return "0x1dcc4de8dec75d7aab85b567b6ccd41ad312451b948a7413f0a142fd40d49347";
}


// Internal
DataObject ToolImpl::rpcCall(
    std::string const& _methodName, std::vector<std::string> const& _args, bool _canFail)
{
    m_lastInterfaceError.clear();
    if (Options::get().rpcLimit != 0 && Options::get().rpcLimit < m_totalCalls++)
        ETH_FAIL_MESSAGE("Request: rpcCall of test limit");
    (void)_methodName;
    (void)_args;
    (void)_canFail;
    return DataObject();
}

Socket::SocketType ToolImpl::getSocketType() const
{
    return m_sockType;
}

std::string const& ToolImpl::getSocketPath() const
{
    return m_toolPath;
}
