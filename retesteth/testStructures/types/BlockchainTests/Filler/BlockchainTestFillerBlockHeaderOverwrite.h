#pragma once
#include "../../../basetypes.h"
#include "../../../configs/FORK.h"
#include "../../Ethereum/BlockHeaderIncomplete.h"
#include <libdataobj/DataObject.h>
#include <libdataobj/SPointer.h>

namespace test
{
namespace teststruct
{
// Information about blockheader overwrite
struct BlockHeaderOverwrite : GCP_SPointerBase
{
    BlockHeaderOverwrite(DataObject const&);

    // Blockheader oerwrite can define timestamp shift from previous block
    bool hasRelTimeStamp() const { return m_hasRelTimeStamp; }
    long long int relTimeStamp() const { return m_relTimeStamp; }
    bool hasBlockHeader() const { return !m_blockHeaderIncomplete.isEmpty(); }
    BlockHeaderIncomplete const& header() const { return m_blockHeaderIncomplete; }

private:
    bool m_hasRelTimeStamp = false;
    long long int m_relTimeStamp;
    spBlockHeaderIncomplete m_blockHeaderIncomplete;
};

typedef GCP_SPointer<BlockHeaderOverwrite> spBlockHeaderOverwrite;

}  // namespace teststruct
}  // namespace test
