#pragma once
#include <retesteth/TestHelper.h>
#include <retesteth/ethObjects/common.h>
#include <retesteth/session/SessionInterface.h>
#include <retesteth/session/Socket.h>
#include <string>

class ToolImpl : public SessionInterface
{
public:
    ToolImpl(Socket::SocketType _type, const string& _path) : m_sockType(_type), m_toolPath(_path)
    {
        m_tmpDir = test::createUniqueTmpDirectory();
    }

public:
    std::string web3_clientVersion() override;

    // ETH Methods
    std::string eth_sendRawTransaction(scheme_transaction const& _transaction) override;
    int eth_getTransactionCount(
        std::string const& _address, std::string const& _blockNumber) override;
    std::string eth_blockNumber() override;
    test::scheme_RPCBlock eth_getBlockByHash(string const& _hash, bool _fullObjects) override;
    test::scheme_RPCBlock eth_getBlockByNumber(
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
    void test_setChainParams(DataObject const& _config) override;
    void test_rewindToBlock(size_t _blockNr) override;
    void test_modifyTimestamp(unsigned long long _timestamp) override;
    string test_mineBlocks(int _number, bool _canFail = false) override;
    string test_importRawBlock(std::string const& _blockRLP) override;
    std::string test_getLogHash(std::string const& _txHash) override;

    // Internal
    DataObject rpcCall(std::string const& _methodName,
        std::vector<std::string> const& _args = std::vector<std::string>(),
        bool _canFail = false) override;
    Socket::SocketType getSocketType() const override;
    std::string const& getSocketPath() const override;

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
    ToolBlock const& getBlockByHashOrNumber(string const& _hashOrNumber) const;
    void verifyRawBlock(dev::RLP const& _blockRLP);

    // Construct RPC like block response
    struct BlockHeaderOverride;
    scheme_RPCBlock internalConstructResponseGetBlockByHashOrNumber(
        DataObject const& _toolResponse);

    // Core blockchain logic
    size_t m_totalCalls = 0;
    fs::path m_tmpDir;
    DataObject m_chainParams;
    std::vector<ToolBlock> m_chainGenesis;  // vector so not to init ToolBlock

    typedef std::vector<ToolBlock> ToolChain;  // tool blockchain of tool blocks
    ToolChain const& getCurrChain() const { return m_blockchainMap.at(m_current_chain_ind); }
    void makeForkForBlockWithPHash(string const& _parentHash);
    void doChainReorg();

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
    BlockHeaderOverride m_currentBlockHeader;
};
