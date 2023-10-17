#pragma once
#include <retesteth/session/Session.h>
#include <retesteth/testStructures/structures.h>
#include <retesteth/testSuites/statetests/StateTestFillerRunner.h>

namespace test::statetests
{

class StateTestChainRunner : public StateTestFillerRunner
{
public:
    StateTestChainRunner(StateTestInFiller const& _test);
    void prepareChainParams(FORK const&) override;
    void performTransactionOnExpect(TransactionInGeneralSection& _tr, StateTestFillerExpectSection const& _expect, FORK const& _network) override;
    void initBlockchainTestData();
    void finalizeBlockchainTestData(TransactionInGeneralSection& _tr, FORK const& _network);
    void comparePostState(StateTestFillerExpectSection const& _expect, FORK const& _network);

private:
    spStateIncomplete correctMiningReward(StateTestFillerExpectSection const& _expect, FORK const& _network);

private:
    spDataObject m_aBlockchainTest;

};

}
