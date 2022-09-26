#pragma once
#include "../../basetypes.h"
#include "MineBlocksResult.h"
#include <libdataobj/DataObject.h>

namespace test
{
namespace teststruct
{
// Structure for RPC response test_rawTransaction
struct TestRawTransaction : MineBlocksResult
{
    TestRawTransaction(DataObject const&);
    FH20 const& sender() const { return m_sender; }
    FH32 const& trhash() const { return m_trHash; }
    VALUE const& intrinsicGas() const { return m_intrinsicGas; }
    std::string const& error() const;

private:
    spFH20 m_sender;
    spFH32 m_trHash;
    spVALUE m_intrinsicGas;
};

}  // namespace teststruct
}  // namespace test
