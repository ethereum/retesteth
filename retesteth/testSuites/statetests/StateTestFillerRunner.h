#pragma once
#include <retesteth/session/Session.h>
#include <retesteth/testStructures/structures.h>
#include <retesteth/testSuiteRunner/TestSuite.h>

namespace test::statetests
{

class StateTestFillerRunner
{
public:
    StateTestFillerRunner(StateTestInFiller const& _test, TestSuite::TestSuiteOptions const& _opt);
    std::vector<TransactionInGeneralSection>& txs() { return m_txs; }
    bool checkNetworkSkip(FORK const& _network);
    virtual void prepareChainParams(FORK const&);
    void setErrorInfo(TransactionInGeneralSection const& _tr, FORK const& _network);
    virtual void performTransactionOnExpect(TransactionInGeneralSection&, StateTestFillerExpectSection const&, FORK const&);
    virtual spDataObject getFilledTest() const;
    void registerForkResult();
    bool checkBigintSkip();
protected:
    StateTestFillerRunner(StateTestInFiller const& _test, test::session::SessionInterface& _session, TestSuite::TestSuiteOptions const& _opt)
      : m_testSuiteOpt(_opt), m_test(_test), m_session(_session) {}

private:
    void fillInfoWithLabels();
    void performPoststate(EthGetBlockBy const& _blockInfo);
    void performStatediff();
    void performVmtrace(EthGetBlockBy const& _blockInfo, TransactionInGeneralSection const& _tr, FORK const& _network);
    std::string performVmtraceAnalys(FH32 const& _trHash, StateTestFillerExpectSection const& _expResult, FORK const& _network);
protected:
    TestSuite::TestSuiteOptions const& m_testSuiteOpt;
    spDataObject m_filledTest;
    spDataObject m_forkResults;
    StateTestInFiller const& m_test;
    test::session::SessionInterface& m_session;
    std::vector<TransactionInGeneralSection> m_txs;
};

}
