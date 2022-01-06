#include <cstdio>
#include <thread>

#include <dataObject/ConvertFile.h>
#include <retesteth/Options.h>
#include <retesteth/session/ToolImpl.h>
#include <retesteth/testStructures/types/BlockchainTests/Filler/BlockchainTestFillerEnv.h>
#include <retesteth/testStructures/types/Ethereum/BlockHeaderReader.h>
#include <retesteth/testStructures/types/Ethereum/TransactionReader.h>

#include "ToolBackend/ToolImplHelper.h"

using namespace test;
using namespace toolimpl;

/// Because ToolBackend is like a light client module of retesteth
/// Execute its logic in try catch blocks. treating any std::exceptions as its failure
/// And treating any Eth exceptions as a message to retesteth behaviour

enum class CallType
{
    DONTFAILONUPWARDS,
    FAILEVERYTHING
};
#define TRYCATCHCALL(X, method, ctype)                                                                     \
    try {                                                                                                  \
        X                                                                                                  \
    }                                                                                                      \
    catch (UpwardsException const& _ex)                                                                    \
    {                                                                                                      \
        makeRPCError(_ex.what());                                                                          \
        ETH_TEST_MESSAGE(string("Response ") + method + ": " + _ex.what());                                \
        if (ctype != CallType::DONTFAILONUPWARDS)                                                          \
            ETH_FAIL_MESSAGE(_ex.what());                                                                  \
    }                                                                                                      \
    catch (EthError const& _ex)                                                                            \
    {                                                                                                      \
        throw EthError(_ex);                                                                               \
    }                                                                                                      \
    catch (std::exception const& _ex)                                                                      \
    {                                                                                                      \
        ETH_FAIL_MESSAGE(_ex.what());                                                                      \
    }                                                                                                      \


spDataObject ToolImpl::web3_clientVersion()
{
    rpcCall("", {});
    ETH_TEST_MESSAGE("\nRequest: web3_clientVersion");
    string const cmd = m_toolPath.string() + " -v";
    TRYCATCHCALL(
                spDataObject res(new DataObject(test::executeCmd(cmd)));
                ETH_TEST_MESSAGE("Response: web3_clientVersion " + res->asString());
                return res;
                , "web3_clientVersion", CallType::FAILEVERYTHING)
    return spDataObject();
}

// ETH Methods
FH32 ToolImpl::eth_sendRawTransaction(BYTES const& _rlp, VALUE const& _secret)
{
    rpcCall("", {});
    ETH_TEST_MESSAGE("\nRequest: eth_sendRawTransaction \n" + _rlp.asString());
    TRYCATCHCALL(
        spTransaction spTr = readTransaction(_rlp);
        spTr.getContent().setSecret(_secret);
        ETH_TEST_MESSAGE(spTr->asDataObject()->asJson());
        m_toolChainManager.getContent().addPendingTransaction(spTr);
        FH32 trHash = spTr.getContent().hash();
        ETH_TEST_MESSAGE("Response: " + trHash.asString());
        return trHash;
        , "eth_sendRawTransaction", CallType::FAILEVERYTHING)
    return FH32::zero();
}

spVALUE ToolImpl::eth_getTransactionCount(FH20 const& _address, VALUE const& _blockNumber)
{
    rpcCall("", {});
    ETH_TEST_MESSAGE("\nRequest: eth_getTransactionCount " + _blockNumber.asString() + " " + _address.asString());
    TRYCATCHCALL(
        spVALUE nonce (blockchain().blockByNumber(_blockNumber).state()->getAccount(_address).nonce().copy());
        ETH_TEST_MESSAGE("Response: eth_getTransactionCount " + nonce->asDecString());
        return nonce;
        , "eth_getTransactionCount", CallType::FAILEVERYTHING)
    return spVALUE(0);
}

VALUE ToolImpl::eth_blockNumber()
{
    rpcCall("", {});
    ETH_TEST_MESSAGE("\nRequest: eth_blockNumber");
    TRYCATCHCALL(
        VALUE const& number = m_toolChainManager.getContent().lastBlock().header()->number();
        string const snumber = number.asDecString();
        ETH_TEST_MESSAGE("Response: eth_blockNumber {" + snumber + "}");
        return number;
        , "eth_blockNumber", CallType::FAILEVERYTHING)
    return VALUE(0);
}


