#pragma once
#include "../Ethereum/State.h"
#include "Filled/Info.h"
#include "Filled/StateTestEnv.h"
#include "Filled/StateTestPostResult.h"
#include "Filled/StateTestTransaction.h"
#include <libdataobj/DataObject.h>

namespace test::teststruct
{
struct StateTestInFilled : GCP_SPointerBase
{
    StateTestInFilled(spDataObject&);

    std::string const& testName() const { return m_name; }
    Info const& testInfo() const { return m_info; }
    StateTestEnv const& Env() const { return m_env; }
    State const& Pre() const { return m_pre; }
    StateTestTransaction const& GeneralTr() const { return m_transaction; }
    std::map<FORK, StateTestPostResults> const& Post() const { return m_post; }

private:
    StateTestInFilled() {}
    std::string m_name;
    GCP_SPointer<Info> m_info;
    GCP_SPointer<StateTestEnv> m_env;
    spState m_pre;
    GCP_SPointer<StateTestTransaction> m_transaction;
    std::map<FORK, StateTestPostResults> m_post;
};

struct GeneralStateTest
{
    GeneralStateTest(spDataObject&);
    std::vector<StateTestInFilled> const& tests() const { return m_tests; }

private:
    GeneralStateTest() {}
    std::vector<StateTestInFilled> m_tests;
};

}  // namespace teststruct
