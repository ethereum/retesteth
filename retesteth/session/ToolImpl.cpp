#include <chrono>
#include <cstdio>
#include <thread>

#include <dataObject/ConvertFile.h>
#include <retesteth/Options.h>
#include <retesteth/session/ToolImpl.h>
#include <retesteth/testStructures/types/BlockchainTests/Filler/BlockchainTestFillerEnv.h>

using namespace test;
using namespace toolimpl;

DataObject ToolImpl::web3_clientVersion()
{
    rpcCall("", {});
    ETH_FAIL_MESSAGE("Request: web3_clientVersion");
    return DataObject();
}

// ETH Methods
FH32 ToolImpl::eth_sendRawTransaction(BYTES const& _rlp)
{
    rpcCall("", {});
    ETH_TEST_MESSAGE("Request: eth_sendRawTransaction \n" + _rlp.asString());
    Transaction tr(_rlp);
    m_toolChainManager.getContent().addPendingTransaction(tr);
    ETH_TEST_MESSAGE("Response: " + tr.hash().asString());
    return tr.hash();
}

int ToolImpl::eth_getTransactionCount(FH20 const& _address, VALUE const& _blockNumber)
{
    rpcCall("", {});
    ETH_TEST_MESSAGE("Request: eth_getTransactionCount b:" + _blockNumber.asString() + " a:" + _address.asString());
    /*
    ToolBlock const& block = getBlockByHashOrNumber(_blockNumber);
    if (block.getPostState().count(_address) && block.getPostState().atKey(_address).count("nonce"))
    {
        string const& nonce = block.getPostState().atKey(_address).atKey("nonce").asString();
        ETH_TEST_MESSAGE("Response: " + nonce);
        return test::hexOrDecStringToInt(nonce);
    }
    */
    ETH_TEST_MESSAGE("0");
    return 0;
}

VALUE ToolImpl::eth_blockNumber()
{
    rpcCall("", {});
    ETH_TEST_MESSAGE("Request: eth_blockNumber");
    /*if (getCurrChain().size() > 0)
        blNumber = dev::toCompactHexPrefixed(getCurrChain().size(), 1);
        */
    // ETH_TEST_MESSAGE("Response: eth_blockNumber {" + blNumber + "}");
    return VALUE(0);
}


EthGetBlockBy ToolImpl::eth_getBlockByHash(FH32 const& _hash, Request _fullObjects)
{
    rpcCall("", {});
    ETH_TEST_MESSAGE("Request: eth_getBlockByHash `" + _hash.asString() + "`");
    (void)_fullObjects;  // always full objects
    /*
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

    ETH_TEST_MESSAGE("Response: eth_getBlockByHash (hash not found: " + _hash + ")");*/
    return EthGetBlockBy(DataObject());
}

EthGetBlockBy ToolImpl::eth_getBlockByNumber(VALUE const& _blockNumber, Request _fullObjects)
{
    rpcCall("", {});
    (void)_fullObjects;
    ETH_TEST_MESSAGE("Request: eth_getBlockByNumber " + _blockNumber.asDecString());
    /*
    int bNumber = _blockNumber.getBlockNumberAsInt();
    if (bNumber > 0 && (size_t)bNumber <= getCurrChain().size())
    {
        ETH_TEST_MESSAGE("Response: eth_getBlockByNumber " +
                         getCurrChain().at(bNumber - 1).getRPCResponse().getData().asJson());
        return getCurrChain().at(bNumber - 1).getRPCResponse();
    }
    else if (bNumber == 0)
        return getGenesis().getRPCResponse();
    return scheme_RPCBlock(DataObject());*/
    return EthGetBlockBy(DataObject());
}

BYTES ToolImpl::eth_getCode(FH20 const& _address, VALUE const& _blockNumber)
{
    rpcCall("", {});
    ETH_TEST_MESSAGE("Request: eth_getCode " + _blockNumber.asString() + " " + _address.asString());

    /*
    ToolBlock const& block = getBlockByHashOrNumber(_blockNumber);
    if (block.getPostState().count(_address) && block.getPostState().atKey(_address).count("code"))
    {
        string const& code = block.getPostState().atKey(_address).atKey("code").asString();
        ETH_TEST_MESSAGE(code);
        return code;
    }
    */
    ETH_TEST_MESSAGE("0x");
    return BYTES(DataObject("0x"));
}

