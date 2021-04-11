#include <cstdio>
#include <thread>

#include <dataObject/ConvertFile.h>
#include <retesteth/EthChecks.h>
#include <retesteth/ExitHandler.h>
#include <retesteth/TestHelper.h>
#include <retesteth/session/RPCImpl.h>
#include <retesteth/session/Session.h>
#include <retesteth/testStructures/Common.h>

using namespace test;

DataObject RPCImpl::web3_clientVersion()
{
    return rpcCall("web3_clientVersion", {});
}

// ETH Methods
FH32 RPCImpl::eth_sendRawTransaction(BYTES const& _rlp, VALUE const& _secret)
{
    (void)_secret;
    DataObject const result = rpcCall("eth_sendRawTransaction", {quote(_rlp.asString())}, true);
    if (!m_lastInterfaceError.empty())
        ETH_ERROR_MESSAGE(m_lastInterfaceError.message());
    return FH32(result);
}

VALUE RPCImpl::eth_getTransactionCount(FH20 const& _address, VALUE const& _blockNumber)
{
    DataObject const response =
        rpcCall("eth_getTransactionCount", {quote(_address.asString()), quote(_blockNumber.asString())});
    if (response.type() == DataType::String)
        return VALUE(response);
    return VALUE(response.asInt());
}

VALUE RPCImpl::eth_blockNumber()
{
    return VALUE(rpcCall("eth_blockNumber", {}));
}

EthGetBlockBy RPCImpl::eth_getBlockByHash(FH32 const& _hash, Request _fullObjects)
{
    return EthGetBlockBy(
        rpcCall("eth_getBlockByHash", {quote(_hash.asString()), _fullObjects == Request::FULLOBJECTS ? "true" : "false"}));
}

EthGetBlockBy RPCImpl::eth_getBlockByNumber(VALUE const& _blockNumber, Request _fullObjects)
{
    return EthGetBlockBy(rpcCall(
        "eth_getBlockByNumber", {quote(_blockNumber.asString()), _fullObjects == Request::FULLOBJECTS ? "true" : "false"}));
}

BYTES RPCImpl::eth_getCode(FH20 const& _address, VALUE const& _blockNumber)
{
    DataObject const res(rpcCall("eth_getCode", {quote(_address.asString()), quote(_blockNumber.asString())}));
    if (res.asString().empty())
    {
        ETH_WARNING_TEST("eth_getCode return `` empty string, correct to `0x` empty bytes ", 6);
        return BYTES(DataObject("0x"));
    }
    return BYTES(res);
}

VALUE RPCImpl::eth_getBalance(FH20 const& _address, VALUE const& _blockNumber)
{
    return VALUE(rpcCall("eth_getBalance", {quote(_address.asString()), quote(_blockNumber.asString())}));
}


// Debug
DebugAccountRange RPCImpl::debug_accountRange(
    VALUE const& _blockNumber, VALUE const& _txIndex, FH32 const& _address, size_t _maxResults)
{
    return rpcCall("debug_accountRange",
        {quote(_blockNumber.asDecString()), _txIndex.asDecString(), quote(_address.asString()), fto_string(_maxResults)});
}

DebugAccountRange RPCImpl::debug_accountRange(
    FH32 const& _blockHash, VALUE const& _txIndex, FH32 const& _address, size_t _maxResults)
{
    return rpcCall("debug_accountRange",
        {quote(_blockHash.asString()), _txIndex.asDecString(), quote(_address.asString()), fto_string(_maxResults)});
}

DebugStorageRangeAt RPCImpl::debug_storageRangeAt(
    VALUE const& _blockNumber, VALUE const& _txIndex, FH20 const& _address, FH32 const& _begin, int _maxResults)
{
    return rpcCall("debug_storageRangeAt", {quote(_blockNumber.asDecString()), _txIndex.asDecString(),
                                               quote(_address.asString()), quote(_begin.asString()), fto_string(_maxResults)});
}

DebugStorageRangeAt RPCImpl::debug_storageRangeAt(
    FH32 const& _blockHash, VALUE const& _txIndex, FH20 const& _address, FH32 const& _begin, int _maxResults)
{
    return rpcCall("debug_storageRangeAt", {quote(_blockHash.asString()), _txIndex.asDecString(), quote(_address.asString()),
                                               quote(_begin.asString()), fto_string(_maxResults)});
}

DebugVMTrace RPCImpl::debug_traceTransaction(FH32 const& _trHash)
{
    (void)_trHash;
    ETH_ERROR_MESSAGE("RPCImpl::debug_traceTransaction is not implemented!");
    static DebugVMTrace empty("", "", FH32::zero(), "");
    return empty;
}

