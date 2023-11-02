#include "ToolBackend/ToolImplHelper.h"
#include <retesteth/Options.h>
#include <retesteth/helpers/TestHelper.h>
#include <retesteth/session/ToolImpl.h>

using namespace test;
using namespace test::debug;
using namespace toolimpl;
using namespace std;

namespace test::session
{
/// Because ToolBackend is like a light client module of retesteth
/// Execute its logic in try catch blocks. treating any std::exceptions as its failure
/// And treating any Eth exceptions as a message to retesteth behaviour

enum class CallType
{
    DONTFAILONUPWARDS,
    FAILEVERYTHING
};
#define TRYCATCHCALL(X, method, ctype, rpclog)                                                             \
    try {                                                                                                  \
        X                                                                                                  \
    }                                                                                                      \
    catch (UpwardsException const& _ex)                                                                    \
    {                                                                                                      \
        makeRPCError(_ex.what());                                                                          \
        ETH_DC_MESSAGE(rpclog, string("Response ") + method + ": " + _ex.what());                          \
        if (ctype != CallType::DONTFAILONUPWARDS)                                                          \
        {                                                                                                  \
            bool const allowErrors = Options::getCurrentConfig().cfgFile().continueOnErrors();             \
            bool const exit512 = string(_ex.what()).find("exited with 512 code") != string::npos;          \
            if (allowErrors || exit512)                                                                    \
                { ETH_ERROR_MESSAGE(_ex.what());}                                                          \
            else                                                                                           \
                { ETH_FAIL_MESSAGE(_ex.what()); }                                                          \
        }                                                                                                  \
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
    ETH_DC_MESSAGE(DC::RPC2, "\nRequest: web3_clientVersion");
    string const cmd = m_toolPath.string() + " -v";
    TRYCATCHCALL(
                int exitCode;
                spDataObject res(new DataObject(test::executeCmd(cmd, exitCode)));
                ETH_DC_MESSAGE(DC::RPC2, "Response: web3_clientVersion " + res->asString());
                size_t const pos = res->asString().find("\n");
                if (pos != string::npos)
                    (*res).asStringUnsafe() = res->asString().substr(0, pos);
                return res;
        , "web3_clientVersion", CallType::FAILEVERYTHING, DC::RPC2)
    return spDataObject();
}

// ETH Methods
FH32 ToolImpl::eth_sendRawTransaction(BYTES const& _rlp, VALUE const& _secret)
{
    rpcCall("", {});
    ETH_DC_MESSAGE(DC::RPC, "\nRequest: eth_sendRawTransaction \n" + _rlp.asString());
    TRYCATCHCALL(
        spTransaction spTr = readTransaction(_rlp);
        spTr.getContent().setSecret(_secret);
        ETH_DC_MESSAGE(DC::RPC, spTr->asDataObject()->asJson());
        m_toolChainManager.getContent().addPendingTransaction(spTr);
        FH32 trHash = spTr.getContent().hash();
        ETH_DC_MESSAGE(DC::RPC, "Response: " + trHash.asString());
        return trHash;
        , "eth_sendRawTransaction", CallType::FAILEVERYTHING, DC::RPC)
    return FH32::zero();
}

spVALUE ToolImpl::eth_getTransactionCount(FH20 const& _address, VALUE const& _blockNumber)
{
    rpcCall("", {});
    ETH_DC_MESSAGE(DC::RPC2, "\nRequest: eth_getTransactionCount " + _blockNumber.asString() + " " + _address.asString());
    TRYCATCHCALL(
        spVALUE nonce (blockchain().blockByNumber(_blockNumber).state()->getAccount(_address).nonce().copy());
        ETH_DC_MESSAGE(DC::RPC2, "Response: eth_getTransactionCount " + nonce->asDecString());
        return nonce;
        , "eth_getTransactionCount", CallType::FAILEVERYTHING, DC::RPC2)
    return spVALUE(0);
}

VALUE ToolImpl::eth_blockNumber()
{
    rpcCall("", {});
    ETH_DC_MESSAGE(DC::RPC2, "\nRequest: eth_blockNumber");
    TRYCATCHCALL(
        VALUE const& number = m_toolChainManager.getContent().lastBlock().header()->number();
        string const snumber = number.asDecString();
        ETH_DC_MESSAGE(DC::RPC2, "Response: eth_blockNumber {" + snumber + "}");
        return number;
        , "eth_blockNumber", CallType::FAILEVERYTHING, DC::RPC2)
    return VALUE(0);
}


spEthGetBlockBy ToolImpl::eth_getBlockByHash(FH32 const& _hash, Request _fullObjects)
{
    rpcCall("", {});
    ETH_DC_MESSAGE(DC::RPC2, "\nRequest: eth_getBlockByHash `" + _hash.asString());
    TRYCATCHCALL(
        spDataObject res = constructEthGetBlockBy(blockchain().blockByHash(_hash), _fullObjects);
        ETH_DC_MESSAGE(DC::RPC2, "Response: eth_getBlockByHash `" + res->asJson());
        return spEthGetBlockBy(new EthGetBlockBy(res));
        , "eth_getBlockByHash", CallType::FAILEVERYTHING, DC::RPC2)
    return spEthGetBlockBy(0);
}

spEthGetBlockBy ToolImpl::eth_getBlockByNumber(VALUE const& _blockNumber, Request _fullObjects)
{
    rpcCall("", {});
    ETH_DC_MESSAGE(DC::RPC2, "\nRequest: eth_getBlockByNumber `" + _blockNumber.asDecString());
    TRYCATCHCALL(
        spDataObject res = constructEthGetBlockBy(blockchain().blockByNumber(_blockNumber), _fullObjects);
        ETH_DC_MESSAGE(DC::RPC2, "Response: eth_getBlockByNumber `" + res->asJson());
        return spEthGetBlockBy(new EthGetBlockBy(res));
        , "eth_getBlockByNumber", CallType::FAILEVERYTHING, DC::RPC2)
    return spEthGetBlockBy(0);
}

spBYTES ToolImpl::eth_getCode(FH20 const& _address, VALUE const& _blockNumber)
{
    rpcCall("", {});
    ETH_DC_MESSAGE(DC::RPC2, "\nRequest: eth_getCode " + _blockNumber.asString() + " " + _address.asString());
    TRYCATCHCALL(
        spBYTES code(blockchain().blockByNumber(_blockNumber).state()->getAccount(_address).code().copy());
        ETH_DC_MESSAGE(DC::RPC2, "Response: eth_getCode " + code->asString());
        return code;
        , "eth_getCode", CallType::FAILEVERYTHING, DC::RPC2)
    return spBYTES(0);
}

spVALUE ToolImpl::eth_getBalance(FH20 const& _address, VALUE const& _blockNumber)
{
    rpcCall("", {});
    ETH_DC_MESSAGE(DC::RPC2, "\nRequest: eth_getBalance " + _blockNumber.asString() + " " + _address.asString());
    TRYCATCHCALL(
        // Make a copy here because we do not expose the memory of the tool backend
        spVALUE balance(blockchain().blockByNumber(_blockNumber).state()->getAccount(_address).balance().copy());
        ETH_DC_MESSAGE(DC::RPC2, "Response: eth_getBalance " + balance->asDecString());
        return balance;
        , "eth_getBalance", CallType::FAILEVERYTHING, DC::RPC2)
    return spVALUE(0);
}

// Debug
DebugAccountRange ToolImpl::debug_accountRange(
    VALUE const& _blockNumber, VALUE const& _txIndex, FH32 const& _addressHash, size_t _maxResults)
{
    rpcCall("", {});
    ETH_DC_MESSAGE(DC::RPC2, "\nRequest: debug_accountRange `" + _blockNumber.asDecString() + " " + _addressHash.asString());
    (void) _txIndex;

    TRYCATCHCALL(
        spDataObject constructResponse =
            constructAccountRange(blockchain().blockByNumber(_blockNumber), _addressHash, _maxResults);
        ETH_DC_MESSAGE(DC::RPC2, "Response: debug_accountRange " + constructResponse->asJson());
        return DebugAccountRange(constructResponse);
        , "debug_accountRange", CallType::FAILEVERYTHING, DC::RPC2)
    return DebugAccountRange(DataObject());
}

DebugAccountRange ToolImpl::debug_accountRange(
    FH32 const& _blockHash, VALUE const& _txIndex, FH32 const& _addressHash, size_t _maxResults)
{
    rpcCall("", {});
    ETH_DC_MESSAGE(DC::RPC2, "\nRequest: debug_accountRange " + _blockHash.asString() + " " + _addressHash.asString());
    (void) _txIndex;

    TRYCATCHCALL(
        spDataObject constructResponse = constructAccountRange(blockchain().blockByHash(_blockHash), _addressHash, _maxResults);
        ETH_DC_MESSAGE(DC::RPC2, "Response: debug_accountRange " + constructResponse->asJson());
        return DebugAccountRange(constructResponse);
        , "debug_accountRange", CallType::FAILEVERYTHING, DC::RPC2)
    return DebugAccountRange(DataObject());
}

DebugStorageRangeAt ToolImpl::debug_storageRangeAt(
    VALUE const& _blockNumber, VALUE const& _txIndex, FH20 const& _address, FH32 const& _begin, int _maxResults)
{
    rpcCall("", {});
    ETH_DC_MESSAGE(DC::RPC2, "\nRequest: debug_storageRangeAt bl:" + _blockNumber.asDecString() + " ind:" + _begin.asString() +
                     " addr:" + _address.asString());
    (void) _txIndex;

    TRYCATCHCALL(
        spDataObject constructResponse =
            constructStorageRangeAt(blockchain().blockByNumber(_blockNumber), _address, _begin, _maxResults);
        ETH_DC_MESSAGE(DC::RPC2, "Response: debug_storageRangeAt " + constructResponse->asJson());
        return DebugStorageRangeAt(constructResponse);
        , "debug_storageRangeAt", CallType::FAILEVERYTHING, DC::RPC2)
    return DebugStorageRangeAt(DataObject());
}

DebugStorageRangeAt ToolImpl::debug_storageRangeAt(
    FH32 const& _blockHash, VALUE const& _txIndex, FH20 const& _address, FH32 const& _begin, int _maxResults)
{
    rpcCall("", {});
    ETH_DC_MESSAGE(DC::RPC2, "\nRequest: debug_storageRangeAt bl:" + _blockHash.asString() + " ind:" + _begin.asString() +
                     " addr:" + _address.asString());
    (void) _txIndex;

    TRYCATCHCALL(
        spDataObject constructResponse =
            constructStorageRangeAt(blockchain().blockByHash(_blockHash), _address, _begin, _maxResults);
        ETH_DC_MESSAGE(DC::RPC2, "Response: debug_storageRangeAt " + constructResponse->asJson());
        return DebugStorageRangeAt(constructResponse);
        , "debug_storageRangeAt", CallType::FAILEVERYTHING, DC::RPC2)
    return DebugStorageRangeAt(DataObject());
}

DebugVMTrace ToolImpl::debug_traceTransaction(FH32 const& _trHash)
{
    rpcCall("", {});
    ETH_DC_MESSAGE(DC::RPC2, "\nRequest: debug_traceTransaction: " + _trHash.asString());
    TRYCATCHCALL(
        return blockchain().lastBlock().getTrTrace(_trHash);
        , "debug_traceTransaction", CallType::FAILEVERYTHING, DC::RPC2)
    return DebugVMTrace();
}

// Test
void ToolImpl::test_setChainParams(spSetChainParamsArgs const& _config)
{
    rpcCall("", {});
    ETH_DC_MESSAGE(DC::RPC, "\nRequest: test_setChainParams \n" + _config->asDataObject()->asJson());

    // Ask tool to calculate genesis header stateRoot for genesisHeader
    TRYCATCHCALL(
        m_toolChainManager = GCP_SPointer<ToolChainManager>(new ToolChainManager(_config, m_toolPath, m_tmpDir));
        ETH_DC_MESSAGE(DC::RPC, "Response test_setChainParams: {true}");
        , "test_setChainParams", CallType::FAILEVERYTHING, DC::RPC)
    ETH_DC_MESSAGE(DC::RPC, "Response test_setChainParams: {false}");
}

void ToolImpl::test_setChainParamsNoGenesis(spSetChainParamsArgs const& _config)
{
    rpcCall("", {});
    ETH_DC_MESSAGE(DC::RPC, "\nRequest: test_setChainParams \n" + _config->asDataObject()->asJson());

    // Ask tool to calculate genesis header stateRoot for genesisHeader
    TRYCATCHCALL(
        m_toolChainManager = GCP_SPointer<ToolChainManager>(new ToolChainManager(_config, m_toolPath, m_tmpDir, ToolChainGenesis::NOTCALCULATE));
        ETH_DC_MESSAGE(DC::RPC, "Response test_setChainParams: {true}");
        , "test_setChainParams", CallType::FAILEVERYTHING, DC::RPC)
    ETH_DC_MESSAGE(DC::RPC, "Response test_setChainParams: {false}");
}

void ToolImpl::test_rewindToBlock(VALUE const& _blockNr)
{
    rpcCall("", {});
    ETH_DC_MESSAGE(DC::RPC, "\nRequest: test_rewindToBlock " + _blockNr.asDecString());
    TRYCATCHCALL(
        blockchain().rewindToBlock(_blockNr);
        , "test_rewindToBlock", CallType::DONTFAILONUPWARDS, DC::RPC)
    ETH_DC_MESSAGE(DC::RPC, "Response: test_rewindToBlock: " + blockchain().lastBlock().header()->number().asDecString());
}

void ToolImpl::test_modifyTimestamp(VALUE const& _timestamp)
{
    rpcCall("", {});
    ETH_DC_MESSAGE(DC::RPC2, "\nRequest: test_modifyTimestamp " + _timestamp.asDecString());
    TRYCATCHCALL(
        blockchain().modifyTimestamp(_timestamp);
        ETH_DC_MESSAGE(DC::RPC2, "Response: test_modifyTimestamp " + _timestamp.asDecString());
            , "test_modifyTimestamp", CallType::DONTFAILONUPWARDS, DC::RPC2)
}

MineBlocksResult ToolImpl::test_mineBlocks(size_t _number)
{
    rpcCall("", {});
    ETH_DC_MESSAGE(DC::RPC, "\nRequest: test_mineBlocks");
    TRYCATCHCALL(
        spDataObject const res = blockchain().mineBlocks(_number);
        ETH_DC_MESSAGE(DC::RPC, "Response test_mineBlocks {" + blockchain().lastBlock().header()->number().asDecString() + "}");
        ETH_DC_MESSAGE(DC::RPC, res->asJson());
        // test_mineBlocks assumed not to fail ever to be able to construct test block RLP
        // for invalid block generation test_importRawBlock is used which takes malicious RLP
        return MineBlocksResult(res);
            , "test_mineBlocks", CallType::FAILEVERYTHING, DC::RPC)
    return MineBlocksResult(DataObject());
}

// Import block from RAW rlp and validate it according to ethereum rules
// Very logic heavy function. Must be on the client side. Its a clien logic.
FH32 ToolImpl::test_importRawBlock(BYTES const& _blockRLP)
{
    rpcCall("", {});
    TRYCATCHCALL(
        ETH_DC_MESSAGE(DC::RPC, "\nRequest: test_importRawBlock, following transaction import are internal");
        ETH_DC_MESSAGE(DC::RPC, _blockRLP.asString());
        FH32 const hash = blockchain().importRawBlock(_blockRLP);
        ETH_DC_MESSAGE(DC::RPC, "Response test_importRawBlock: " + hash.asString());
        return hash;
        , "test_importRawBlock", CallType::DONTFAILONUPWARDS, DC::RPC)
    return FH32::zero();
}

FH32 ToolImpl::test_getLogHash(FH32 const& _txHash)
{
    rpcCall("", {});
    TRYCATCHCALL(
        ETH_DC_MESSAGE(DC::RPC2, "\nRequest: test_getLogHash " + _txHash.asString());
        FH32 const& res = blockchain().lastBlock().logHash();
        ETH_DC_MESSAGE(DC::RPC2, "Response: test_getLogHash " + res.asString());
        return res;
        , "test_getLogHash", CallType::FAILEVERYTHING, DC::RPC2)
    return _txHash;
}

TestRawTransaction ToolImpl::test_rawTransaction(BYTES const& _rlp, FORK const& _fork)
{
    auto const& genesisSetupInTool = Options::getCurrentConfig().getGenesisTemplate(_fork);
    FORK t8nForkName(genesisSetupInTool.getCContent().atKey("params").atKey("fork").asString());
    // TODO parametrize structure here. so not to get an error if params or fork dissappear

    rpcCall("", {});
    TRYCATCHCALL(
        ETH_DC_MESSAGE(DC::RPC, "\nRequest: test_rawTransaction '" + _rlp.asString() + "', Fork: `" + t8nForkName.asString());
        TestRawTransaction res = ToolChainManager::test_rawTransaction(_rlp, t8nForkName, m_toolPath, m_tmpDir);
        return res;
        , "test_rawTransaction", CallType::FAILEVERYTHING, DC::RPC)
    return TestRawTransaction(DataObject());
}

std::string ToolImpl::test_rawEOFCode(BYTES const& _code, FORK const& _fork)
{
    auto const& genesisSetupInTool = Options::getCurrentConfig().getGenesisTemplate(_fork);
    FORK t8nForkName(genesisSetupInTool.getCContent().atKey("params").atKey("fork").asString());

    rpcCall("", {});
    TRYCATCHCALL(
        ETH_DC_MESSAGE(DC::RPC, "\nRequest: test_rawEOFCode '" + _code.asString().substr(0, 50) + "', Fork: `" + t8nForkName.asString());
        string res = ToolChainManager::test_rawEOFCode(_code, t8nForkName, m_toolPath, m_tmpDir);
        return res;
        , "test_rawTransaction", CallType::DONTFAILONUPWARDS, DC::RPC)
    return string();
}

void ToolImpl::test_registerWithdrawal(BYTES const& _rlp)
{
    rpcCall("", {});
    TRYCATCHCALL(
        ETH_DC_MESSAGE(DC::RPC, "\nRequest: test_registerWithdrawal '" + _rlp.asString());
        m_toolChainManager.getContent().registerWithdrawal(_rlp);
        , "test_registerWithdrawal", CallType::FAILEVERYTHING, DC::RPC)
}


VALUE ToolImpl::test_calculateDifficulty(FORK const& _fork, VALUE const& _blockNumber, VALUE const& _parentTimestamp,
    VALUE const& _parentDifficulty, VALUE const& _currentTimestamp, VALUE const& _uncleNumber)
{
    rpcCall("", {});
    TRYCATCHCALL(
        ETH_DC_MESSAGE(DC::RPC, "\nRequest: test_calculateDifficulty '");
        ETH_DC_MESSAGE(DC::RPC, "Fork: " + _fork.asString() + ", bn: " + _blockNumber.asString() + ", pt: " + _parentTimestamp.asString() +
            ", pd: " + _parentDifficulty.asString() + ", ct: " + _currentTimestamp.asString() + ", un: " + _uncleNumber.asString());
        return ToolChainManager::test_calculateDifficulty(_fork, _blockNumber, _parentTimestamp, _parentDifficulty, _currentTimestamp, _uncleNumber,
            m_toolPath, m_tmpDir);
        , "test_calculateDifficulty", CallType::FAILEVERYTHING, DC::RPC)
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
    ETH_DC_MESSAGE(DC::LOWLOG, "makeRPCError " + _error);
    m_lastInterfaceError = RPCError("", _error);
}

}  // namespace test::session