VALUE ToolImpl::eth_getBalance(FH20 const& _address, VALUE const& _blockNumber)
{
    rpcCall("", {});
    ETH_TEST_MESSAGE("Request: eth_getBalance " + _blockNumber.asString() + " " + _address.asString());
    /*
        ToolBlock const& block = getBlockByHashOrNumber(_blockNumber);
        if (block.getPostState().count(_address) &&
            block.getPostState().atKey(_address).count("balance"))
        {
            string const& balance = block.getPostState().atKey(_address).atKey("balance").asString();
            ETH_TEST_MESSAGE(balance);
            return balance;
        }
        */
    ETH_TEST_MESSAGE("0x");
    return VALUE(DataObject("0x"));
}

// Debug
DebugAccountRange ToolImpl::debug_accountRange(
    VALUE const& _blockNumber, VALUE const& _txIndex, FH32 const& _address, int _maxResults)
{
    rpcCall("", {});
    ETH_TEST_MESSAGE("Request: debug_accountRange " + _blockNumber.asString());

    (void)_txIndex;
    (void)_address;
    (void)_maxResults;
    /*
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
        return constructResponse;*/
    return DebugAccountRange(DataObject());
}

DebugAccountRange ToolImpl::debug_accountRange(
    FH32 const& _blockHash, VALUE const& _txIndex, FH32 const& _address, int _maxResults)
{
    (void)_blockHash;
    (void)_txIndex;
    (void)_address;
    (void)_maxResults;
    return DebugAccountRange(DataObject());
}

DebugStorageRangeAt ToolImpl::debug_storageRangeAt(
    VALUE const& _blockNumber, VALUE const& _txIndex, FH20 const& _address, FH32 const& _begin, int _maxResults)
{
    rpcCall("", {});
    ETH_TEST_MESSAGE("Request: debug_storageRangeAt bl:" + _blockNumber.asDecString() + " ind:" + _begin.asString() +
                     " addr:" + _address.asString());
    (void)_txIndex;
    (void)_maxResults;

    /*
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
    */
    return DebugStorageRangeAt(DataObject());
}

DebugStorageRangeAt ToolImpl::debug_storageRangeAt(
    FH32 const& _blockHash, VALUE const& _txIndex, FH20 const& _address, FH32 const& _begin, int _maxResults)
{
    (void)_blockHash;
    (void)_txIndex;
    (void)_address;
    (void)_begin;
    (void)_maxResults;
    return DebugStorageRangeAt(DataObject());
}

DebugTraceTransaction ToolImpl::debug_traceTransaction(FH32 const& _trHash)
{
    rpcCall("", {});
    (void)_trHash;
    ETH_FAIL_MESSAGE("Request: debug_traceTransaction");
    return DebugTraceTransaction(DataObject());
}

// Test
void ToolImpl::test_setChainParams(SetChainParamsArgs const& _config)
{
    rpcCall("", {});
    ETH_TEST_MESSAGE("Request: test_setChainParams \n" + _config.asDataObject().asJson());

    // Ask tool to calculate genesis header stateRoot for genesisHeader
    EthereumBlock genesis(BlockHeader(_config.genesis()));
    m_toolChainManager = GCP_SPointer<ToolChainManager>(new ToolChainManager(_config, m_toolPath));

    ETH_TEST_MESSAGE("Response test_setChainParams: {true}");
}

void ToolImpl::test_rewindToBlock(VALUE const& _blockNr)
{
    rpcCall("", {});
    ETH_TEST_MESSAGE("Request: test_rewindToBlock");
    (void)_blockNr;
    /*
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
    */
}

