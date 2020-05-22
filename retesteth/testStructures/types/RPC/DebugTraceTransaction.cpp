#include "DebugTraceTransaction.h"
#include <retesteth/EthChecks.h>

namespace test
{
namespace teststruct
{
DebugTraceTransaction::DebugTraceTransaction(DataObject const& _data)
{
    try
    {
        for (auto const& entry : _data.atKey("structLogs").getSubObjects())
            m_entries.push_back(DebugTraceTransactionLog(entry));
        m_gas = spVALUE(new VALUE(_data.atKey("gas")));
        m_return = spBYTES(new BYTES(_data.atKey("return")));
    }
    catch (std::exception const& _ex)
    {
        ETH_ERROR_MESSAGE(string("DebugTraceTransaction unmarshal error: ") + _ex.what());
    }
}

DebugTraceTransactionLog::DebugTraceTransactionLog(DataObject const& _data) : m_data(_data) {}

string DebugTraceTransaction::getFinal() const
{
    DebugTraceTransactionLog const& lastLog = m_entries.at(m_entries.size() - 1);
    DataObject final;
    final["output"] = m_return.getCContent().asString();
    final["gasUsed"] = (m_gas.getCContent() - lastLog.gas()).asDecString();
    return final.asJson(0, false);
}

}  // namespace teststruct
}  // namespace test
