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
        ToolBlock(DataObject const& _toolResponse, DataObject const& _chainParams,
            DataObject const& _pstate)
          : m_toolresponse(_toolResponse), m_env(_chainParams), m_postState(_pstate)
        {}
        DataObject const& getToolResponse() const { return m_toolresponse.getData(); }
        DataObject const& getEnv() const { return m_env; }
        DataObject const& getPostState() const { return m_postState; }

    private:
        scheme_toolResponse m_toolresponse;
        DataObject m_env;
        DataObject m_postState;
    };
    ToolBlock const& getBlockByHashOrNumber(string const& _hashOrNumber) const;

private:
    Socket::SocketType m_sockType;
    string m_toolPath;

    size_t m_rpcSequence = 1;
    unsigned m_sleepTime = 10;  // 10 milliseconds
    unsigned m_successfulMineRuns = 0;
    unsigned m_maxMiningTime = 250000;  // should be instant with --test (1 sec)

    fs::path m_tmpDir;
    DataObject m_chainParams;
    unsigned long long m_timestamp;
    std::list<scheme_transaction> m_transactions;
    string getGenesisForTool(DataObject const& _genesis) const;
    string getTxsForTool() const;

    std::vector<ToolBlock> m_blockchain;
};
