#pragma once
#include "Filler/InfoIncomplete.h"
#include "Filler/StateTestFillerEnv.h"
#include "Filler/StateTestFillerExpectSection.h"
#include "Filler/StateTestFillerTransaction.h"
#include <retesteth/dataObject/DataObject.h>
#include <retesteth/dataObject/SPointer.h>

using namespace dataobject;
using namespace test::teststruct;
namespace test
{
namespace teststruct
{
struct StateTestInFiller : GCP_SPointerBase
{
    StateTestInFiller(spDataObject&);

    string const& testName() const { return m_name; }
    bool hasInfo() const { return !m_info.isEmpty(); }
    InfoIncomplete const& Info() const
    {
        assert(hasInfo());
        return m_info;
    }
    StateTestFillerEnv const& Env() const { return m_env; }
    State const& Pre() const { return m_pre; }
    StateTestFillerTransaction const& GeneralTr() const { return m_transaction; }
    std::vector<StateTestFillerExpectSection> const& Expects() const { return m_expectSections; }
    std::set<FORK> getAllForksFromExpectSections() const;

private:
    StateTestInFiller() {}
    string m_name;
    GCP_SPointer<InfoIncomplete> m_info;
    GCP_SPointer<StateTestFillerEnv> m_env;
    spState m_pre;
    GCP_SPointer<StateTestFillerTransaction> m_transaction;
    std::vector<StateTestFillerExpectSection> m_expectSections;
};

struct GeneralStateTestFiller
{
    GeneralStateTestFiller(spDataObject&);
    std::vector<StateTestInFiller> const& tests() const { return m_tests; }

private:
    GeneralStateTestFiller() {}
    std::vector<StateTestInFiller> m_tests;
};


}  // namespace teststruct
}  // namespace test
