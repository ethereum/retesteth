#pragma once
#include "StateTestFillerRunner.h"
#include <retesteth/session/Session.h>
#include <retesteth/testStructures/structures.h>
#include <retesteth/testSuiteRunner/TestSuite.h>

namespace test::statetests
{

class StateTestFillerRunnerEEST : public StateTestFillerRunner
{
public:
    StateTestFillerRunnerEEST(StateTestInFiller const& _test, TestSuite::TestSuiteOptions const& _opt);
    void performTransactionOnExpect(TransactionInGeneralSection&, StateTestFillerExpectSection const&, FORK const&) override;
    spDataObject getFilledTest() const override;

protected:
    void constructTestVector(FORK const& _network, TransactionInGeneralSection&);

private:
    spDataObject m_finalFilled_test;
};

}
