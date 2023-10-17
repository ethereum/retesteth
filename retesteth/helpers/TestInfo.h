#pragma once
#include <string>

namespace test
{

struct TestInfo
{
    TestInfo(std::string const& _fork, int _trD, int _trG, int _trV);
    TestInfo(std::string const& _fork, size_t _trD, size_t _trG, size_t _trV);
    TestInfo(std::string const& _fork, size_t _block, std::string const& _chainName = std::string());
    TestInfo(std::string const& _info, std::string const& _testName = std::string());

    TestInfo(): m_isStateTransactionInfo(false), m_isBlockchainTestInfo(false) {}
    std::string errorDebug() const;
    static std::string caseName();
    void setTrDataDebug(std::string const& _data) { m_sTransactionData = _data; }

private:
    std::string makeTestCaseName() const;

private:
    std::string m_sFork, m_sChainName;
    std::string m_currentTestCaseName;
    std::string m_sTransactionData;

    int m_trD, m_trG, m_trV;
    size_t m_blockNumber;
    bool m_isStateTransactionInfo = false;
    bool m_isBlockchainTestInfo = false;
    bool m_isGeneralTestInfo = false;
};
}
