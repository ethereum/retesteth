#include <chrono>
#include <cstdio>
#include <thread>

#include <dataObject/ConvertFile.h>
#include <retesteth/session/RPCImpl.h>

DataObject RPCImpl::web3_clientVersion()
{
    return rpcCall("web3_clientVersion", {});
}

// ETH Methods
DataObject RPCImpl::eth_sendRawTransaction(BYTES const& _rlp)
{
    DataObject result = rpcCall("eth_sendRawTransaction", {quote(_rlp.asString())}, true);
    DataObject const& lastError = getLastRPCError();
    if (lastError.type() != DataType::Null)
        ETH_ERROR_MESSAGE(lastError.atKey("message").asString());
    if (!isHash<h256>(result.asString()))
        ETH_ERROR_MESSAGE("eth_sendRawTransaction return invalid hash: '" + result.asString() + "'");
    return result;
}

int RPCImpl::eth_getTransactionCount(FH20 const& _address, VALUE const& _blockNumber)
{
    DataObject response = rpcCall("eth_getTransactionCount", {quote(_address.asString()), quote(_blockNumber.asString())});
    if (response.type() == DataType::String)
        return test::hexOrDecStringToInt(response.asString());
    return response.asInt();
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
    DataObject res(rpcCall("eth_getCode", {quote(_address.asString()), quote(_blockNumber.asString())}));
    if (res.asString().empty())
    {
        ETH_WARNING_TEST("eth_getCode return `` empty string, correct to `0x` empty bytes ", 6);
        res = "0x";
    }
    return BYTES(res);
}

VALUE RPCImpl::eth_getBalance(FH20 const& _address, VALUE const& _blockNumber)
{
    return VALUE(rpcCall("eth_getBalance", {quote(_address.asString()), quote(_blockNumber.asString())}));
}


// Debug
DebugAccountRange RPCImpl::debug_accountRange(
    VALUE const& _blockNumber, VALUE const& _txIndex, FH32 const& _address, int _maxResults)
{
    return rpcCall("debug_accountRange",
        {quote(_blockNumber.asDecString()), _txIndex.asDecString(), quote(_address.asString()), to_string(_maxResults)});
}

DebugAccountRange RPCImpl::debug_accountRange(
    FH32 const& _blockHash, VALUE const& _txIndex, FH32 const& _address, int _maxResults)
{
    return rpcCall("debug_accountRange",
        {quote(_blockHash.asString()), _txIndex.asDecString(), quote(_address.asString()), to_string(_maxResults)});
}

DebugStorageRangeAt RPCImpl::debug_storageRangeAt(
    VALUE const& _blockNumber, VALUE const& _txIndex, FH20 const& _address, FH32 const& _begin, int _maxResults)
{
    return rpcCall("debug_storageRangeAt", {quote(_blockNumber.asDecString()), _txIndex.asDecString(),
                                               quote(_address.asString()), quote(_begin.asString()), to_string(_maxResults)});
}

DebugStorageRangeAt RPCImpl::debug_storageRangeAt(
    FH32 const& _blockHash, VALUE const& _txIndex, FH20 const& _address, FH32 const& _begin, int _maxResults)
{
    return rpcCall("debug_storageRangeAt", {quote(_blockHash.asString()), _txIndex.asDecString(), quote(_address.asString()),
                                               quote(_begin.asString()), to_string(_maxResults)});
}

DebugTraceTransaction RPCImpl::debug_traceTransaction(FH32 const& _trHash)
{
    return DebugTraceTransaction(rpcCall("debug_traceTransaction", {quote(_trHash.asString()), "{}"}));
}

// Test
void RPCImpl::test_setChainParams(DataObject const& _config)
{
    ETH_FAIL_REQUIRE_MESSAGE(rpcCall("test_setChainParams", {_config.asJson()}) == true,
        "remote test_setChainParams = false");
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

void RPCImpl::test_mineBlocks(int _number)
{
    // VALUE startBlock = rpcCall("eth_blockNumber");

    DataObject res = rpcCall("test_mineBlocks", {to_string(_number)}, true);
    ETH_ERROR_REQUIRE_MESSAGE(res.asBool() == true, "remote test_mineBLocks = false");
    // CAN DO GUARD HERE THAT BLOCK NUMBER HAS REALLY CHANGED

    /*
    // We auto-calibrate the time it takes to mine the transaction.
    // It would be better to go without polling, but that would probably need a change to the test
    // client
    auto startTime = std::chrono::steady_clock::now();
    unsigned sleepTime = m_sleepTime;
    size_t tries = 0;
    for (;; ++tries)
    {
        std::this_thread::sleep_for(chrono::milliseconds(sleepTime));
        auto endTime = std::chrono::steady_clock::now();
        unsigned timeSpent =
            std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();
        if (timeSpent > m_maxMiningTime)
            break;  // could be that some blocks are invalid.
        // ETH_FAIL_MESSAGE("Error in test_mineBlocks: block mining timeout! " +
        // test::TestOutputHelper::get().testName());

        blockNumber = rpcCall("eth_blockNumber");
        u256 number = (blockNumber.type() == DataType::String) ? u256(blockNumber.asString()) :
                                                                 blockNumber.asInt();
        if (number >= startBlock + _number)
            return toString(number);
        else
            sleepTime *= 2;

        if (tries > 1)
        {
            m_successfulMineRuns = 0;
            m_sleepTime += 2;
        }
        else if (tries == 1)
        {
            m_successfulMineRuns++;
            if (m_successfulMineRuns > 5)
            {
                m_successfulMineRuns = 0;
                if (m_sleepTime > 2)
                    m_sleepTime--;
            }
        }
    }

    // Better keep it int everywhere in codebase. !!!
    return DataObject(toString(startBlock));
    */
}

DataObject RPCImpl::test_importRawBlock(BYTES const& _blockRLP)
{
    DataObject res = rpcCall("test_importRawBlock", {quote(_blockRLP.asString())}, true);
    if (res.type() != DataType::Null)
        return res.asString();
    return string();
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
    ETH_TEST_MESSAGE("Reply: " + reply);

    DataObject result = ConvertJsoncppStringToData(reply, string(), true);
    if (result.count("error"))
        result["result"] = "";
    requireJsonFields(result, "rpcCall_response ('" + request.substr(0, 70) + "')",
        {{"jsonrpc", {{DataType::String}, jsonField::Required}},
            {"id", {{DataType::Integer}, jsonField::Required}},
            {"result", {{DataType::String, DataType::Integer, DataType::Bool, DataType::Object,
                            DataType::Array},
                           jsonField::Required}},
            {"error", {{DataType::String, DataType::Object}, jsonField::Optional}}});

    if (result.count("error"))
    {
        test::TestOutputHelper const& helper = test::TestOutputHelper::get();
        m_lastInterfaceError["message"] =
            "Error on JSON-RPC call (" + helper.testInfo().errorDebug() + "):\nRequest: '" +
            request + "'" + "\nResult: '" + result["error"]["message"].asString() + "'\n";
        m_lastInterfaceError["error"] = result["error"]["message"].asString();
        if (_canFail)
            return DataObject(DataType::Null);
        ETH_FAIL_MESSAGE(m_lastInterfaceError.atKey("message").asString());
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
