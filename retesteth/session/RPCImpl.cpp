#include <cstdio>
#include <thread>

#include <libdataobj/ConvertFile.h>
#include <retesteth/EthChecks.h>
#include <retesteth/ExitHandler.h>
#include <retesteth/TestHelper.h>
#include <retesteth/session/RPCImpl.h>
#include <retesteth/session/Session.h>
#include <retesteth/testStructures/Common.h>
#include <retesteth/Options.h>

using namespace test;
using namespace test::debug;

spDataObject RPCImpl::web3_clientVersion()
{
    return rpcCall("web3_clientVersion", {});
}

// ETH Methods
FH32 RPCImpl::eth_sendRawTransaction(BYTES const& _rlp, VALUE const& _secret)
{
    (void)_secret;
    spDataObject const result = rpcCall("eth_sendRawTransaction", {quote(_rlp.asString())}, true);
    if (!m_lastInterfaceError.empty())
        ETH_ERROR_MESSAGE(m_lastInterfaceError.message());
    return FH32(result.getCContent());
}

spVALUE RPCImpl::eth_getTransactionCount(FH20 const& _address, VALUE const& _blockNumber)
{
    try
    {
        spDataObject response = rpcCall("eth_getTransactionCount", {quote(_address.asString()), quote(_blockNumber.asString())});
        (*response).performModifier(mod_valueToCompactEvenHexPrefixed);
        if (response->type() == DataType::String)
            return spVALUE(new VALUE(response));
        return spVALUE(new VALUE(response->asInt()));
    }
    catch(std::exception const& _ex)
    {
        ETH_FAIL_MESSAGE(string("RPC eth_getTransactionCount Exception: ") + _ex.what());
    }
    return spVALUE(0);
}

VALUE RPCImpl::eth_blockNumber()
{
    return VALUE(rpcCall("eth_blockNumber", {}).getCContent());
}

EthGetBlockBy RPCImpl::eth_getBlockByHash(FH32 const& _hash, Request _fullObjects)
{
    spDataObject response = rpcCall("eth_getBlockByHash", {quote(_hash.asString()), _fullObjects == Request::FULLOBJECTS ? "true" : "false"});
    ClientConfig const& cfg = Options::getCurrentConfig();
    cfg.performFieldReplace(*response, FieldReplaceDir::ClientToRetesteth);
    return EthGetBlockBy(response);
}

EthGetBlockBy RPCImpl::eth_getBlockByNumber(VALUE const& _blockNumber, Request _fullObjects)
{
    spDataObject response = rpcCall("eth_getBlockByNumber", {quote(_blockNumber.asString()), _fullObjects == Request::FULLOBJECTS ? "true" : "false"});
    ClientConfig const& cfg = Options::getCurrentConfig();
    cfg.performFieldReplace(*response, FieldReplaceDir::ClientToRetesteth);
    return EthGetBlockBy(response);
}

spBYTES RPCImpl::eth_getCode(FH20 const& _address, VALUE const& _blockNumber)
{
    spDataObject res = rpcCall("eth_getCode", {quote(_address.asString()), quote(_blockNumber.asString())});
    if (res->asString().empty())
    {
        ETH_DC_MESSAGE(DC::LOWLOG, "eth_getCode return `` empty string, correct to `0x` empty bytes ");
        return spBYTES(new BYTES(DataObject("0x")));
    }
    return spBYTES(new BYTES(res));
}

spVALUE RPCImpl::eth_getBalance(FH20 const& _address, VALUE const& _blockNumber)
{
    auto ret = rpcCall("eth_getBalance", {quote(_address.asString()), quote(_blockNumber.asString())});
    return spVALUE(new VALUE(ret));
}


// Debug
DebugAccountRange RPCImpl::debug_accountRange(
    VALUE const& _blockNumber, VALUE const& _txIndex, FH32 const& _address, size_t _maxResults)
{
    auto res = rpcCall("debug_accountRange",
        {quote(_blockNumber.asDecString()), _txIndex.asDecString(), quote(_address.asString()), fto_string(_maxResults)});
    return DebugAccountRange(res.getCContent());
}

DebugAccountRange RPCImpl::debug_accountRange(
    FH32 const& _blockHash, VALUE const& _txIndex, FH32 const& _address, size_t _maxResults)
{
    auto res = rpcCall("debug_accountRange",
        {quote(_blockHash.asString()), _txIndex.asDecString(), quote(_address.asString()), fto_string(_maxResults)});
    return DebugAccountRange(res.getCContent());
}

DebugStorageRangeAt RPCImpl::debug_storageRangeAt(
    VALUE const& _blockNumber, VALUE const& _txIndex, FH20 const& _address, FH32 const& _begin, int _maxResults)
{
    auto res = rpcCall("debug_storageRangeAt", {quote(_blockNumber.asDecString()), _txIndex.asDecString(),
                                               quote(_address.asString()), quote(_begin.asString()), fto_string(_maxResults)});
    return DebugStorageRangeAt(res.getCContent());
}

DebugStorageRangeAt RPCImpl::debug_storageRangeAt(
    FH32 const& _blockHash, VALUE const& _txIndex, FH20 const& _address, FH32 const& _begin, int _maxResults)
{
    auto res = rpcCall("debug_storageRangeAt", {quote(_blockHash.asString()), _txIndex.asDecString(), quote(_address.asString()),
                                                quote(_begin.asString()), fto_string(_maxResults)});
    return DebugStorageRangeAt(res.getCContent());
}

DebugVMTrace RPCImpl::debug_traceTransaction(FH32 const& _trHash)
{
    (void)_trHash;
    ETH_FAIL_MESSAGE("RPCImpl::debug_traceTransaction is not implemented!");
    static DebugVMTrace empty("", "", FH32::zero(), "");
    return empty;
}

