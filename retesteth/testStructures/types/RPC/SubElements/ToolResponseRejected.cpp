#include "ToolResponseRejected.h"
#include <retesteth/EthChecks.h>
#include <retesteth/testStructures/Common.h>

namespace test
{
namespace teststruct
{
ToolResponseRejected::ToolResponseRejected(DataObject const& _data)
{
    m_index = _data.atKey("index").asInt();
    m_sError = _data.atKey("error").asString();
    if (_data.count("hash"))
        m_trHash = spFH32(new FH32(_data.atKey("hash")));

    REQUIRE_JSONFIELDS(_data, "ToolResponseRejected " + _data.getKey(),
        {{"index", {{DataType::Integer}, jsonField::Required}},
         {"error", {{DataType::String}, jsonField::Required}},
         {"hash", {{DataType::String}, jsonField::Optional}}
                      });
}


}  // namespace teststruct
}  // namespace test
