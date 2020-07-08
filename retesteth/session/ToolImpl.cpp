#include <cstdio>
#include <thread>

#include <dataObject/ConvertFile.h>
#include <retesteth/Options.h>
#include <retesteth/session/ToolImpl.h>
#include <retesteth/testStructures/types/BlockchainTests/Filler/BlockchainTestFillerEnv.h>

#include "ToolBackend/ToolImplHelper.h"

using namespace test;
using namespace toolimpl;

DataObject ToolImpl::web3_clientVersion()
{
    rpcCall("", {});
    ETH_TEST_MESSAGE("\nRequest: web3_clientVersion");
    string const cmd = m_toolPath.string() + " -v";
    DataObject res(test::executeCmd(cmd));
    ETH_TEST_MESSAGE("Response: web3_clientVersion " + res.asString());
    return res;
}

// ETH Methods
FH32 ToolImpl::eth_sendRawTransaction(BYTES const& _rlp)
{
    rpcCall("", {});
    ETH_TEST_MESSAGE("\nRequest: eth_sendRawTransaction \n" + _rlp.asString());
    Transaction tr(_rlp);
    m_toolChainManager.getContent().addPendingTransaction(tr);
    FH32 trHash = tr.hash();
    ETH_TEST_MESSAGE("Response: " + trHash.asString());
    return trHash;
}

size_t ToolImpl::eth_getTransactionCount(FH20 const& _address, VALUE const& _blockNumber)
{
    rpcCall("", {});
    ETH_TEST_MESSAGE("\nRequest: eth_getTransactionCount " + _blockNumber.asString() + " " + _address.asString());
    VALUE const& nonce = blockchain().blockByNumber(_blockNumber).state().getAccount(_address).nonce();
    ETH_TEST_MESSAGE("Response: eth_getTransactionCount " + nonce.asDecString());
    return (size_t)nonce.asU256();
}

VALUE ToolImpl::eth_blockNumber()
{
    rpcCall("", {});
    ETH_TEST_MESSAGE("\nRequest: eth_blockNumber");
    string const snumber = m_toolChainManager.getContent().lastBlock().header().number().asDecString();
    ETH_TEST_MESSAGE("Response: eth_blockNumber {" + snumber + "}");
    return m_toolChainManager.getContent().lastBlock().header().number();
}


EthGetBlockBy ToolImpl::eth_getBlockByHash(FH32 const& _hash, Request _fullObjects)
{
    rpcCall("", {});
    (void)_fullObjects;
    ETH_TEST_MESSAGE("\nRequest: eth_getBlockByHash `" + _hash.asString());
    DataObject res = constructEthGetBlockBy(blockchain().blockByHash(_hash));
    ETH_LOG("Response: eth_getBlockByHash `" + res.asJson(), 7);
    return EthGetBlockBy(res);
}

EthGetBlockBy ToolImpl::eth_getBlockByNumber(VALUE const& _blockNumber, Request _fullObjects)
{
    rpcCall("", {});
    (void)_fullObjects;
    ETH_TEST_MESSAGE("\nRequest: eth_getBlockByNumber `" + _blockNumber.asDecString());
    DataObject res = constructEthGetBlockBy(blockchain().blockByNumber(_blockNumber));
    ETH_LOG("Response: eth_getBlockByNumber `" + res.asJson(), 7);
    return EthGetBlockBy(res);
}

BYTES ToolImpl::eth_getCode(FH20 const& _address, VALUE const& _blockNumber)
{
    rpcCall("", {});
    ETH_TEST_MESSAGE("\nRequest: eth_getCode " + _blockNumber.asString() + " " + _address.asString());
    BYTES const& code = blockchain().blockByNumber(_blockNumber).state().getAccount(_address).code();
    ETH_TEST_MESSAGE("Response: eth_getCode " + code.asString());
    return code;
}

VALUE ToolImpl::eth_getBalance(FH20 const& _address, VALUE const& _blockNumber)
{
    rpcCall("", {});
    ETH_TEST_MESSAGE("\nRequest: eth_getBalance " + _blockNumber.asString() + " " + _address.asString());
    VALUE const& balance = blockchain().blockByNumber(_blockNumber).state().getAccount(_address).balance();
    ETH_TEST_MESSAGE("Response: eth_getBalance " + balance.asDecString());
    return balance;
}

// Debug
DebugAccountRange ToolImpl::debug_accountRange(
    VALUE const& _blockNumber, VALUE const& _txIndex, FH32 const& _addressHash, size_t _maxResults)
{
    rpcCall("", {});
    ETH_TEST_MESSAGE("\nRequest: debug_accountRange `" + _blockNumber.asDecString() + " " + _addressHash.asString());

    (void)_txIndex;
    (void)_addressHash;
    DataObject constructResponse = constructAccountRange(blockchain().blockByNumber(_blockNumber), _addressHash, _maxResults);
    ETH_TEST_MESSAGE("Response: debug_accountRange " + constructResponse.asJson());
    return DebugAccountRange(constructResponse);
}

DebugAccountRange ToolImpl::debug_accountRange(
    FH32 const& _blockHash, VALUE const& _txIndex, FH32 const& _addressHash, size_t _maxResults)
{
    rpcCall("", {});
    ETH_TEST_MESSAGE("\nRequest: debug_accountRange " + _blockHash.asString() + " " + _addressHash.asString());

    (void)_txIndex;
    (void)_addressHash;
    DataObject constructResponse = constructAccountRange(blockchain().blockByHash(_blockHash), _addressHash, _maxResults);
    ETH_TEST_MESSAGE("Response: debug_accountRange " + constructResponse.asJson());
    return DebugAccountRange(constructResponse);
}

