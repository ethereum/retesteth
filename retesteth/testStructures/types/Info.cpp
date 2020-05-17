#include "Info.h"

namespace test
{
namespace teststruct
{
Info::Info(DataObject const& _data)
{
    m_comment = _data.atKey("comment").asString();
    m_filling_rpc_server = _data.atKey("m_filling-rpc-server").asString();
    m_filling_tool_version = _data.atKey("m_filling-tool-version").asString();
    m_lllcversion = _data.atKey("lllcversion").asString();
    m_source = _data.atKey("source").asString();
    m_sourceHash = _data.atKey("sourceHash").asString();
}

}  // namespace teststruct
}  // namespace test
