#pragma once
#include "Socket.h"
#include <retesteth/dataObject/DataObject.h>
#include <retesteth/testStructures/basetypes.h>
#include <retesteth/testStructures/types/rpc.h>

#include <string>

using namespace dataobject;
using namespace test::teststruct;

enum class Request
{
    FULLOBJECTS,
    LESSOBJECTS
};

struct RPCError
{
    RPCError() : m_empty(true) { clear(); };
    RPCError(string const& _error, string const& _message) : m_error(_error), m_message(_message), m_empty(false){};
    string const& error() const { return m_error; }
    string const& message() const { return m_message; }
    bool empty() const { return m_empty; }
    void clear()
    {
        m_empty = true;
        m_message = "No error message";
        m_error = "No error";
    }

private:
    string m_error;
    string m_message;
    bool m_empty;
};

class SessionInterface
{
public:
    // DataObject represents json output
    virtual DataObject web3_clientVersion() = 0;

    // ETH Methods
    virtual FH32 eth_sendRawTransaction(BYTES const& _rlp) = 0;
    virtual size_t eth_getTransactionCount(FH20 const& _address, VALUE const& _blockNumber) = 0;

    virtual VALUE eth_blockNumber() = 0;
    virtual EthGetBlockBy eth_getBlockByHash(FH32 const& _blockHash, Request _fullObjects) = 0;
    virtual EthGetBlockBy eth_getBlockByNumber(VALUE const& _blockNumber, Request _fullObjects) = 0;
    virtual BYTES eth_getCode(FH20 const& _address, VALUE const& _blockNumber) = 0;
    virtual VALUE eth_getBalance(FH20 const& _address, VALUE const& _blockNumber) = 0;

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
    virtual void test_setChainParams(SetChainParamsArgs const& _config) = 0;
    virtual void test_rewindToBlock(VALUE const& _blockNr) = 0;
    virtual void test_modifyTimestamp(VALUE const& _timestamp) = 0;
    virtual void test_mineBlocks(size_t _number) = 0;
    virtual FH32 test_importRawBlock(BYTES const& _blockRLP) = 0;
    virtual FH32 test_getLogHash(FH32 const& _txHash) = 0;

    // Internal
    virtual DataObject rpcCall(std::string const& _methodName,
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
