#pragma once
#include <retesteth/session/Session.h>
#include <retesteth/testStructures/structures.h>

namespace test::statetests
{

class StateTestRunner
{
public:
    StateTestRunner(StateTestInFilled const& _test);
    bool checkNetworkSkip(FORK const&);
    void prepareChainParams(FORK const&);
    std::vector<TransactionInGeneralSection>& txs() { return m_txs; }
    void setTransactionInfo(TransactionInGeneralSection& _tr, FORK const& _network);
    void performTransactionOnResult(TransactionInGeneralSection&, StateTestPostResult const&, FORK const&);
private:
    std::vector<TransactionInGeneralSection> buildTransactionsWithLabels();
    void performVMTrace(TransactionInGeneralSection& _tr, FH32 const& _remoteStateHash, FORK const& _network);
    void performPostState(TransactionInGeneralSection& _tr, FORK const& _network);
    void performStateDiff();
    void performValidations(TransactionInGeneralSection& _tr, StateTestPostResult const& _result);
    std::string makeFilename(TransactionInGeneralSection& _tr, FORK const& _network);
private:
    StateTestInFilled const& m_test;
    test::session::SessionInterface& m_session;
    std::vector<TransactionInGeneralSection> m_txs;
};

}
