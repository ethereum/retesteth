#include "BlockHeader.h"
#include "BlockHeader1559.h"
#include "BlockHeaderLegacy.h"
#include "BlockHeaderMerge.h"


namespace test::teststruct
{
spBlockHeader readBlockHeader(DataObject const& _data);
spBlockHeader readBlockHeader(dev::RLP const& _rlp);

}  // namespace teststruct
