#include "StateTestPostResult.h"
#include <retesteth/testStructures/Common.h>

namespace test
{
namespace teststruct
{
StateTestPostResult::StateTestPostResult(DataObject const& _data)
{
    m_dataInd = _data.atKey("indexes").atKey("data").asInt();
    m_gasInd = _data.atKey("indexes").atKey("gas").asInt();
    m_valInd = _data.atKey("indexes").atKey("value").asInt();
    m_hash = spFH32(new FH32(_data.atKey("hash").asString()));
    if (_data.count("logs"))
        m_log = spFH32(new FH32(_data.atKey("logs").asString()));
    if (_data.count("txbytes"))
        m_txbytes = spBYTES(new BYTES(_data.atKey("txbytes").asString()));
    if (_data.count("expectException"))
        m_expectException = _data.atKey("expectException").asString();
    requireJsonFields(_data, "StateTestPostResult " + _data.getKey(),
        {{"indexes", {{DataType::Object}, jsonField::Required}},
         {"hash", {{DataType::String}, jsonField::Required}},
         {"txbytes", {{DataType::String}, jsonField::Optional}},
         {"expectException", {{DataType::String}, jsonField::Optional}},
         {"logs", {{DataType::String}, jsonField::Optional}}});
}

/// Used solo to print the debug message
spDataObject StateTestPostResult::asDataObject() const
{
    spDataObject res (new DataObject());
    (*res)["hash"] = m_hash->asString();
    if (!m_log.isEmpty())
        (*res)["logs"] = m_log->asString();
    (*res)["indexes"]["data"] = m_dataInd;
    (*res)["indexes"]["gas"] = m_gasInd;
    (*res)["indexes"]["value"] = m_valInd;
    return res;
}

}  // namespace teststruct
}  // namespace test
