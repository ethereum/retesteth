#pragma once
#include "Socket.h"
#include <libdataobj/DataObject.h>
#include <retesteth/testStructures/basetypes.h>
#include <retesteth/testStructures/types/rpc.h>
#include <string>

namespace test::session
{
enum class Request
{
    FULLOBJECTS,
    LESSOBJECTS
};

struct RPCError
{
    RPCError() : m_empty(true) { clear(); };
    RPCError(std::string const& _error, std::string const& _message) : m_error(_error), m_message(_message), m_empty(false){};
    std::string const& error() const { return m_error; }
    std::string const& message() const { return m_message; }
    bool empty() const { return m_empty; }
    void clear()
    {
        m_empty = true;
        m_message = "No error message";
        m_error = "No error";
    }

private:
    std::string m_error;
    std::string m_message;
    bool m_empty;
};

using namespace dataobject;
using namespace test::teststruct;

class SessionInterface
{
public:
    // DataObject represents json output
    virtual spDataObject web3_clientVersion() = 0;

    // ETH Methods
    virtual FH32 eth_sendRawTransaction(BYTES const& _rlp, VALUE const& _secret) = 0;

    virtual VALUE eth_blockNumber() = 0;
    virtual EthGetBlockBy eth_getBlockByHash(FH32 const& _blockHash, Request _fullObjects) = 0;
    virtual EthGetBlockBy eth_getBlockByNumber(VALUE const& _blockNumber, Request _fullObjects) = 0;

    // Account functions
    virtual spBYTES eth_getCode(FH20 const& _address, VALUE const& _blockNumber) = 0;
    virtual spVALUE eth_getBalance(FH20 const& _address, VALUE const& _blockNumber) = 0;
    virtual spVALUE eth_getTransactionCount(FH20 const& _address, VALUE const& _blockNumber) = 0;

    // Debug
    virtual DebugAccountRange debug_accountRange(
        VALUE const& _blockNumber, VALUE const& _txIndex, FH32 const& _addrHash, size_t _maxResults) = 0;
    virtual DebugAccountRange debug_accountRange(
        FH32 const& _blockHash, VALUE const& _txIndex, FH32 const& _addrHash, size_t _maxResults) = 0;
    virtual DebugStorageRangeAt debug_storageRangeAt(
        VALUE const& _blockNumber, VALUE const& _txIndex, FH20 const& _addrHash, FH32 const& _begin, int _maxResults) = 0;
    virtual DebugStorageRangeAt debug_storageRangeAt(
        FH32 const& _blockHash, VALUE const& _txIndex, FH20 const& _addrHash, FH32 const& _begin, int _maxResults) = 0;
    virtual DebugVMTrace debug_traceTransaction(FH32 const& _trHash) = 0;

    // Test
    virtual void test_setChainParams(spSetChainParamsArgs const& _config) = 0;
    virtual void test_rewindToBlock(VALUE const& _blockNr) = 0;
    virtual void test_modifyTimestamp(VALUE const& _timestamp) = 0;
    virtual MineBlocksResult test_mineBlocks(size_t _number) = 0;
    virtual FH32 test_importRawBlock(BYTES const& _blockRLP) = 0;
    virtual FH32 test_getLogHash(FH32 const& _txHash) = 0;
    virtual TestRawTransaction test_rawTransaction(BYTES const& _rlp, FORK const& _fork) = 0;
    virtual VALUE test_calculateDifficulty(FORK const& _fork, VALUE const& _blockNumber, VALUE const& _parentTimestamp,
        VALUE const& _parentDifficulty, VALUE const& _currentTimestamp, VALUE const& _uncleNumber) = 0;

    // Internal
    virtual spDataObject rpcCall(std::string const& _methodName,
        std::vector<std::string> const& _args = std::vector<std::string>(),
        bool _canFail = false) = 0;
    virtual Socket::SocketType getSocketType() const = 0;
    virtual std::string const& getSocketPath() const = 0;

    RPCError const& getLastRPCError() const { return m_lastInterfaceError; }
    virtual ~SessionInterface() {}

protected:
    inline std::string quote(std::string const& _arg) { return "\"" + _arg + "\""; }
    RPCError m_lastInterfaceError;
};

}  // namespace test::session
