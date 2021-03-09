#pragma once
#include <retesteth/dataObject/DataObject.h>
#include <retesteth/dataObject/SPointer.h>
#include <map>
using namespace dataobject;

namespace test
{
namespace teststruct
{
struct Info : GCP_SPointerBase
{
    Info(DataObject const&);
    string const& comment() const { return m_comment; }
    string const& filling_rpc_server() const { return m_filling_rpc_server; }
    string const& filling_tool_version() const { return m_filling_tool_version; }
    string const& lllcversion() const { return m_lllcversion; }
    string const& source() const { return m_source; }
    string const& sourceHash() const { return m_sourceHash; }
    std::map<string, string> const& labels() const { return m_labels; }

private:
    Info() {}
    string m_comment;
    string m_filling_rpc_server;
    string m_filling_tool_version;
    string m_lllcversion;
    string m_source;
    string m_sourceHash;
    std::map<string, string> m_labels;
};

typedef GCP_SPointer<Info> spInfo;

}  // namespace teststruct
}  // namespace test
