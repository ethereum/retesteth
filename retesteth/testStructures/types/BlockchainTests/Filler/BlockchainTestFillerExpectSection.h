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
    std::vector<FORK> const& forks() const { return m_forks; }
    bool hasFork(FORK const& _fork) const
    {
        for (auto const& el : m_forks)
            if (el == _fork)
                return true;
        return false;
    }

private:
    std::vector<FORK> m_forks;
    GCP_SPointer<StateIncomplete> m_result;
};


}  // namespace teststruct
}  // namespace test
