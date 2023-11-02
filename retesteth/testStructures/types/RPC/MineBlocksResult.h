#pragma once
#include <retesteth/testStructures/basetypes.h>
#include <libdataobj/DataObject.h>
#include <retesteth/testStructures/Common.h>

namespace test
{
namespace teststruct
{

struct MineBlocksResult
{
    MineBlocksResult(DataObject const& _data)
    {
        m_isRejectData = false;
        if (_data.type() == DataType::Bool)
            m_result = _data.asBool();
        else
        {
            REQUIRE_JSONFIELDS(_data, "test_mineBlocks::MineBlocksResult",
                {{"result", {{DataType::Bool, DataType::Integer}, jsonField::Required}},
                    {"rejectedTransactions", {{DataType::Array}, jsonField::Optional}},
                    {"acceptedTransactions", {{DataType::Array}, jsonField::Optional}}});

            if (_data.atKey("result").type() == DataType::Bool)
                m_result = _data.atKey("result").asBool();
            else
                m_result = _data.atKey("result").asInt();
            m_isRejectData = true;
            if (_data.count("rejectedTransactions"))
            {
                for (auto const& el : _data.atKey("rejectedTransactions").getSubObjects())
                    m_rejectedTransactions.emplace(
                        FH32(el->atKey("hash").asString()), el->atKey("error").asString());
            }
        }
    }

    bool isRejectData() const { return m_isRejectData; }
    size_t rejectedTxCount() const { return m_rejectedTransactions.size(); }

    std::string const& getTrException(FH32 const& _hash) const
    {
        static std::string empty;
        if (m_rejectedTransactions.count(_hash))
            return m_rejectedTransactions.at(_hash);
        return empty;
    }

protected:
    bool m_result;
    bool m_isRejectData;
    std::map<FH32, std::string> m_rejectedTransactions;
};


}  // namespace teststruct
}  // namespace test
