#include "BlockHeader.h"
#include "BlockHeader1559.h"
#include "BlockHeaderLegacy.h"
#include "BlockHeaderMerge.h"
#include "BlockHeaderShanghai.h"

namespace test::teststruct
{
spBlockHeader readBlockHeader(DataObject const& _data);
spBlockHeader readBlockHeader(dev::RLP const& _rlp);

// Block Serialization conditions
bool isBlockExportCurrentRandom(BlockHeader const&);
bool isBlockExportWithdrawals(BlockHeader const&);
bool isBlockExportBasefee(BlockHeader const&);
bool isBlockExportDifficulty(BlockHeader const&);

bool isBlockPoS(BlockHeader const&);

}  // namespace teststruct
