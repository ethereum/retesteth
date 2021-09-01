#include "Info.h"
#include <retesteth/testStructures/Common.h>

namespace test
{
namespace teststruct
{
Info::Info(DataObject const& _data)
{
    REQUIRE_JSONFIELDS(_data, "Info " + _data.getKey(),
        {{"comment", {{DataType::String}, jsonField::Required}},
            {"filling-rpc-server", {{DataType::String}, jsonField::Required}},
            {"filling-tool-version", {{DataType::String}, jsonField::Required}},
            {"lllcversion", {{DataType::String}, jsonField::Required}},
            {"source", {{DataType::String}, jsonField::Required}},
            {"sourceHash", {{DataType::String}, jsonField::Required}},
            {"labels", {{DataType::Object}, jsonField::Optional}}});

    m_comment = _data.atKey("comment").asString();
    m_filling_rpc_server = _data.atKey("filling-rpc-server").asString();
    m_filling_tool_version = _data.atKey("filling-tool-version").asString();
    m_lllcversion = _data.atKey("lllcversion").asString();
    m_source = _data.atKey("source").asString();
    m_sourceHash = _data.atKey("sourceHash").asString();

    if (_data.count("labels"))
    {
        for (auto const& el : _data.atKey("labels").getSubObjects())
            m_labels.emplace(el->getKey(), el->asString());
    }
}

}  // namespace teststruct
}  // namespace test
