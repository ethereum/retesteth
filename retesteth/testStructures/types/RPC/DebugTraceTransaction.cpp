#include "DebugTraceTransaction.h"
#include <retesteth/EthChecks.h>
#include <retesteth/testStructures/Common.h>

using namespace std;
namespace test::teststruct
{
DebugTraceTransaction::DebugTraceTransaction(DataObject const& _data)
{
    try
    {
        REQUIRE_JSONFIELDS(_data, "DebugTraceTransaction " + _data.getKey(),
            {{"structLogs", {{DataType::Object}, jsonField::Required}},
                {"gas", {{DataType::String}, jsonField::Required}},
                {"return", {{DataType::String}, jsonField::Required}}});

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

DebugTraceTransactionLog::DebugTraceTransactionLog(DataObject const& _data) {
    m_data = spDataObject();
    (*m_data).copyFrom(_data);
}

string DebugTraceTransaction::getFinal() const
{
    if (m_entries.size() == 0)
        return "";
    DebugTraceTransactionLog const& lastLog = m_entries.at(m_entries.size() - 1);
    DataObject final;
    final["output"] = m_return->asString();
    final["gasUsed"] = (m_gas.getCContent() - lastLog.gas()).asDecString();
    return final.asJson(0, false);
}

}  // namespace teststruct
