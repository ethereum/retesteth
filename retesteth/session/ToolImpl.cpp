#include <chrono>
#include <cstdio>
#include <thread>

#include <dataObject/ConvertFile.h>
#include <retesteth/ethObjects/common.h>
#include <retesteth/session/ToolImpl.h>

void ToolImpl::executeTool()
{
    /*
#if defined(_WIN32)
    BOOST_ERROR("LLL compilation only supported on posix systems.");
    return "";
#else
    fs::path path(fs::temp_directory_path() / fs::unique_path());

    string cmd = string(m_toolPath);
    cmd += path.string();

    writeFile(path.string(), _code);
    string result = executeCmd(cmd);
    fs::remove_all(path);
    result = "0x" + result;
    checkHexHasEvenLength(result);
    return result;
#endif
*/
    return;
}

std::string ToolImpl::web3_clientVersion()
{
    ETH_TEST_MESSAGE("Request: web3_clientVersion");
    return "";
}

// ETH Methods
std::string ToolImpl::eth_sendRawTransaction(scheme_transaction const& _transaction)
{
    ETH_TEST_MESSAGE("Request: eth_sendRawTransaction \n" + _transaction.getData().asJson());
    m_transactions.push_back(_transaction);
    return "";
}

int ToolImpl::eth_getTransactionCount(std::string const& _address, std::string const& _blockNumber)
{
    ETH_TEST_MESSAGE("Request: eth_getTransactionCount");
    (void)_address;
    (void)_blockNumber;
    return 0;
}

std::string ToolImpl::eth_blockNumber()
{
    ETH_TEST_MESSAGE("Request: eth_blockNumber");
    return "";
}

test::scheme_block ToolImpl::eth_getBlockByHash(string const& _hash, bool _fullObjects)
{
    ETH_TEST_MESSAGE("Request: eth_getBlockByHash");
    (void)_fullObjects;
    return test::scheme_block(DataObject(_hash));
}

test::scheme_block ToolImpl::eth_getBlockByNumber(
    BlockNumber const& _blockNumber, bool _fullObjects)
{
    ETH_TEST_MESSAGE("Request: eth_getBlockByNumber");
    (void)_fullObjects;
    (void)_blockNumber;
    return test::scheme_block(DataObject());
}

std::string ToolImpl::eth_getCode(std::string const& _address, std::string const& _blockNumber)
{
    ETH_TEST_MESSAGE("Request: eth_getCode");
    (void)_address;
    (void)_blockNumber;
    return "";
}

std::string ToolImpl::eth_getBalance(std::string const& _address, std::string const& _blockNumber)
{
    ETH_TEST_MESSAGE("Request: eth_getBalance");
    (void)_address;
    (void)_blockNumber;
    return "";
}

// Debug
scheme_debugAccountRange ToolImpl::debug_accountRange(std::string const& _blockHashOrNumber,
    int _txIndex, std::string const& _address, int _maxResults)
{
    ETH_TEST_MESSAGE("Request: debug_accountRange");
    (void)_txIndex;
    (void)_address;
    (void)_maxResults;
    return scheme_debugAccountRange(DataObject(_blockHashOrNumber));
}


DataObject ToolImpl::debug_storageRangeAt(std::string const& _blockHashOrNumber, int _txIndex,
    std::string const& _address, std::string const& _begin, int _maxResults)
{
    ETH_TEST_MESSAGE("Request: debug_storageRangeAt");
    (void)_txIndex;
    (void)_address;
    (void)_begin;
    (void)_maxResults;
    return DataObject(_blockHashOrNumber);
}

scheme_debugTraceTransaction ToolImpl::debug_traceTransaction(std::string const& _trHash)
{
    ETH_TEST_MESSAGE("Request: debug_traceTransaction");
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
    (void)_blockNr;
}

void ToolImpl::test_modifyTimestamp(unsigned long long _timestamp)
{
    ETH_TEST_MESSAGE("Request: test_modifyTimestamp \n{" + DataObject(_timestamp).asJson() + "}");
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
    return env.asJson();
}

string ToolImpl::getTxsForTool() const
{
    DataObject txs;
    for (auto const& tx : m_transactions)
    {
        DataObject txToolFormat = tx.getDataForBCTest();
        txToolFormat.renameKey("data", "input");
        txToolFormat.renameKey("gasLimit", "gas");
        txToolFormat["hash"] = "0x0557bacce3375c98d806609b8d5043072f0b6a8bae45ae5a67a00d3a1a18d673";
        txs.addArrayObject(txToolFormat);
    }
    return txs.asJson();
}

string ToolImpl::test_mineBlocks(int _number, bool _canFail)
{
    ETH_TEST_MESSAGE("Request: test_mineBlocks");

    // env.json file
    fs::path envPath = m_tmpDir / "env.json";
    writeFile(envPath.string(), getGenesisForTool(m_chainParams["genesis"]));

    // txs.json file
    fs::path txsPath = m_tmpDir / "txs.json";
    writeFile(txsPath.string(), getTxsForTool());

    ETH_TEST_MESSAGE(txsPath.string());
    // m_tmpDir = ;

    (void)_number;
    (void)_canFail;
    return "";
}

string ToolImpl::test_importRawBlock(std::string const& _blockRLP)
{
    ETH_TEST_MESSAGE("Request: test_importRawBlock");
    (void)_blockRLP;
    return string();
}

std::string ToolImpl::test_getLogHash(std::string const& _txHash)
{
    ETH_TEST_MESSAGE("Request: test_getLogHash");
    (void)_txHash;
    return string();
}


// Internal
DataObject ToolImpl::rpcCall(
    std::string const& _methodName, std::vector<std::string> const& _args, bool _canFail)
{
    ETH_TEST_MESSAGE("Request: rpcCall");
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