DebugStorageRangeAt ToolImpl::debug_storageRangeAt(
    VALUE const& _blockNumber, VALUE const& _txIndex, FH20 const& _address, FH32 const& _begin, int _maxResults)
{
    rpcCall("", {});
    ETH_TEST_MESSAGE("\nRequest: debug_storageRangeAt bl:" + _blockNumber.asDecString() + " ind:" + _begin.asString() +
                     " addr:" + _address.asString());
    DataObject constructResponse =
        constructStorageRangeAt(blockchain().blockByNumber(_blockNumber), _address, _begin, _maxResults);
    (void)_txIndex;
    (void)_maxResults;
    ETH_TEST_MESSAGE("Response: debug_storageRangeAt " + constructResponse.asJson());
    return DebugStorageRangeAt(constructResponse);
}

DebugStorageRangeAt ToolImpl::debug_storageRangeAt(
    FH32 const& _blockHash, VALUE const& _txIndex, FH20 const& _address, FH32 const& _begin, int _maxResults)
{
    rpcCall("", {});
    ETH_TEST_MESSAGE("\nRequest: debug_storageRangeAt bl:" + _blockHash.asString() + " ind:" + _begin.asString() +
                     " addr:" + _address.asString());
    DataObject constructResponse = constructStorageRangeAt(blockchain().blockByHash(_blockHash), _address, _begin, _maxResults);
    (void)_txIndex;
    (void)_maxResults;
    ETH_TEST_MESSAGE("Response: debug_storageRangeAt " + constructResponse.asJson());
    return DebugStorageRangeAt(constructResponse);
}

DebugTraceTransaction ToolImpl::debug_traceTransaction(FH32 const& _trHash)
{
    rpcCall("", {});
    (void)_trHash;
    ETH_FAIL_MESSAGE("\nRequest: debug_traceTransaction");
    return DebugTraceTransaction(DataObject());
}

// Test
void ToolImpl::test_setChainParams(SetChainParamsArgs const& _config)
{
    rpcCall("", {});
    ETH_TEST_MESSAGE("\nRequest: test_setChainParams \n" + _config.asDataObject().asJson());

    // Ask tool to calculate genesis header stateRoot for genesisHeader
    EthereumBlock genesis(BlockHeader(_config.genesis()));
    m_toolChainManager = GCP_SPointer<ToolChainManager>(new ToolChainManager(_config, m_toolPath, m_tmpDir));

    ETH_TEST_MESSAGE("Response test_setChainParams: {true}");
}

void ToolImpl::test_rewindToBlock(VALUE const& _blockNr)
{
    rpcCall("", {});
    ETH_TEST_MESSAGE("\nRequest: test_rewindToBlock " + _blockNr.asDecString());
    blockchain().rewindToBlock(_blockNr);
    ETH_TEST_MESSAGE("Response: test_rewindToBlock: " + blockchain().lastBlock().header().number().asDecString());
}

void ToolImpl::test_modifyTimestamp(VALUE const& _timestamp)
{
    rpcCall("", {});
    ETH_TEST_MESSAGE("\nRequest: test_modifyTimestamp " + _timestamp.asDecString());
    blockchain().modifyTimestamp(_timestamp);
    ETH_TEST_MESSAGE("Response: test_modifyTimestamp " + _timestamp.asDecString());
}

void ToolImpl::test_mineBlocks(size_t _number)
{
    rpcCall("", {});
    ETH_TEST_MESSAGE("\nRequest: test_mineBlocks");
    try
    {
        blockchain().mineBlocks(_number);
    }
    catch (std::exception const& _ex)
    {
        ETH_ERROR_MESSAGE(_ex.what());
    }

    ETH_TEST_MESSAGE("Response test_mineBlocks {" + blockchain().lastBlock().header().number().asDecString() + "}");
}

// Import block from RAW rlp and validate it according to ethereum rules
// Very logic heavy function. Must be on the client side. Its a clien logic.
FH32 ToolImpl::test_importRawBlock(BYTES const& _blockRLP)
{
    rpcCall("", {});
    try
    {
        ETH_TEST_MESSAGE("\nRequest: test_importRawBlock, following transaction import are internal");
        FH32 const hash = blockchain().importRawBlock(_blockRLP);
        ETH_TEST_MESSAGE("Response test_importRawBlock: " + hash.asString());
        return hash;
    }
    catch (std::exception const& _ex)
    {
        makeRPCError(_ex.what());
        ETH_TEST_MESSAGE(string("Response test_importRawBlock: ") + _ex.what());
        return FH32::zero();
    }
}

FH32 ToolImpl::test_getLogHash(FH32 const& _txHash)
{
    rpcCall("", {});
    (void)_txHash;
    ETH_TEST_MESSAGE("\nRequest: test_getLogHash " + _txHash.asString());
    FH32 const& res = blockchain().lastBlock().logHash();
    ETH_TEST_MESSAGE("Response: test_getLogHash " + res.asString());
    return res;
}

// Internal
DataObject ToolImpl::rpcCall(
    std::string const& _methodName, std::vector<std::string> const& _args, bool _canFail)
{
    m_lastInterfaceError.clear();
    if (Options::get().rpcLimit != 0 && Options::get().rpcLimit < m_totalCalls++)
        ETH_FAIL_MESSAGE("\nRequest: rpcCall of test limit");
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

void ToolImpl::makeRPCError(string const& _error)
{
    ETH_LOG("makeRPCError " + _error, 5);
    m_lastInterfaceError = RPCError("", _error);
}
