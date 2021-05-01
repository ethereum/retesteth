#include "BlockHeaderReader.h"
#include <retesteth/TestHelper.h>

using namespace dataobject;
using namespace test::teststruct;

namespace test
{
namespace teststruct
{
spBlockHeader readBlockHeader(dev::RLP const& _rlp)
{
    if (_rlp.itemCount() == 15)
        return spBlockHeader(new BlockHeaderLegacy(_rlp));
    else
        return spBlockHeader(new BlockHeader1559(_rlp));
}

spBlockHeader readBlockHeader(DataObject const& _filledData)
{
    if (_filledData.count("gasTarget"))
        return spBlockHeader(new BlockHeader1559(_filledData));
    else
        return spBlockHeader(new BlockHeaderLegacy(_filledData));
}

}  // namespace teststruct
}  // namespace test
