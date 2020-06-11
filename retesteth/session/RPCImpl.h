#pragma once
#include <retesteth/session/SessionInterface.h>
#include <retesteth/session/Socket.h>
#include <string>

class RPCImpl : public SessionInterface
{
public:
    RPCImpl(Socket::SocketType _type, const string& _path) : m_socket(_type, _path) {}

public:
    DataObject web3_clientVersion() override;

    // ETH Methods
    FH32 eth_sendRawTransaction(BYTES const& _rlp) override;
    int eth_getTransactionCount(FH20 const& _address, VALUE const& _blockNumber) override;
    VALUE eth_blockNumber() override;
    EthGetBlockBy eth_getBlockByHash(FH32 const& _hash, Request _fullObjects) override;
    EthGetBlockBy eth_getBlockByNumber(VALUE const& _blockNumber, Request _fullObjects) override;

    BYTES eth_getCode(FH20 const& _address, VALUE const& _blockNumber) override;
    VALUE eth_getBalance(FH20 const& _address, VALUE const& _blockNumber) override;

    // Debug
    DebugAccountRange debug_accountRange(
        VALUE const& _blockNumber, VALUE const& _txIndex, FH32 const& _addrHash, int _maxResults) override;
    DebugAccountRange debug_accountRange(
        FH32 const& _blockHash, VALUE const& _txIndex, FH32 const& _address, int _maxResults) override;
    DebugStorageRangeAt debug_storageRangeAt(
        VALUE const& _blockNumber, VALUE const& _txIndex, FH20 const& _addrHash, FH32 const& _begin, int _maxResults) override;
    DebugStorageRangeAt debug_storageRangeAt(
        FH32 const& _blockHash, VALUE const& _txIndex, FH20 const& _address, FH32 const& _begin, int _maxResults) override;
    DebugTraceTransaction debug_traceTransaction(FH32 const& _trHash) override;

    // Test
    void test_setChainParams(SetChainParamsArgs const& _config) override;
    void test_rewindToBlock(VALUE const& _blockNr) override;
    void test_modifyTimestamp(VALUE const& _timestamp) override;
    void test_mineBlocks(size_t _number) override;
    FH32 test_importRawBlock(BYTES const& _blockRLP) override;
    FH32 test_getLogHash(FH32 const& _txHash) override;

    // Internal
    std::string sendRawRequest(std::string const& _request);
    DataObject rpcCall(std::string const& _methodName,
        std::vector<std::string> const& _args = std::vector<std::string>(),
        bool _canFail = false) override;
    Socket::SocketType getSocketType() const override;
    std::string const& getSocketPath() const override;

private:
    Socket m_socket;
    size_t m_rpcSequence = 1;
};
