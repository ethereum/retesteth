#pragma once
#include <retesteth/ethObjects/common.h>
#include <string>

class SessionInterface
{
public:
    virtual std::string web3_clientVersion() = 0;

    // ETH Methods
    virtual std::string eth_sendRawTransaction(std::string const& _rlp) = 0;
    virtual std::string eth_sendTransaction(std::string const& _transaction) = 0;
    virtual int eth_getTransactionCount(
        std::string const& _address, std::string const& _blockNumber) = 0;
    virtual test::scheme_transactionReceipt eth_getTransactionReceipt(
        std::string const& _transactionHash) = 0;

    virtual std::string eth_blockNumber() = 0;
    virtual test::scheme_block eth_getBlockByHash(string const& _hash, bool _fullObjects) = 0;
    virtual test::scheme_block eth_getBlockByNumber(
        BlockNumber const& _blockNumber, bool _fullObjects) = 0;
    virtual std::string eth_getCode(
        std::string const& _address, std::string const& _blockNumber) = 0;
    virtual std::string eth_getBalance(
        std::string const& _address, std::string const& _blockNumber) = 0;

    // Debug
    virtual scheme_debugAccountRange debug_accountRange(std::string const& _blockHashOrNumber,
        int _txIndex, std::string const& _address, int _maxResults) = 0;
    virtual DataObject debug_storageRangeAt(std::string const& _blockHashOrNumber, int _txIndex,
        std::string const& _address, std::string const& _begin, int _maxResults) = 0;
    virtual scheme_debugTraceTransaction debug_traceTransaction(std::string const& _trHash) = 0;

    // Test
    virtual void test_setChainParams(std::string const& _config) = 0;
    virtual void test_rewindToBlock(size_t _blockNr) = 0;
    virtual void test_modifyTimestamp(unsigned long long _timestamp) = 0;
    virtual string test_mineBlocks(int _number, bool _canFail = false) = 0;
    virtual string test_importRawBlock(std::string const& _blockRLP) = 0;
    virtual std::string test_getLogHash(std::string const& _txHash) = 0;

    // Internal
    virtual std::string sendRawRequest(std::string const& _request) = 0;
    virtual DataObject rpcCall(std::string const& _methodName,
        std::vector<std::string> const& _args = std::vector<std::string>(),
        bool _canFail = false) = 0;

    virtual Socket::SocketType getSocketType() const = 0;
    virtual std::string const& getSocketPath() const = 0;

    string const& getLastRPCErrorMessage() const
    {
        /// Returns empty string if last RPC call had no errors, error string if there was an error
        static string const empty;
        return (m_lastInterfaceError.type() != DataType::Null) ?
                   m_lastInterfaceError.atKey("error").asString() :
                   empty;
    }
    DataObject const& getLastRPCError() const { return m_lastInterfaceError; }
    virtual ~SessionInterface() {}

protected:
    inline std::string quote(std::string const& _arg) { return "\"" + _arg + "\""; }
    DataObject m_lastInterfaceError;  // last RPC error info
};
