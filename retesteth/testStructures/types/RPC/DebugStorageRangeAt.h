#pragma once
#include "../../basetypes.h"
#include "../Ethereum/Storage.h"
#include <retesteth/dataObject/DataObject.h>

using namespace dataobject;

namespace test
{
namespace teststruct
{
// Structure for RPC response debug_accountRange
struct DebugStorageRangeAt
{
    DebugStorageRangeAt(DataObject const&);
    Storage const& storage() const { return m_storage; }
    FH32 const& nextKey() const { return m_nextKey; }

private:
    spStorage m_storage;
    spFH32 m_nextKey;
};

}  // namespace teststruct
}  // namespace test