// Test
void RPCImpl::test_setChainParams(SetChainParamsArgs const& _config)
{
    RPCSession::currentCfgCountTestRun();
    ETH_FAIL_REQUIRE_MESSAGE(
        rpcCall("test_setChainParams", {_config.asDataObject().asJson()}) == true, "remote test_setChainParams = false");
}

void RPCImpl::test_rewindToBlock(VALUE const& _blockNr)
{
    ETH_FAIL_REQUIRE_MESSAGE(
        rpcCall("test_rewindToBlock", {_blockNr.asDecString()}) == true, "remote test_rewintToBlock = false");
}

void RPCImpl::test_modifyTimestamp(VALUE const& _timestamp)
{
    ETH_FAIL_REQUIRE_MESSAGE(
        rpcCall("test_modifyTimestamp", {_timestamp.asDecString()}) == true, "test_modifyTimestamp was not successfull");
}

void RPCImpl::test_mineBlocks(size_t _number)
{
    DataObject const res = rpcCall("test_mineBlocks", {to_string(_number)}, true);
    if (res.type() == DataType::Bool)
        ETH_ERROR_REQUIRE_MESSAGE(res.asBool() == true, "remote test_mineBLocks = false");
    else
        ETH_ERROR_MESSAGE("remote test_mineBLocks = " + res.asJson());
}

FH32 RPCImpl::test_importRawBlock(BYTES const& _blockRLP)
{
    DataObject const res = rpcCall("test_importRawBlock", {quote(_blockRLP.asString())}, true);
    if (res.type() == DataType::String && res.asString().size() > 2)
        return FH32(res.asString());
    return FH32(FH32::zero());
}

FH32 RPCImpl::test_getLogHash(FH32 const& _txHash)
{
    return FH32(rpcCall("test_getLogHash", {quote(_txHash.asString())}));
}

// Internal
std::string RPCImpl::sendRawRequest(std::string const& _request)
{
    JsonObjectValidator validator;
    return m_socket.sendRequest(_request, validator);
}

DataObject RPCImpl::rpcCall(
    std::string const& _methodName, std::vector<std::string> const& _args, bool _canFail)
{
    string request = "{\"jsonrpc\":\"2.0\",\"method\":\"" + _methodName + "\",\"params\":[";
    for (size_t i = 0; i < _args.size(); ++i)
    {
        request += _args[i];
        if (i + 1 != _args.size())
            request += ", ";
    }

    request += "],\"id\":" + to_string(m_rpcSequence++) + "}";

    ETH_TEST_MESSAGE("Request: " + request);
    JsonObjectValidator validator;  // read response while counting `{}`
    string reply = m_socket.sendRequest(request, validator);
    ETH_TEST_MESSAGE("Reply: `" + reply + "`");

    DataObject result = ConvertJsoncppStringToData(reply, string(), true);
    if (result.count("error"))
        result["result"] = "";

    if (!ExitHandler::receivedExitSignal())
    {
        requireJsonFields(result, "rpcCall_response (req: '" + request.substr(0, 70) + "')",
            {{"jsonrpc", {{DataType::String}, jsonField::Required}}, {"id", {{DataType::Integer}, jsonField::Required}},
                {"result", {{DataType::String, DataType::Integer, DataType::Bool, DataType::Object, DataType::Array},
                               jsonField::Required}},
                {"error", {{DataType::String, DataType::Object}, jsonField::Optional}}});
    }
    else
    {
        result.clear();
        result["error"]["message"] = "Received Exit Signal";
    }

    if (result.count("error"))
    {
        test::TestOutputHelper const& helper = test::TestOutputHelper::get();
        string const message = "Error on JSON-RPC call (" + helper.testInfo().errorDebug() + "):\nRequest: '" + request + "'" +
                               "\nResult: '" + result["error"]["message"].asString() + "'\n";
        m_lastInterfaceError = RPCError(result["error"]["message"].asString(), message);

        if (_canFail)
            return DataObject(DataType::Null);

        if (!ExitHandler::receivedExitSignal())
            ETH_FAIL_MESSAGE(m_lastInterfaceError.message());
    }
    m_lastInterfaceError.clear();  // null the error as last RPC call was success.
    return result.atKey("result");
}

Socket::SocketType RPCImpl::getSocketType() const
{
    return m_socket.type();
}

std::string const& RPCImpl::getSocketPath() const
{
    return m_socket.path();
}
