#pragma once
#include "../../../basetypes.h"
#include <libdataobj/DataObject.h>

namespace test
{
namespace teststruct
{
struct ToolResponseReceipt
{
    ToolResponseReceipt(DataObject const& _data);
    FH32 const& trHash() const { return m_trHash; }
    VALUE const& gasUsed() const { return m_trGasUsed; }

private:
    ToolResponseReceipt() {}
    spVALUE m_trGasUsed;
    spFH32 m_trHash;
};

}  // namespace teststruct
}  // namespace test
