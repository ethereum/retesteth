#pragma once
#include "../../../configs/FORK.h"
#include "../../../types/Ethereum/StateIncomplete.h"
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
    StateIncomplete const& result() const { return m_result.getCContent(); }
    std::set<FORK> const& forks() const { return m_forks; }

    // Check that this indexes are present in this expect section
    bool checkIndexes(size_t _dInd, size_t _gInd, size_t _vInd) const;

private:
    std::set<int> m_dataInd;
    std::set<int> m_gasInd;
    std::set<int> m_valInd;
    std::set<FORK> m_forks;
    GCP_SPointer<StateIncomplete> m_result;
};


}  // namespace teststruct
}  // namespace test
