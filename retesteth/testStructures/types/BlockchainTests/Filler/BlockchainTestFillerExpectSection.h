#pragma once
#include "../../../configs/FORK.h"
#include "../../../types/Ethereum/StateIncomplete.h"
#include <libdataobj/DataObject.h>
#include <libdataobj/SPointer.h>

namespace test
{
namespace teststruct
{
struct BlockchainTestFillerExpectSection
{
    BlockchainTestFillerExpectSection(spDataObject&);
    StateIncomplete const& result() const { return m_result; }
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
