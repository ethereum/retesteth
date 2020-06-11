#pragma once
#include "../../../basetypes.h"
#include <retesteth/dataObject/DataObject.h>

using namespace dataobject;

namespace test
{
namespace teststruct
{
struct ToolResponseReceipt
{
    ToolResponseReceipt(DataObject const& _data);
    FH32 const& trHash() const { return m_trHash.getCContent(); }

private:
    ToolResponseReceipt() {}
    spFH32 m_trHash;
};

}  // namespace teststruct
}  // namespace test
