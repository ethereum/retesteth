#pragma once
#include "ToolBackend/ToolChainManager.h"
#include <retesteth/session/SessionInterface.h>
#include <retesteth/session/Socket.h>
#include <string>

namespace test::session
{
using namespace dataobject;

class ToolImpl : public SessionInterface
{
public:
    ToolImpl(Socket::SocketType _type, fs::path const& _path, fs::path const& _tmpDir)
      : m_sockType(_type), m_toolPath(_path), m_tmpDir(_tmpDir)
    {}

public:
    spDataObject web3_clientVersion() override;

    // ETH Methods
    FH32 eth_sendRawTransaction(BYTES const& _rlp, VALUE const& _secret) override;
    VALUE eth_blockNumber() override;
    EthGetBlockBy eth_getBlockByHash(FH32 const& _hash, Request _fullObjects) override;
    EthGetBlockBy eth_getBlockByNumber(VALUE const& _blockNumber, Request _fullObjects) override;

    // Account functions
    spVALUE eth_getTransactionCount(FH20 const& _address, VALUE const& _blockNumber) override;
    spBYTES eth_getCode(FH20 const& _address, VALUE const& _blockNumber) override;
    spVALUE eth_getBalance(FH20 const& _address, VALUE const& _blockNumber) override;

    // Debug
    DebugAccountRange debug_accountRange(
        VALUE const& _blockNumber, VALUE const& _txIndex, FH32 const& _addrHash, size_t _maxResults) override;
    DebugAccountRange debug_accountRange(
        FH32 const& _blockHash, VALUE const& _txIndex, FH32 const& _address, size_t _maxResults) override;
    DebugStorageRangeAt debug_storageRangeAt(
        VALUE const& _blockNumber, VALUE const& _txIndex, FH20 const& _addrHash, FH32 const& _begin, int _maxResults) override;
    DebugStorageRangeAt debug_storageRangeAt(
        FH32 const& _blockHash, VALUE const& _txIndex, FH20 const& _address, FH32 const& _begin, int _maxResults) override;
    DebugVMTrace debug_traceTransaction(FH32 const& _trHash) override;

    // Test
    void test_setChainParams(spSetChainParamsArgs const& _config) override;
    void test_rewindToBlock(VALUE const& _blockNr) override;
    void test_modifyTimestamp(VALUE const& _timestamp) override;
    MineBlocksResult test_mineBlocks(size_t _number) override;
    FH32 test_importRawBlock(BYTES const& _blockRLP) override;
    FH32 test_getLogHash(FH32 const& _txHash) override;
    TestRawTransaction test_rawTransaction(BYTES const& _rlp, FORK const& _fork) override;
    VALUE test_calculateDifficulty(FORK const& _fork, VALUE const& _blockNumber, VALUE const& _parentTimestamp,
        VALUE const& _parentDifficulty, VALUE const& _currentTimestamp, VALUE const& _uncleNumber) override;

    // Internal
    std::string sendRawRequest(std::string const& _request);
    spDataObject rpcCall(std::string const& _methodName,
        std::vector<std::string> const& _args = std::vector<std::string>(),
        bool _canFail = false) override;
    Socket::SocketType getSocketType() const override;
    std::string const& getSocketPath() const override;

private:
    Socket::SocketType m_sockType;
    fs::path m_toolPath;
    fs::path m_tmpDir;
    size_t m_totalCalls = 0;
    toolimpl::ToolChainManager& blockchain() { return m_toolChainManager.getContent(); }
    void makeRPCError(std::string const& _error);

    // Manage blockchains as ethereum client backend
    GCP_SPointer<toolimpl::ToolChainManager> m_toolChainManager;
};

}  // namespace test::session
