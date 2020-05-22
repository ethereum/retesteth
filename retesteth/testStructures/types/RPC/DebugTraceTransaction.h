#pragma once
#include "../../basetypes.h"
#include <retesteth/dataObject/DataObject.h>

using namespace dataobject;

namespace test
{
namespace teststruct
{
struct DebugTraceTransactionLog
{
    DebugTraceTransactionLog(DataObject const&);
    VALUE gas() const { return VALUE(m_data.atKey("gas").asString()); }
    DataObject const& getData() const { return m_data; }

private:
    DataObject m_data;
};

struct DebugTraceTransaction
{
    DebugTraceTransaction(DataObject const&);
    std::vector<DebugTraceTransactionLog> const& getEntries() const { return m_entries; }
    string getFinal() const;

private:
    std::vector<DebugTraceTransactionLog> m_entries;
    spVALUE m_gas;
    spBYTES m_return;
};

}  // namespace teststruct
}  // namespace test