EthGetBlockBy ToolImpl::eth_getBlockByHash(FH32 const& _hash, Request _fullObjects)
{
    rpcCall("", {});
    (void)_fullObjects;
    ETH_TEST_MESSAGE("\nRequest: eth_getBlockByHash `" + _hash.asString());
    TRYCATCHCALL(
        spDataObject res = constructEthGetBlockBy(blockchain().blockByHash(_hash));
        ETH_LOG("Response: eth_getBlockByHash `" + res->asJson(), 7);
        return EthGetBlockBy(res);
        , "eth_getBlockByHash", CallType::FAILEVERYTHING)
    spDataObject spnull(0);
    return EthGetBlockBy(spnull);
}

EthGetBlockBy ToolImpl::eth_getBlockByNumber(VALUE const& _blockNumber, Request _fullObjects)
{
    rpcCall("", {});
    (void)_fullObjects;
    ETH_TEST_MESSAGE("\nRequest: eth_getBlockByNumber `" + _blockNumber.asDecString());
    TRYCATCHCALL(
        spDataObject res = constructEthGetBlockBy(blockchain().blockByNumber(_blockNumber));
        ETH_LOG("Response: eth_getBlockByNumber `" + res->asJson(), 7);
        return EthGetBlockBy(res);
        , "eth_getBlockByNumber", CallType::FAILEVERYTHING)
    spDataObject spnull(0);
    return EthGetBlockBy(spnull);
}

spBYTES ToolImpl::eth_getCode(FH20 const& _address, VALUE const& _blockNumber)
{
    rpcCall("", {});
    ETH_TEST_MESSAGE("\nRequest: eth_getCode " + _blockNumber.asString() + " " + _address.asString());
    TRYCATCHCALL(
        spBYTES code(blockchain().blockByNumber(_blockNumber).state()->getAccount(_address).code().copy());
        ETH_TEST_MESSAGE("Response: eth_getCode " + code->asString());
        return code;
        , "eth_getCode", CallType::FAILEVERYTHING)
    return spBYTES(0);
}

spVALUE ToolImpl::eth_getBalance(FH20 const& _address, VALUE const& _blockNumber)
{
    rpcCall("", {});
    ETH_TEST_MESSAGE("\nRequest: eth_getBalance " + _blockNumber.asString() + " " + _address.asString());
    TRYCATCHCALL(
        // Make a copy here because we do not expose the memory of the tool backend
        spVALUE balance(blockchain().blockByNumber(_blockNumber).state()->getAccount(_address).balance().copy());
        ETH_TEST_MESSAGE("Response: eth_getBalance " + balance->asDecString());
        return balance;
        , "eth_getBalance", CallType::FAILEVERYTHING)
    return spVALUE(0);
}

// Debug
DebugAccountRange ToolImpl::debug_accountRange(
    VALUE const& _blockNumber, VALUE const& _txIndex, FH32 const& _addressHash, size_t _maxResults)
{
    rpcCall("", {});
    ETH_TEST_MESSAGE("\nRequest: debug_accountRange `" + _blockNumber.asDecString() + " " + _addressHash.asString());
    (void) _txIndex;

    TRYCATCHCALL(
        spDataObject constructResponse =
            constructAccountRange(blockchain().blockByNumber(_blockNumber), _addressHash, _maxResults);
        ETH_TEST_MESSAGE("Response: debug_accountRange " + constructResponse->asJson());
        return DebugAccountRange(constructResponse);
        , "debug_accountRange", CallType::FAILEVERYTHING)
    return DebugAccountRange(DataObject());
}

DebugAccountRange ToolImpl::debug_accountRange(
    FH32 const& _blockHash, VALUE const& _txIndex, FH32 const& _addressHash, size_t _maxResults)
{
    rpcCall("", {});
    ETH_TEST_MESSAGE("\nRequest: debug_accountRange " + _blockHash.asString() + " " + _addressHash.asString());
    (void) _txIndex;

    TRYCATCHCALL(
        spDataObject constructResponse = constructAccountRange(blockchain().blockByHash(_blockHash), _addressHash, _maxResults);
        ETH_TEST_MESSAGE("Response: debug_accountRange " + constructResponse->asJson());
        return DebugAccountRange(constructResponse);
        , "debug_accountRange", CallType::FAILEVERYTHING)
    return DebugAccountRange(DataObject());
}