// Test
void RPCImpl::test_setChainParams(spSetChainParamsArgs const& _config)
{
    RPCSession::currentCfgCountTestRun();

    ClientConfig const& cfg = Options::getCurrentConfig();
    spDataObject data = _config->asDataObject();
    cfg.performFieldReplace(*data, FieldReplaceDir::RetestethToClient);

    spDataObject res =  rpcCall("test_setChainParams", {data->asJson()});
    ETH_ERROR_REQUIRE_MESSAGE(*res == true, "remote test_setChainParams = false");
}

void RPCImpl::test_rewindToBlock(VALUE const& _blockNr)
{
    spDataObject res = rpcCall("test_rewindToBlock", {_blockNr.asDecString()});
    ETH_FAIL_REQUIRE_MESSAGE(*res == true, "remote test_rewintToBlock = false");
}

void RPCImpl::test_modifyTimestamp(VALUE const& _timestamp)
{
    spDataObject res = rpcCall("test_modifyTimestamp", {_timestamp.asDecString()});
    ETH_FAIL_REQUIRE_MESSAGE(*res == true, "test_modifyTimestamp was not successfull");
}

MineBlocksResult RPCImpl::test_mineBlocks(size_t _number)
{
    spDataObject const res = rpcCall("test_mineBlocks", {to_string(_number)}, true);

    if (res->type() == DataType::Object)
    {
        auto const& result = res->atKey("result");
        bool miningres = result.type() == DataType::Bool ? result.asBool() : result.asInt() == 1;
        ETH_ERROR_REQUIRE_MESSAGE(miningres == true, "remote test_mineBlocks = false");
    }
    else if (res->type() == DataType::Bool)
    {
        ETH_ERROR_REQUIRE_MESSAGE(res->asBool() == true, "remote test_mineBlocks = false");
    }
    else
        ETH_ERROR_MESSAGE("remote test_mineBlocks = " + res->asJson());

    return MineBlocksResult(res);
}

FH32 RPCImpl::test_importRawBlock(BYTES const& _blockRLP)
{
    spDataObject const res = rpcCall("test_importRawBlock", {quote(_blockRLP.asString())}, true);
    if (res->type() == DataType::String && res->asString().size() > 2)
        return FH32(res->asString());
    return FH32(FH32::zero());
}

FH32 RPCImpl::test_getLogHash(FH32 const& _txHash)
{
    return FH32(rpcCall("test_getLogHash", {quote(_txHash.asString())}));
}

TestRawTransaction RPCImpl::test_rawTransaction(BYTES const& _rlp, FORK const& _fork)
{
    spDataObject const res = rpcCall("test_rawTransaction", {quote(_rlp.asString()), quote(_fork.asString())});
    return TestRawTransaction(res);
}

VALUE RPCImpl::test_calculateDifficulty(FORK const& _fork, VALUE const& _blockNumber, VALUE const& _parentTimestamp,
    VALUE const& _parentDifficulty, VALUE const& _currentTimestamp, VALUE const& _uncleNumber)
{
    return VALUE(rpcCall("test_calculateDifficulty", {
             quote(_fork.asString()),
             quote(_blockNumber.asString()),
             quote(_parentTimestamp.asString()),
             quote(_parentDifficulty.asString()),
             quote(_currentTimestamp.asString()),
             quote(_uncleNumber.asString())
            })
        );
}

// Internal
std::string RPCImpl::sendRawRequest(std::string const& _request)
{
    JsonObjectValidator validator;
    return m_socket.sendRequest(_request, validator);
}

spDataObject RPCImpl::rpcCall(
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

    ETH_DC_MESSAGE(DC::RPC, "Request: " + request);
    JsonObjectValidator validator;  // read response while counting `{}`
    string reply = m_socket.sendRequest(request, validator);
    ETH_DC_MESSAGE(DC::RPC, "Reply: `" + reply + "`");

    spDataObject result = ConvertJsoncppStringToData(reply, string(), false);
    if (result->count("error"))
        (*result)["result"] = "";

    if (!ExitHandler::receivedExitSignal())
    {
        REQUIRE_JSONFIELDS(result, "rpcCall_response (req: '" + request.substr(0, 70) + "')",
            {{"jsonrpc", {{DataType::String}, jsonField::Required}},
             {"id", {{DataType::Integer}, jsonField::Required}},
             {"result", {{DataType::String, DataType::Integer,
                          DataType::Bool, DataType::Object, DataType::Array},
                        jsonField::Required}},
             {"error", {{DataType::String, DataType::Object}, jsonField::Optional}}});
    }
    else
    {
        (*result).clear();
        (*result)["error"]["message"] = "Received Exit Signal";
    }

    if (result->count("error"))
    {
        test::TestOutputHelper const& helper = test::TestOutputHelper::get();
        string const message = "Error on JSON-RPC call (" + helper.testInfo().errorDebug() + "):\nRequest: '" + request + "'" +
                               "\nResult: '" + (*result)["error"]["message"].asString() + "'\n";
        m_lastInterfaceError = RPCError((*result)["error"]["message"].asString(), message);

        if (_canFail)
            return spDataObject(new DataObject(DataType::Null));

        if (!ExitHandler::receivedExitSignal())
            ETH_FAIL_MESSAGE(m_lastInterfaceError.message());
    }
    m_lastInterfaceError.clear();  // null the error as last RPC call was success.
    return result.getContent().atKeyPointer("result");
}

Socket::SocketType RPCImpl::getSocketType() const
{
    return m_socket.type();
}

std::string const& RPCImpl::getSocketPath() const
{
    return m_socket.path();
}
