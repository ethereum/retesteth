#pragma once
#include "../../StateIncomplete.h"
#include <retesteth/dataObject/DataObject.h>
#include <retesteth/dataObject/SPointer.h>
using namespace dataobject;
using namespace test::teststruct;

namespace test
{
namespace teststruct
{
struct StateTestFillerExpectSection
{
    StateTestFillerExpectSection(DataObject const&);
    StateIncomplete const& result() const { return *m_result.getCPtr(); }
    std::set<string> const& forks() const { return m_forks; }

private:
    std::set<int> m_dataInd;
    std::set<int> m_gasInd;
    std::set<int> m_valInd;
    std::set<string> m_forks;
    GCP_SPointer<StateIncomplete> m_result;
};


}  // namespace teststruct
}  // namespace test