DebugStorageRangeAt ToolImpl::debug_storageRangeAt(
    VALUE const& _blockNumber, VALUE const& _txIndex, FH20 const& _address, FH32 const& _begin, int _maxResults)
{
    rpcCall("", {});
    ETH_TEST_MESSAGE("\nRequest: debug_storageRangeAt bl:" + _blockNumber.asDecString() + " ind:" + _begin.asString() +
                     " addr:" + _address.asString());
    (void) _txIndex;

    TRYCATCHCALL(
        spDataObject constructResponse =
            constructStorageRangeAt(blockchain().blockByNumber(_blockNumber), _address, _begin, _maxResults);
        ETH_TEST_MESSAGE("Response: debug_storageRangeAt " + constructResponse->asJson());
        return DebugStorageRangeAt(constructResponse);
        , "debug_storageRangeAt", CallType::FAILEVERYTHING)
    return DebugStorageRangeAt(DataObject());
}

DebugStorageRangeAt ToolImpl::debug_storageRangeAt(
    FH32 const& _blockHash, VALUE const& _txIndex, FH20 const& _address, FH32 const& _begin, int _maxResults)
{
    rpcCall("", {});
    ETH_TEST_MESSAGE("\nRequest: debug_storageRangeAt bl:" + _blockHash.asString() + " ind:" + _begin.asString() +
                     " addr:" + _address.asString());
    (void) _txIndex;

    TRYCATCHCALL(
        spDataObject constructResponse =
            constructStorageRangeAt(blockchain().blockByHash(_blockHash), _address, _begin, _maxResults);
        ETH_TEST_MESSAGE("Response: debug_storageRangeAt " + constructResponse->asJson());
        return DebugStorageRangeAt(constructResponse);
        , "debug_storageRangeAt", CallType::FAILEVERYTHING)
    return DebugStorageRangeAt(DataObject());
}

DebugVMTrace ToolImpl::debug_traceTransaction(FH32 const& _trHash)
{
    rpcCall("", {});
    ETH_TEST_MESSAGE("\nRequest: debug_traceTransaction: " + _trHash.asString());
    TRYCATCHCALL(
        return blockchain().lastBlock().getTrTrace(_trHash);
        , "debug_traceTransaction", CallType::FAILEVERYTHING)
    return DebugVMTrace();
}

// Test
void ToolImpl::test_setChainParams(spSetChainParamsArgs const& _config)
{
    rpcCall("", {});
    ETH_TEST_MESSAGE("\nRequest: test_setChainParams \n" + _config->asDataObject()->asJson());

    // Ask tool to calculate genesis header stateRoot for genesisHeader
    TRYCATCHCALL(
        m_toolChainManager = GCP_SPointer<ToolChainManager>(new ToolChainManager(_config, m_toolPath, m_tmpDir));
        ETH_TEST_MESSAGE("Response test_setChainParams: {true}");
        , "test_setChainParams", CallType::FAILEVERYTHING)
    ETH_TEST_MESSAGE("Response test_setChainParams: {false}");
}

void ToolImpl::test_rewindToBlock(VALUE const& _blockNr)
{
    rpcCall("", {});
    ETH_TEST_MESSAGE("\nRequest: test_rewindToBlock " + _blockNr.asDecString());
    TRYCATCHCALL(
        blockchain().rewindToBlock(_blockNr);
        , "test_rewindToBlock", CallType::DONTFAILONUPWARDS)
    ETH_TEST_MESSAGE("Response: test_rewindToBlock: " + blockchain().lastBlock().header()->number().asDecString());
}

void ToolImpl::test_modifyTimestamp(VALUE const& _timestamp)
{
    rpcCall("", {});
    ETH_TEST_MESSAGE("\nRequest: test_modifyTimestamp " + _timestamp.asDecString());
    TRYCATCHCALL(
        blockchain().modifyTimestamp(_timestamp);
        ETH_TEST_MESSAGE("Response: test_modifyTimestamp " + _timestamp.asDecString());
            , "test_modifyTimestamp", CallType::DONTFAILONUPWARDS)
}

