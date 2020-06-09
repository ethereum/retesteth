#pragma once
#include "ToolBackend/ToolChainManager.h"
#include <retesteth/TestHelper.h>
#include <retesteth/session/SessionInterface.h>
#include <retesteth/session/Socket.h>
#include <string>
using namespace toolimpl;

class ToolImpl : public SessionInterface
{
public:
    ToolImpl(Socket::SocketType _type, fs::path const& _path) : m_sockType(_type), m_toolPath(_path) {}

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
    void test_mineBlocks(int _number) override;
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
    Socket::SocketType m_sockType;
    fs::path m_toolPath;
    size_t m_totalCalls = 0;

    // Manage blockchains as ethereum client backend
    GCP_SPointer<ToolChainManager> m_toolChainManager;

private:
    /*
private:
    class ToolBlock
    {
    public:
        ToolBlock(scheme_RPCBlock const& _rpcBlockResponse, DataObject const& _chainParams,
            DataObject const& _pstate)
          : m_blockResponse(_rpcBlockResponse), m_env(_chainParams), m_postState(_pstate)
        {}
        scheme_RPCBlock const& getRPCResponse() const { return m_blockResponse; }
        void overwriteBlockHeader(DataObject const& _header)
        {
            m_blockResponse.overwriteBlockHeader(_header);
        }
        DataObject const& getEnv() const { return m_env; }
        DataObject const& getPostState() const { return m_postState; }
        string const& getHash() const { return m_blockResponse.getBlockHash(); }
        int getNumber() const { return test::hexOrDecStringToInt(m_blockResponse.getNumber()); }
        void markInvalidTransactions() { m_wereInvalidTr = true; }
        bool wereInvalidTransactions() const { return m_wereInvalidTr; }

    private:
        scheme_RPCBlock m_blockResponse;
        DataObject m_env;
        DataObject m_postState;
        bool m_wereInvalidTr = false;
    };

private:
    Socket::SocketType m_sockType;
    string m_toolPath;

    // Helper functions
    string prepareAllocForTool() const;
    string prepareTxsForTool() const;
    string prepareEnvForTool() const;
    ToolBlock const& getBlockByHashOrNumber(string const&) const;
    void verifyRawBlock(toolimpl::BlockHeadFromRLP const&, dev::RLP const&);

    // Construct RPC like block response
    struct BlockHeaderOverride;
    scheme_RPCBlock internalConstructResponseGetBlockByHashOrNumber(
        DataObject const& _toolResponse);

    // Core blockchain logic
    fs::path m_tmpDir;
    DataObject m_chainParams;
    std::vector<ToolBlock> m_chainGenesis;  // vector so not to init ToolBlock

    typedef std::vector<ToolBlock> ToolChain;  // tool blockchain of tool blocks
    ToolChain const& getCurrChain() const { return m_blockchainMap.at(m_current_chain_ind); }
    void makeForkForBlockWithPHash(string const& _parentHash);
    void doChainReorg();
    void onError(string const&, string const&);

    ToolBlock const& getGenesis() const { return m_chainGenesis.at(0); }
    ToolBlock const& getLastBlock(size_t _stepsBack = 1) const
    {
        // 1 - last block  2 - previous block
        ToolChain const& tch = m_blockchainMap.at(m_current_chain_ind);
        if (tch.size() >= _stepsBack)
            return tch.at(tch.size() - _stepsBack);
        return m_chainGenesis.at(0);
    }
    std::map<size_t, ToolChain> m_blockchainMap;  // all blockchains we know
    size_t m_current_chain_ind = 0;               // max total difficulty blockchain
    size_t m_maxChainID = 0;

    // std::vector<ToolBlock> m_blockchain;
    std::list<scheme_transaction> m_transactions;

    // Internal hack-logic to tell test_mineBlocks which block number is mined without passing the
    // params because test_mineBlocks is interface function. if expectedBlockNumber == -1 then use
    // default logic
    struct BlockHeaderOverride
    {
        bool isMiningGenesis = false;  // Is asking tool to calculate genesis hashes
        bool isImportRawBlock = false;
        DataObject header;
        int currentBlockNumber = -1;   // The number of cuurent block
        unsigned long long timestamp;  // Timestamp distance between blocks
        std::vector<scheme_RPCBlock> uncles;  // Full RPCBlock because we need to calculate host
                                              // block unclehash
        void reset()
        {
            uncles.clear();
            header.clear();
            currentBlockNumber = -1;
            isMiningGenesis = false;
            isImportRawBlock = false;
        }
    };
    BlockHeaderOverride m_currentBlockHeader;*/
};
