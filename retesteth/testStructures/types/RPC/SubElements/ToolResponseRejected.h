#pragma once
#include "../../../basetypes.h"
#include <libdataobj/DataObject.h>

namespace test
{
namespace teststruct
{
struct ToolResponseRejected
{
    ToolResponseRejected(DataObject const& _data);
    size_t const& index() const { return m_index; }
    std::string const& error() const { return m_sError; }
    FH32 const& trHash() const { return m_trHash; }

private:
    ToolResponseRejected() {}
    size_t m_index;
    std::string m_sError;
    spFH32 m_trHash;
};

}  // namespace teststruct
}  // namespace test
