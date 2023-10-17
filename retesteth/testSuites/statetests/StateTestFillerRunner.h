#pragma once
#include <retesteth/session/Session.h>
#include <retesteth/testStructures/structures.h>

namespace test::statetests
{

class StateTestFillerRunner
{
public:
    StateTestFillerRunner(StateTestInFiller const& _test);
    std::vector<TransactionInGeneralSection>& txs() { return m_txs; }
    bool checkNetworkSkip(FORK const& _network);
    virtual void prepareChainParams(FORK const&);
    void setErrorInfo(TransactionInGeneralSection const& _tr, FORK const& _network);
    virtual void performTransactionOnExpect(TransactionInGeneralSection&, StateTestFillerExpectSection const&, FORK const&);
    spDataObject getFilledTest() const { return m_filledTest; }
    void registerForkResult();
protected:
    StateTestFillerRunner(StateTestInFiller const& _test, test::session::SessionInterface& _session)
      : m_test(_test), m_session(_session) {}
private:
    void fillInfoWithLabels();
    void performPoststate(EthGetBlockBy const& _blockInfo);
    void performStatediff();
    void performVmtrace(EthGetBlockBy const& _blockInfo, TransactionInGeneralSection const& _tr, FORK const& _network);
    std::string performVmtraceAnalys(FH32 const& _trHash, StateTestFillerExpectSection const& _expResult, FORK const& _network);
protected:
    spDataObject m_filledTest;
    spDataObject m_forkResults;
    StateTestInFiller const& m_test;
    test::session::SessionInterface& m_session;
    std::vector<TransactionInGeneralSection> m_txs;
};

}
