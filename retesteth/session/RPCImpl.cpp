
#include <chrono>
#include <cstdio>
#include <thread>

#include <dataObject/ConvertFile.h>
#include <retesteth/ethObjects/common.h>
#include <retesteth/session/RPCImpl.h>

std::string RPCImpl::web3_clientVersion()
{
    return rpcCall("web3_clientVersion", {}).asString();
}

// ETH Methods
std::string RPCImpl::eth_sendRawTransaction(scheme_transaction const& _transaction)
{
    DataObject result =
        rpcCall("eth_sendRawTransaction", {quote(_transaction.getSignedRLP())}, true);

    DataObject const& lastError = getLastRPCError();
    if (lastError.type() != DataType::Null)
        ETH_ERROR_MESSAGE(lastError.atKey("message").asString());
    if (!isHash<h256>(result.asString()))
        ETH_ERROR_MESSAGE(
            "eth_sendRawTransaction return invalid hash: '" + result.asString() + "'");
    if (result.type() == DataType::Null)  // if the method failed
        return "";
    return result.asString();
}

int RPCImpl::eth_getTransactionCount(std::string const& _address, std::string const& _blockNumber)
{
    DataObject res = rpcCall("eth_getTransactionCount", {quote(_address), quote(_blockNumber)});
    return (res.type() == DataType::String) ? test::hexOrDecStringToInt(res.asString()) :
                                              res.asInt();
}

std::string RPCImpl::eth_blockNumber()
{
    DataObject res = rpcCall("eth_blockNumber", {});
    return res.type() == DataType::String ? res.asString() : toString(res.asInt());
}

test::scheme_block RPCImpl::eth_getBlockByHash(string const& _hash, bool _fullObjects)
{
    return test::scheme_block(
        rpcCall("eth_getBlockByHash", {quote(_hash), _fullObjects ? "true" : "false"}));
}

test::scheme_block RPCImpl::eth_getBlockByNumber(BlockNumber const& _blockNumber, bool _fullObjects)
{
    return test::scheme_block(rpcCall("eth_getBlockByNumber",
        {quote(_blockNumber.getBlockNumberAsString()), _fullObjects ? "true" : "false"}));
}

std::string RPCImpl::eth_getCode(std::string const& _address, std::string const& _blockNumber)
{
    return rpcCall("eth_getCode", {quote(_address), quote(_blockNumber)}).asString();
}

std::string RPCImpl::eth_getBalance(std::string const& _address, std::string const& _blockNumber)
{
    string address = (_address.length() == 20) ? "0x" + _address : _address;
    return rpcCall("eth_getBalance", {quote(address), quote(_blockNumber)}).asString();
}


// Debug
scheme_debugAccountRange RPCImpl::debug_accountRange(std::string const& _blockHashOrNumber,
    int _txIndex, std::string const& _address, int _maxResults)
{
    return scheme_debugAccountRange(rpcCall(
        "debug_accountRange", {quote(toString(u256(test::sfromHex(_blockHashOrNumber)))),
                                  to_string(_txIndex), quote(_address), to_string(_maxResults)}));
}


DataObject RPCImpl::debug_storageRangeAt(std::string const& _blockHashOrNumber, int _txIndex,
    std::string const& _address, std::string const& _begin, int _maxResults)
{
    return rpcCall("debug_storageRangeAt",
        {quote(toString(u256(test::sfromHex(_blockHashOrNumber)))), to_string(_txIndex),
            quote(_address), quote(_begin), to_string(_maxResults)});
}

scheme_debugTraceTransaction RPCImpl::debug_traceTransaction(std::string const& _trHash)
{
    return scheme_debugTraceTransaction(rpcCall("debug_traceTransaction", {quote(_trHash), "{}"}));
}

// Test
void RPCImpl::test_setChainParams(DataObject const& _config)
{
    ETH_FAIL_REQUIRE_MESSAGE(rpcCall("test_setChainParams", {_config.asJson()}) == true,
        "remote test_setChainParams = false");
}

void RPCImpl::test_rewindToBlock(size_t _blockNr)
{
    ETH_FAIL_REQUIRE_MESSAGE(rpcCall("test_rewindToBlock", {to_string(_blockNr)}) == true,
        "remote test_rewintToBlock = false");
}

void RPCImpl::test_modifyTimestamp(unsigned long long _timestamp)
{
    ETH_FAIL_REQUIRE_MESSAGE(rpcCall("test_modifyTimestamp", {to_string(_timestamp)}) == true,
        "test_modifyTimestamp was not successfull");
}

string RPCImpl::test_mineBlocks(int _number, bool _canFail)
{
    DataObject blockNumber = rpcCall("eth_blockNumber");
    u256 startBlock = (blockNumber.type() == DataType::String) ? u256(blockNumber.asString()) :
                                                                 blockNumber.asInt();

    static const string c_error = "remote test_mineBLocks = false";
    if (!_canFail)
        ETH_ERROR_REQUIRE_MESSAGE(
            rpcCall("test_mineBlocks", {to_string(_number)}, true) == true, c_error);
    else
        return c_error;

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
    return toString(startBlock);
}

string RPCImpl::test_importRawBlock(std::string const& _blockRLP)
{
    DataObject res = rpcCall("test_importRawBlock", {quote(_blockRLP)}, true);
    if (res.type() != DataType::Null)
        return res.asString();
    return string();
}

std::string RPCImpl::test_getLogHash(std::string const& _txHash)
{
    return rpcCall("test_getLogHash", {quote(_txHash)}).asString();
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
            "Error on JSON-RPC call (" + helper.testInfo().getMessage() + "):\nRequest: '" +
            request + "'" + "\nResult: '" + result["error"]["message"].asString() + "'\n";
        m_lastInterfaceError["error"] = result["error"]["message"].asString();
        if (_canFail)
            return DataObject(DataType::Null);
        ETH_FAIL_MESSAGE(m_lastInterfaceError.atKey("message").asString());
    }
    m_lastInterfaceError.clear();  // null the error as last RPC call was success.
    return result["result"];
}

Socket::SocketType RPCImpl::getSocketType() const
{
    return m_socket.type();
}

std::string const& RPCImpl::getSocketPath() const
{
    return m_socket.path();
}
