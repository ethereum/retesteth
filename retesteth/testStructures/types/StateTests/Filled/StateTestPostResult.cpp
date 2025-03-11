#include "StateTestPostResult.h"
#include <retesteth/testStructures/Common.h>
#include <retesteth/Constants.h>
#include <retesteth/Options.h>

namespace test::teststruct
{
StateTestPostResult::StateTestPostResult(DataObject const& _data)
{
    auto const& opt = Options::get();
    bool const isNewTest = !opt.isLegacy() && !opt.isLegacyConstantinople();
    REQUIRE_JSONFIELDS(_data, "StateTestPostResult " + _data.getKey(),
        {{"indexes", {{DataType::Object}, jsonField::Required}},
            {"hash", {{DataType::String}, jsonField::Required}},
            {"post", {{DataType::Object}, jsonField::Optional}},
            {"txbytes", {{DataType::String}, jsonField::Optional}},
            {"txtrace", {{DataType::String}, jsonField::Optional}},
            {"state", {{DataType::Object}, isNewTest ? jsonField::Required : jsonField::Optional}},
            {"expectException", {{DataType::String}, jsonField::Optional}},
            {"logs", {{DataType::String}, jsonField::Optional}}});

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
}

/// Used solo to print the debug message
spDataObject StateTestPostResult::asDataObject() const
{
    spDataObject res;
    (*res)["txbytes"] = m_txbytes->asString();
    (*res)["hash"] = m_hash->asString();
    if (!m_log.isEmpty())
        (*res)["logs"] = m_log->asString();
    (*res)["indexes"]["data"] = m_dataInd;
    (*res)["indexes"]["gas"] = m_gasInd;
    (*res)["indexes"]["value"] = m_valInd;
    return res;
}

FH32 const& StateTestPostResult::logs() const
{
    static FH32 emptyLogs(C_EMPTY_LIST_HASH);
    if (m_log.isEmpty())
        return emptyLogs;
    return m_log;
}

}  // namespace teststruct