void ToolImpl::test_modifyTimestamp(VALUE const& _timestamp)
{(void)_timestamp;
    rpcCall("", {});
    //ETH_TEST_MESSAGE("Request: test_modifyTimestamp " + DataObject(_timestamp).asJson());
    //m_currentBlockHeader.timestamp = _timestamp;
}

/*
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

        // Tool Require no leading zeros in transaction.
        string const to = txToolFormat.atKey("to").asString();
        string const data = txToolFormat.atKey("data").asString();
        txToolFormat.performModifier(test::mod_removeLeadingZerosFromHexValues);
        txToolFormat["to"] = to;
        txToolFormat["data"] = data;

        txToolFormat.renameKey("data", "input");
        txToolFormat.renameKey("gasLimit", "gas");

        string const& value = txToolFormat.atKey("value").asString();
        if (value == "0x" || value.empty())
            txToolFormat.atKeyUnsafe("value") = "0x0";
        string const& toAddr = txToolFormat.atKey("to").asString();
        if (toAddr.empty() || toAddr == "0x")
            txToolFormat.removeKey("to");

        txToolFormat["hash"] = tx.getHash();
        txs.addArrayObject(txToolFormat);
    }
    return txs.asJson();
}


*/

void ToolImpl::test_mineBlocks(int _number)
{
    (void)_number;
    rpcCall("", {});
    ETH_TEST_MESSAGE("Request: test_mineBlocks");
    /*
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

    // If calculating geneis block, disable rewards so to see the state root hash

    string const engine = m_chainParams.atKey("sealEngine").asString();
    if (!m_currentBlockHeader.isMiningGenesis && engine != "NoReward")
    {
        if (engine == "Ethash")
            ETH_WARNING_TEST("t8ntool backend treat Ethash as NoProof!", 6);

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
                string const reward = rewards.atKey(RewardMapForToolBefore5.at(fork)).asString();
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
    ETH_TEST_MESSAGE(cmd);

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
    */
}

// Import block from RAW rlp and validate it according to ethereum rules
// Very logic heavy function. Must be on the client side. Its a clien logic.
FH32 ToolImpl::test_importRawBlock(BYTES const& _blockRLP)
{
    rpcCall("", {});
    ETH_TEST_MESSAGE("Request: test_importRawBlock, following transaction import are internal");
    (void)_blockRLP;

    /*
    static vector<string> exceptions = {"BlockWrongStoreClears"};
    for (auto const& el : exceptions)
    {
        if (TestOutputHelper::get().testName().find(el) != string::npos)
        {
            ETH_WARNING("t8ntool backend can't verify this block!");
            onError("TooImpl can't verify this block!", "Test exception");
            return string();
        }
    }

    string rawBlockHash;
    try
    {
        bytes blockBytes = dev::fromHex(_blockRLP);
        dev::RLP blockRLP(blockBytes);
        ostringstream out;
        out << blockRLP;
        ETH_TEST_MESSAGE(out.str());
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
                string("RAW import hash != test_mineBlocks imitating rawblock. ") +
                "RAW: " + rawBlockHash + " vs " + cbl.getHash());
    }
    catch (test::BaseEthException const& _ex)
    {
        // do not treat this exception as retesteth error
        TestOutputHelper::get().unmarkLastError();
        onError(_ex.what(), "Import raw block failed");
        ETH_TEST_MESSAGE("Response test_importRawBlock: " + rawBlockHash);
        return string();
    }
    catch (RLPException const& _ex)
    {
        onError(_ex.what(), "Import raw block failed");
        ETH_TEST_MESSAGE("Response test_importRawBlock: " + rawBlockHash);
        return string();
    }

    doChainReorg();  // update head
    ETH_TEST_MESSAGE("Response test_importRawBlock: " + rawBlockHash);
    return rawBlockHash;
    */
    return FH32::zero();
}

FH32 ToolImpl::test_getLogHash(FH32 const& _txHash)
{
    rpcCall("", {});
    ETH_TEST_MESSAGE("Request: test_getLogHash " + _txHash.asString());
    /*
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
    */
    return FH32::zero();
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
    return m_toolPath.string();
}
