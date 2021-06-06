#pragma once
#include "../Ethereum/State.h"
#include "../Ethereum/StateIncomplete.h"
#include "../Ethereum/Transaction.h"
#include "../StateTests/Filler/InfoIncomplete.h"
#include "../StateTests/Filler/StateTestFillerEnv.h"
#include <retesteth/dataObject/DataObject.h>
#include <retesteth/dataObject/SPointer.h>

using namespace dataobject;
using namespace test::teststruct;
namespace test
{
namespace teststruct
{
struct VMTestInFiller : GCP_SPointerBase
{
    VMTestInFiller(DataObject const&);

    string const& testName() const { return m_name; }
    bool hasInfo() const { return !m_info.isEmpty(); }
    InfoIncomplete const& Info() const
    {
        assert(hasInfo());
        return m_info;
    }
    StateTestFillerEnv const& Env() const { return m_env; }
    State const& Pre() const { return m_pre; }
    Transaction const& Tr() const { return m_transaction; }
    bool hasExpect() const { return !m_expect.isEmpty(); }
    StateIncomplete const& Expect() const { return m_expect; }

private:
    VMTestInFiller() {}
    string m_name;
    GCP_SPointer<InfoIncomplete> m_info;
    GCP_SPointer<StateTestFillerEnv> m_env;
    spState m_pre;
    spStateIncomplete m_expect;
    spTransaction m_transaction;
};

struct VMTestFiller
{
    VMTestFiller(DataObject const&);
    std::vector<VMTestInFiller> const& tests() const { return m_tests; }

private:
    VMTestFiller() {}
    std::vector<VMTestInFiller> m_tests;
};


}  // namespace teststruct
}  // namespace test
