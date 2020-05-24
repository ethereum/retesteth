#include "StateTestPostResult.h"

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
    m_log = spFH32(new FH32(_data.atKey("logs").asString()));
}

DataObject StateTestPostResult::asDataObject() const
{
    DataObject res;
    res["hash"] = m_hash.getCContent().asString();
    res["logs"] = m_log.getCContent().asString();
    res["indexes"]["data"] = m_dataInd;
    res["indexes"]["gas"] = m_gasInd;
    res["indexes"]["value"] = m_valInd;
    return res;
}

}  // namespace teststruct
}  // namespace test
