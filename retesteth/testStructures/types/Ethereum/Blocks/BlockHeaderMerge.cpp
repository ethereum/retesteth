#include "BlockHeaderMerge.h"
#include <retesteth/EthChecks.h>

using namespace std;
using namespace dev;

namespace test::teststruct
{
BlockHeaderMerge& BlockHeaderMerge::castFrom(BlockHeader& _from)
{
    try
    {
        if (_from.type() != BlockType::BlockHeaderMerge)
            ETH_FAIL_MESSAGE("BlockHeaderMerge::castFrom() got wrong block type!");
        return dynamic_cast<BlockHeaderMerge&>(_from);
    }
    catch (...)
    {
        ETH_FAIL_MESSAGE("BlockHeaderMerge::castFrom() failed!");
    }
    return dynamic_cast<BlockHeaderMerge&>(_from);
}

BlockHeaderMerge const& BlockHeaderMerge::castFrom(spBlockHeader const& _from)
{
    try
    {
        if (_from->type() != BlockType::BlockHeaderMerge)
            ETH_FAIL_MESSAGE("BlockHeaderMerge::castFrom() got wrong block type!");
        return dynamic_cast<BlockHeaderMerge const&>(_from.getCContent());
    }
    catch (...)
    {
        ETH_FAIL_MESSAGE("BlockHeaderMerge::castFrom() failed!");
    }
    spBlockHeaderMerge foo(new BlockHeaderMerge(DataObject()));
    return foo.getCContent();
}

}  // namespace teststruct
