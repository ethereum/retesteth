#pragma once
#include <retesteth/ethObjects/common.h>
#include <retesteth/session/SessionInterface.h>
#include <retesteth/session/Socket.h>
#include <string>

class RPCImpl : public SessionInterface
{
public:
    RPCImpl(Socket::SocketType _type, const string& _path) : m_socket(_type, _path) {}

public:
    std::string web3_clientVersion() override;

    // ETH Methods
    std::string eth_sendRawTransaction(std::string const& _rlp) override;
    std::string eth_sendTransaction(std::string const& _transaction) override;
    int eth_getTransactionCount(
        std::string const& _address, std::string const& _blockNumber) override;
    test::scheme_transactionReceipt eth_getTransactionReceipt(
        std::string const& _transactionHash) override;

    std::string eth_blockNumber() override;
    test::scheme_block eth_getBlockByHash(string const& _hash, bool _fullObjects) override;
    test::scheme_block eth_getBlockByNumber(
        BlockNumber const& _blockNumber, bool _fullObjects) override;

    std::string eth_getCode(std::string const& _address, std::string const& _blockNumber) override;
    std::string eth_getBalance(
        std::string const& _address, std::string const& _blockNumber) override;

    // Debug
    scheme_debugAccountRange debug_accountRange(std::string const& _blockHashOrNumber, int _txIndex,
        std::string const& _address, int _maxResults) override;
    DataObject debug_storageRangeAt(std::string const& _blockHashOrNumber, int _txIndex,
        std::string const& _address, std::string const& _begin, int _maxResults) override;
    scheme_debugTraceTransaction debug_traceTransaction(std::string const& _trHash) override;

    // Test
    void test_setChainParams(std::string const& _config) override;
    void test_rewindToBlock(size_t _blockNr) override;
    void test_modifyTimestamp(unsigned long long _timestamp) override;
    string test_mineBlocks(int _number, bool _canFail = false) override;
    string test_importRawBlock(std::string const& _blockRLP) override;

    // ?? Deprecated ??
    std::string test_getBlockStatus(std::string const& _blockHash) override;
    std::string test_getLogHash(std::string const& _txHash) override;

    // ?? Deprecated ??
    std::string eth_getStorageRoot(
        std::string const& _address, std::string const& _blockNumber) override;
    std::string eth_getStorageAt(std::string const& _address, std::string const& _position,
        std::string const& _blockNumber) override;
    std::string personal_newAccount(std::string const& _password) override;
    void personal_unlockAccount(
        std::string const& _address, std::string const& _password, int _duration) override;

    // Internal
    std::string sendRawRequest(std::string const& _request) override;
    DataObject rpcCall(std::string const& _methodName,
        std::vector<std::string> const& _args = std::vector<std::string>(),
        bool _canFail = false) override;
    Socket::SocketType getSocketType() const;
    std::string const& getSocketPath() const;

private:
    Socket m_socket;
    size_t m_rpcSequence = 1;
    unsigned m_sleepTime = 10;  // 10 milliseconds
    unsigned m_successfulMineRuns = 0;
    unsigned m_maxMiningTime = 250000;  // should be instant with --test (1 sec)
};
