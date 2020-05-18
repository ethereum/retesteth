#pragma once
#include "../../Common.h"
#include "../InfoIncomplete.h"
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
    StateTestInFiller(DataObject const&);

    string const& testName() const { return m_name; }
    bool hasInfo() const { return m_info.isEmpty(); }
    InfoIncomplete const& Info() const
    {
        assert(hasInfo());
        return *m_info.getCPtr();
    }
    StateTestFillerEnv const& Env() const { return *m_env.getCPtr(); }
    State const& Pre() const { return *m_pre.getCPtr(); }
    StateTestFillerTransaction const& GTr() const { return *m_transaction.getCPtr(); }
    std::vector<StateTestFillerExpectSection> const& Expects() const { return m_expectSections; }
    std::list<FORK> getAllNetworksFromExpectSections() const;

private:
    StateTestInFiller() {}
    string m_name;
    GCP_SPointer<InfoIncomplete> m_info;
    GCP_SPointer<StateTestFillerEnv> m_env;
    GCP_SPointer<State> m_pre;
    GCP_SPointer<StateTestFillerTransaction> m_transaction;
    std::vector<StateTestFillerExpectSection> m_expectSections;
};

struct GeneralStateTestFiller
{
    GeneralStateTestFiller(DataObject const&);
    std::vector<StateTestInFiller> const& tests() const { return m_tests; }

private:
    std::vector<StateTestInFiller> m_tests;
};


}  // namespace teststruct
}  // namespace test
