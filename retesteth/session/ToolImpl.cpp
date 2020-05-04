#include <chrono>
#include <cstdio>
#include <thread>

#include <dataObject/ConvertFile.h>
#include <retesteth/ethObjects/common.h>
#include <retesteth/session/ToolImpl.h>


std::string ToolImpl::web3_clientVersion()
{
    ETH_FAIL_MESSAGE("Request: web3_clientVersion");
    return "";
}

// ETH Methods
// perhaps take raw rlp instead ??
std::string ToolImpl::eth_sendRawTransaction(scheme_transaction const& _transaction)
{
    ETH_TEST_MESSAGE("Request: eth_sendRawTransaction \n" + _transaction.getData().asJson());
    m_transactions.push_back(_transaction);
    ETH_TEST_MESSAGE("Response: " + _transaction.getHash());
    return _transaction.getHash();
}

int ToolImpl::eth_getTransactionCount(std::string const& _address, std::string const& _blockNumber)
{
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
    ETH_FAIL_MESSAGE("Request: eth_blockNumber");
    return "";
}

test::scheme_block ToolImpl::eth_getBlockByHash(string const& _hash, bool _fullObjects)
{
    ETH_TEST_MESSAGE("Request: eth_getBlockByHash `" + _hash + "`");
    (void)_fullObjects;
    return test::scheme_block(internalConstructResponseGetBlockByHashOrNumber(_hash));
}

ToolImpl::ToolBlock const& ToolImpl::getBlockByHashOrNumber(string const& _hashOrNumber) const
{
    if (_hashOrNumber.size() == 66)
    {
        for (size_t i = 0; i < m_blockchain.size(); i++)
            if (m_blockchain.at(i).getHash() == _hashOrNumber)
                return m_blockchain.at(i);
    }
    else
    {
        int bNumber = hexOrDecStringToInt(_hashOrNumber) - 1;
        if (bNumber >= 0 && (size_t)bNumber < m_blockchain.size())
            return m_blockchain.at(bNumber);
    }
    ETH_ERROR_MESSAGE("Wrong block hash or number! " + _hashOrNumber);
    return m_blockchain.at(0);
}

DataObject ToolImpl::internalConstructResponseGetBlockByHashOrNumber(
    string const& _blockHashOrNumber) const
{
    DataObject constructResponse;
    ToolBlock const& toolBlock = getBlockByHashOrNumber(_blockHashOrNumber);
    DataObject const& rdata = toolBlock.getToolResponse();
    DataObject const& env = toolBlock.getEnv().atKey("genesis");
    size_t receiptSize = rdata.atKey("receipts").getSubObjects().size();
    if (receiptSize > 0)
    {
        DataObject const& lastReceipt = rdata.atKey("receipts")
                                            .getSubObjects()
                                            .at(rdata.atKey("receipts").getSubObjects().size() - 1);
        constructResponse["extraData"] = env.atKey("extraData");
        constructResponse["gasLimit"] = env.atKey("gasLimit");
        constructResponse["gasUsed"] = "0x00";  // calculate all receipts gas used";
        constructResponse["hash"] = dev::toHexPrefixed(h256(0));  // calculate";
        constructResponse["logsBloom"] = lastReceipt.atKey("logsBloom");
        constructResponse["miner"] = env.atKey("author");
        constructResponse["number"] = dev::toCompactHexPrefixed(toolBlock.getNumber(), 1);
        constructResponse["parentHash"] = rdata.atKey("receiptRoot");
        constructResponse["receiptsRoot"] = rdata.atKey("receiptRoot");
        constructResponse["sha3Uncles"] = dev::toHexPrefixed(h256(0));  // calculate";
        constructResponse["size"] = "0x00";                             // calculate";
        constructResponse["stateRoot"] = rdata.atKey("stateRoot");
        constructResponse["timestamp"] = env.atKey("timestamp");
        constructResponse["transactions"].addArrayObject(dev::toHexPrefixed(h256(1)));  // if tx
                                                                                        // true
        constructResponse["uncles"] = DataObject(DataType::Array);
        constructResponse["transactionsRoot"] = dev::toHexPrefixed(h256(0));  // calculate";
        constructResponse["difficulty"] = env.atKey("difficulty");
    }
    return constructResponse;
}
test::scheme_block ToolImpl::eth_getBlockByNumber(
    BlockNumber const& _blockNumber, bool _fullObjects)
{
    ETH_TEST_MESSAGE("Request: eth_getBlockByNumber " + _blockNumber.getBlockNumberAsString());
    (void)_fullObjects;
    return test::scheme_block(
        internalConstructResponseGetBlockByHashOrNumber(_blockNumber.getBlockNumberAsString()));
}

