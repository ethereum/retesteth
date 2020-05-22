#pragma once
#include "Filled/Info.h"
#include "Filled/StateTestEnv.h"
#include "Filled/StateTestTransaction.h"
#include <retesteth/dataObject/DataObject.h>
#include <retesteth/dataObject/SPointer.h>

using namespace dataobject;
using namespace test::teststruct;
namespace test
{
namespace teststruct
{
struct StateTestInFilled : GCP_SPointerBase
{
    StateTestInFilled(DataObject const&);

    string const& testName() const { return m_name; }
    Info const& testInfo() const { return m_info.getCContent(); }
    StateTestEnv const& Env() const { return m_env.getCContent(); }
    State const& Pre() const { return m_pre.getCContent(); }
    StateTestTransaction const& Tr() const { return m_transaction.getCContent(); }
    // std::vector<StateTestExpectSection> const& Expects() const { return m_expectSections; }
    // std::list<FORK> getAllForksFromExpectSections() const;

private:
    StateTestInFilled() {}
    string m_name;
    GCP_SPointer<Info> m_info;
    GCP_SPointer<StateTestEnv> m_env;
    spState m_pre;
    GCP_SPointer<StateTestTransaction> m_transaction;
    // std::vector<StateTestExpectSection> m_expectSections;
};

struct GeneralStateTest
{
    GeneralStateTest(DataObject const&);
    std::vector<StateTestInFilled> const& tests() const { return m_tests; }

private:
    GeneralStateTest() {}
    std::vector<StateTestInFilled> m_tests;
};


}  // namespace teststruct
}  // namespace test
