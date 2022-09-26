#pragma once
#include "../../basetypes.h"
#include <libdataobj/DataObject.h>

namespace test
{
namespace teststruct
{
// Structure for RPC response debug_accountRange
struct DebugAccountRange
{
    DebugAccountRange(DataObject const&);
    FH32 const& nextKey() const { return m_nextKey; }
    std::vector<spFH20> const& addresses() const { return m_addresses; }

private:
    std::vector<spFH20> m_addresses;
    spFH32 m_nextKey;
};

}  // namespace teststruct
}  // namespace test