std::string ToolImpl::eth_getCode(std::string const& _address, std::string const& _blockNumber)
{
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
    ETH_TEST_MESSAGE("Request: debug_accountRange");
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
    ETH_TEST_MESSAGE("Request: debug_storageRangeAt " + _blockHashOrNumber + " " + _address);
    (void)_txIndex;
    (void)_begin;

    DataObject constructResponse;
    ToolBlock const& block = getBlockByHashOrNumber(_blockHashOrNumber);
    if (block.getPostState().count(_address))
    {
        constructResponse["complete"] = true;
        if (block.getPostState().atKey(_address).count("storage"))
        {
            int iStore = 0;
            for (auto const& el :
                block.getPostState().atKey(_address).atKey("storage").getSubObjects())
            {
                DataObject record;
                record["key"] = dev::toCompactHexPrefixed(hexOrDecStringToInt(el.getKey()), 1);
                record["value"] = dev::toCompactHexPrefixed(hexOrDecStringToInt(el.asString()), 1);
                constructResponse["storage"][toString(iStore)] = record;
                if (iStore == _maxResults)
                    break;
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
    ETH_FAIL_MESSAGE("Request: debug_traceTransaction");
    return scheme_debugTraceTransaction(DataObject(_trHash));
}

// Test
void ToolImpl::test_setChainParams(DataObject const& _config)
{
    ETH_TEST_MESSAGE("Request: test_setChainParams \n" + _config.asJson());
    m_chainParams = _config;
}

void ToolImpl::test_rewindToBlock(size_t _blockNr)
{
    ETH_TEST_MESSAGE("Request: test_rewindToBlock");
    if (_blockNr == 0)
        m_blockchain.clear();
    else
    {
        for (size_t i = _blockNr; i < m_blockchain.size(); i++)
            m_blockchain.pop_back();
    }
}

void ToolImpl::test_modifyTimestamp(unsigned long long _timestamp)
{
    ETH_TEST_MESSAGE("Request: test_modifyTimestamp " + DataObject(_timestamp).asJson());
    m_timestamp = _timestamp;
}

string ToolImpl::getGenesisForTool(DataObject const& _genesis) const
{
    DataObject env;
    env["currentCoinbase"] = _genesis.atKey("author");
    env["currentDifficulty"] = _genesis.atKey("difficulty");
    env["currentGasLimit"] = _genesis.atKey("gasLimit");
    env["currentNumber"] = "0x01";
    env["currentTimestamp"] = dev::toCompactHexPrefixed(m_timestamp, 1);
    env["previousHash"] = "0xdac58aa524e50956d0c0bae7f3f8bb9d35381365d07804dd5b48a5a297c06af4";
    env["blockHashes"]["0"] = "0xdac58aa524e50956d0c0bae7f3f8bb9d35381365d07804dd5b48a5a297c06af4";
    return env.asJson();
}

void removeLeadingZerosFromHexValues(DataObject& _obj)
{
    static std::vector<std::string> const c_hashes{std::string{"to"}, std::string{"data"}};
    if (_obj.type() == DataType::String && !inArray(c_hashes, _obj.getKey()))
    {
        string const& origVal = _obj.asString();
        bool replacePossible = true;
        while (replacePossible)
        {
            if (origVal[0] == '0' && origVal[1] == 'x' && origVal[2] == '0' && origVal.size() >= 4)
            {
                _obj.setString("0x" + origVal.substr(3));
                continue;
            }
            replacePossible = false;
        }
    }
}

string ToolImpl::getTxsForTool() const
{
    DataObject txs;
    for (auto const& tx : m_transactions)
    {
        DataObject txToolFormat = tx.getDataForBCTest();
        txToolFormat.performModifier(removeLeadingZerosFromHexValues);
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

string ToolImpl::test_mineBlocks(int _number, bool _canFail)
{
    (void)_canFail;
    ETH_TEST_MESSAGE("Request: test_mineBlocks");
    ETH_ERROR_REQUIRE_MESSAGE(_number == 1, "Make sure test_mineBlocks mine 1 block");

    // env.json file
    fs::path envPath = m_tmpDir / "env.json";
    writeFile(envPath.string(), getGenesisForTool(m_chainParams["genesis"]));

    // txs.json file
    fs::path txsPath = m_tmpDir / "txs.json";
    writeFile(txsPath.string(), getTxsForTool());
    m_transactions.clear();  // comment to trigger transaction rejection

    // alloc.json file
    fs::path allocPath = m_tmpDir / "alloc.json";
    writeFile(allocPath.string(), m_chainParams.atKey("accounts").asJson(0, true, true));

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

    ETH_TEST_MESSAGE("Alloc:\n" + contentsString(allocPath.string()));
    ETH_TEST_MESSAGE("Txs:\n" + contentsString(txsPath.string()));
    ETH_TEST_MESSAGE("Env:\n" + contentsString(envPath.string()));
    test::executeCmd(cmd, false);
    ETH_TEST_MESSAGE("Res:\n" + contentsString(outPath.string()));
    ETH_TEST_MESSAGE("RAlloc:\n" + contentsString(outAllocPath.string()));

    ToolBlock block(ConvertJsoncppStringToData(contentsString(outPath)),  // Tool Result
        m_chainParams,                                                    // Env, alloc info
        ConvertJsoncppStringToData(contentsString(outAllocPath))          // Result state
    );
    block.setHashNumber("0x0bad", m_blockchain.size());

    m_blockchain.push_back(block);
    fs::remove_all(m_tmpDir);
    return toString(m_blockchain.size());
}

string rlpToString(dev::RLP const& _rlp, bool _corretHexOdd = false)
{
    std::ostringstream stream;
    stream << _rlp;
    string ret = stream.str();
    if (ret.size() > 0 && ret[0] == '"')
    {
        string retFormated;
        ret = ret.substr(1);
        while (ret[0] == '\\' && ret[1] == 'x')
        {
            retFormated += ret.substr(2, 2);
            ret = ret.substr(4);
        }
        return "0x" + retFormated;
    }
    if (_corretHexOdd && ret.substr(2).size() % 2 == 1)
        return "0x0" + ret.substr(2);
    return stream.str();
}

string ToolImpl::test_importRawBlock(std::string const& _blockRLP)
{
    ETH_TEST_MESSAGE("Request: test_importRawBlock, following transaction import are internal");
    bytes blockBytes = dev::fromHex(_blockRLP);
    dev::RLP blockRLP(blockBytes);

    dev::RLP header = blockRLP[0];  // block header
    // 0 - parentHash           // 8 - number
    // 1 - uncleHash            // 9 - gasLimit
    // 2 - coinbase             // 10 - gasUsed
    // 3 - stateRoot            // 11 - timestamp
    // 4 - transactionTrie      // 12 - extraData
    // 5 - receiptTrie          // 13 - mixHash
    // 6 - bloom                // 14 - nonce
    // 7 - difficulty

    // Information about current blockheader from ToolImpl prespective
    m_chainParams["genesis"]["author"] = rlpToString(header[2]);
    m_chainParams["genesis"]["difficulty"] = rlpToString(header[7]);
    m_chainParams["genesis"]["gasLimit"] = rlpToString(header[9]);
    m_chainParams["genesis"]["extraData"] = rlpToString(header[12]);
    m_chainParams["genesis"]["timestamp"] = rlpToString(header[11]);
    m_chainParams["genesis"]["nonce"] = rlpToString(header[14]);
    m_chainParams["genesis"]["mixHash"] = rlpToString(header[13]);

    // block transactions
    m_transactions.clear();
    for (auto const& tr : blockRLP[1])
    {
        DataObject trInfo;
        // 0 - nonce        3 - to      6 - v
        // 1 - gasPrice     4 - value   7 - r
        // 2 - gasLimit     5 - data    8 - s
        std::ostringstream stream;
        stream << tr[5];
        trInfo["data"] = stream.str() == "0x0" ? "0x" : rlpToString(tr[5], true);
        trInfo["gasLimit"] = rlpToString(tr[2], true);
        trInfo["gasPrice"] = rlpToString(tr[1], true);
        trInfo["nonce"] = rlpToString(tr[0], true);
        trInfo["to"] = rlpToString(tr[3], true);
        trInfo["value"] = rlpToString(tr[4], true);
        trInfo["v"] = rlpToString(tr[6], true);
        trInfo["r"] = rlpToString(tr[7], true);
        trInfo["s"] = rlpToString(tr[8], true);
        eth_sendRawTransaction(scheme_transaction(trInfo));
    }
    test_mineBlocks(1);
    string blockHash = dev::toHexPrefixed(dev::sha3(header.data()));
    ToolBlock& lastMinedBlock = m_blockchain[m_blockchain.size() - 1];
    lastMinedBlock.setHashNumber(blockHash, test::hexOrDecStringToInt(rlpToString(header[8])));
    ETH_TEST_MESSAGE("Response: " + blockHash);
    return blockHash;
}

std::string ToolImpl::test_getLogHash(std::string const& _txHash)
{
    ETH_TEST_MESSAGE("Request: test_getLogHash " + _txHash);
    for (auto const& bl : m_blockchain)
    {
        for (auto const& receipt : bl.getToolResponse().atKey("receipts").getSubObjects())
        {
            if (receipt.atKey("transactionHash").asString() == _txHash)
                return bl.getToolResponse().atKey("logsHash").asString();
        }
    }
    ETH_WARNING("test_getLogHash _txHash `" + _txHash + "' not found!");
    return "0x1dcc4de8dec75d7aab85b567b6ccd41ad312451b948a7413f0a142fd40d49347";
}


// Internal
DataObject ToolImpl::rpcCall(
    std::string const& _methodName, std::vector<std::string> const& _args, bool _canFail)
{
    ETH_FAIL_MESSAGE("Request: rpcCall");
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
