#pragma once
#include "../../basetypes.h"
#include <libdataobj/DataObject.h>

namespace test
{
namespace teststruct
{
struct DebugTraceTransactionLog
{
    DebugTraceTransactionLog(DataObject const&);
    VALUE gas() const { return VALUE(m_data->atKey("gas")); }
    DataObject const& getData() const { return m_data; }

private:
    spDataObject m_data;
};

struct DebugTraceTransaction
{
    DebugTraceTransaction(DataObject const&);
    std::vector<DebugTraceTransactionLog> const& getEntries() const { return m_entries; }
    std::string getFinal() const;

private:
    std::vector<DebugTraceTransactionLog> m_entries;
    spVALUE m_gas;
    spBYTES m_return;
};

}  // namespace teststruct
}  // namespace test
