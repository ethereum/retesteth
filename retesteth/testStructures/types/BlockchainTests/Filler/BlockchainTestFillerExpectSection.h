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
struct BlockchainTestFillerExpectSection
{
    BlockchainTestFillerExpectSection(DataObject const&);
    StateIncomplete const& result() const { return m_result.getCContent(); }
    std::set<FORK> const& forks() const { return m_forks; }

private:
    std::set<FORK> m_forks;
    GCP_SPointer<StateIncomplete> m_result;
};


}  // namespace teststruct
}  // namespace test
