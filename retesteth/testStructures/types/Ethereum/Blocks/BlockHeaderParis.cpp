#include "BlockHeaderParis.h"
#include <retesteth/EthChecks.h>

using namespace std;
using namespace dev;

namespace test::teststruct
{

namespace  {
inline bool isChild(BlockType _t)
{
    // Can't use compareFork function here because of EthereumClassic and custom fork names
    return _t != BlockType::BlockHeaderParis &&
           _t != BlockType::BlockHeaderShanghai &&
           _t != BlockType::BlockHeader4844;
}
}


BlockHeaderParis& BlockHeaderParis::castFrom(BlockHeader& _from)
{
    try
    {
        if (isChild(_from.type()))
            ETH_FAIL_MESSAGE("BlockHeaderParis::castFrom() got wrong block type!");
        return dynamic_cast<BlockHeaderParis&>(_from);
    }
    catch (...)
    {
        ETH_FAIL_MESSAGE("BlockHeaderParis::castFrom() failed!");
    }
    return dynamic_cast<BlockHeaderParis&>(_from);
}

BlockHeaderParis const& BlockHeaderParis::castFrom(spBlockHeader const& _from)
{
    try
    {
        if (isChild(_from->type()))
            ETH_FAIL_MESSAGE("BlockHeaderParis::castFrom() got wrong block type!");
        return dynamic_cast<BlockHeaderParis const&>(_from.getCContent());
    }
    catch (...)
    {
        ETH_FAIL_MESSAGE("BlockHeaderParis::castFrom() failed!");
    }
    spBlockHeaderParis foo(new BlockHeaderParis(DataObject()));
    return foo.getCContent();
}

}  // namespace teststruct
