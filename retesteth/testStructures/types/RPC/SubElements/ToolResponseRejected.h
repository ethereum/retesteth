#pragma once
#include "../../../basetypes.h"
#include <retesteth/dataObject/DataObject.h>

using namespace dataobject;

namespace test
{
namespace teststruct
{
struct ToolResponseRejected
{
    ToolResponseRejected(DataObject const& _data);
    size_t const& index() const { return m_index; }
    string const& error() const { return m_sError; }
    FH32 const& trHash() const { return m_trHash.getCContent(); }

private:
    ToolResponseRejected() {}
    size_t m_index;
    string m_sError;
    spFH32 m_trHash;
};

}  // namespace teststruct
}  // namespace test