void ToolImpl::test_setRandom(VALUE const& _random)
{
    rpcCall("", {});
    ETH_TEST_MESSAGE("\nRequest: test_setRandom " + _random.asDecString());
    TRYCATCHCALL(
        blockchain().setRandom(_random);
        ETH_TEST_MESSAGE("Response: test_setRandom " + _random.asDecString());
            , "test_setRandom", CallType::DONTFAILONUPWARDS)
}

MineBlocksResult ToolImpl::test_mineBlocks(size_t _number)
{
    rpcCall("", {});
    ETH_TEST_MESSAGE("\nRequest: test_mineBlocks");
    TRYCATCHCALL(
        spDataObject const res = blockchain().mineBlocks(_number);
        ETH_TEST_MESSAGE("Response test_mineBlocks {" + blockchain().lastBlock().header()->number().asDecString() + "}");
        ETH_TEST_MESSAGE(res->asJson());
        return MineBlocksResult(res);
            , "test_mineBlocks", CallType::FAILEVERYTHING)
    return MineBlocksResult(DataObject());
}

// Import block from RAW rlp and validate it according to ethereum rules
// Very logic heavy function. Must be on the client side. Its a clien logic.
FH32 ToolImpl::test_importRawBlock(BYTES const& _blockRLP)
{
    rpcCall("", {});
    TRYCATCHCALL(
        ETH_TEST_MESSAGE("\nRequest: test_importRawBlock, following transaction import are internal");
        ETH_TEST_MESSAGE(_blockRLP.asString());
        FH32 const hash = blockchain().importRawBlock(_blockRLP);
        ETH_TEST_MESSAGE("Response test_importRawBlock: " + hash.asString());
        return hash;
        , "test_importRawBlock", CallType::DONTFAILONUPWARDS)
    return FH32::zero();
}

FH32 ToolImpl::test_getLogHash(FH32 const& _txHash)
{
    rpcCall("", {});
    TRYCATCHCALL(
        ETH_TEST_MESSAGE("\nRequest: test_getLogHash " + _txHash.asString());
        FH32 const& res = blockchain().lastBlock().logHash();
        ETH_TEST_MESSAGE("Response: test_getLogHash " + res.asString());
        return res;
        , "test_getLogHash", CallType::FAILEVERYTHING)
    return _txHash;
}

TestRawTransaction ToolImpl::test_rawTransaction(BYTES const& _rlp, FORK const& _fork)
{
    rpcCall("", {});
    TRYCATCHCALL(
        ETH_TEST_MESSAGE("\nRequest: test_rawTransaction '" + _rlp.asString() + "', Fork: `" + _fork.asString());
        TestRawTransaction res = ToolChainManager::test_rawTransaction(_rlp, _fork, m_toolPath, m_tmpDir);
        return res;
        , "test_rawTransaction", CallType::FAILEVERYTHING)
    return TestRawTransaction(DataObject());
}

VALUE ToolImpl::test_calculateDifficulty(FORK const& _fork, VALUE const& _blockNumber, VALUE const& _parentTimestamp,
    VALUE const& _parentDifficulty, VALUE const& _currentTimestamp, VALUE const& _uncleNumber)
{
    rpcCall("", {});
    TRYCATCHCALL(
        ETH_TEST_MESSAGE("\nRequest: test_calculateDifficulty '");
        ETH_TEST_MESSAGE("Fork: " + _fork.asString() + ", bn: " + _blockNumber.asString() + ", pt: " + _parentTimestamp.asString() +
            ", pd: " + _parentDifficulty.asString() + ", ct: " + _currentTimestamp.asString() + ", un: " + _uncleNumber.asString());
        return ToolChainManager::test_calculateDifficulty(_fork, _blockNumber, _parentTimestamp, _parentDifficulty, _currentTimestamp, _uncleNumber,
            m_toolPath, m_tmpDir);
        , "test_calculateDifficulty", CallType::FAILEVERYTHING)
    return VALUE(DataObject());
}

// Internal
spDataObject ToolImpl::rpcCall(
    std::string const& _methodName, std::vector<std::string> const& _args, bool _canFail)
{
    m_lastInterfaceError.clear();
    if (Options::get().rpcLimit != 0 && Options::get().rpcLimit < m_totalCalls++)
        ETH_FAIL_MESSAGE("\nRequest: rpcCall of test limit");
    (void)_methodName;
    (void)_args;
    (void)_canFail;
    return spDataObject(0);
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
