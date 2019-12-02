#pragma once
#include "../object.h"

#include <libdevcore/Address.h>
#include <libdevcore/RLP.h>
#include <libdevcore/SHA3.h>
#include <retesteth/TestHelper.h>

namespace test
{
class opLog : public object
{
public:
    opLog(DataObject const& _entry) : object(_entry)
    {
        requireJsonFields(_entry, "scheme_debugTraceTransaction_entry_rpc_response",
            {{"depth", {{DataType::Integer}, jsonField::Required}},
                {"gas", {{DataType::String}, jsonField::Required}},
                {"gasCost", {{DataType::String}, jsonField::Required}},
                {"memSize", {{DataType::Integer}, jsonField::Required}},
                {"memory", {{DataType::Array}, jsonField::Required}},
                {"op", {{DataType::Integer}, jsonField::Required}},
                {"opName", {{DataType::String}, jsonField::Required}},
                {"pc", {{DataType::Integer}, jsonField::Required}},
                {"stack", {{DataType::Array}, jsonField::Required}},
                {"storage", {{DataType::Object}, jsonField::Optional}}});
    }
};

class scheme_debugTraceTransaction : public object
{
public:
    scheme_debugTraceTransaction(DataObject const& _receipt) : object(_receipt)
    {
        requireJsonFields(_receipt, "scheme_debugTraceTransaction_rpc_response",
            {{"gas", {DataType::String}}, {"return", {DataType::String}},
                {"structLogs", {DataType::Array}}});

        for (auto const& entry : _receipt.atKey("structLogs").getSubObjects())
            m_entries.push_back(opLog(entry));
    }

    std::vector<opLog> const& getEntries() const { return m_entries; }
    string getFinal() const
    {
        dev::u256 totalGas(m_data.atKey("gas").asString());
        dev::u256 finalGas(m_entries.at(m_entries.size() - 1).getData().atKey("gas").asString());
        DataObject final;
        final["output"] = m_data.atKey("return");
        final["gasUsed"] = dev::toString(totalGas - finalGas);
        return final.asJson(0, false);
    }

private:
    std::vector<opLog> m_entries;
};
}  // namespace test
