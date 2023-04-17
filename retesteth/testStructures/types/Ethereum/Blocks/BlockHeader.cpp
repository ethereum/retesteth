#include "BlockHeader.h"
#include <libdevcore/CommonIO.h>
#include <libdevcore/SHA3.h>
#include <retesteth/Constants.h>

namespace test::teststruct
{
std::string BlockHeader::TypeToString(BlockType _t)
{
    switch (_t)
    {
        using enum BlockType;
        case BlockHeader1559: return "BlockHeader1559";
        case BlockHeaderLegacy: return "BlockHeaderLegacy";
        case BlockHeaderMerge: return "BlockHeaderMerge";
        case BlockHeaderShanghai: return "BlockHeaderShanghai";
        default: return "UnparsedBlockType";
    }
    return "UnparsedBlockType";
}

std::string BlockHeader::BlockTypeToString(BlockType _bl)
{
    switch (_bl)
    {
        using enum BlockType;
        case BlockHeader1559: return "BlockHeader1559";
        case BlockHeaderLegacy: return "BlockHeaderLegacy";
        case BlockHeaderMerge: return "BlockHeaderMerge";
        case BlockHeaderShanghai: return "BlockHeaderShanghai";
        default: return "BlockHeaderUndefined";
    };
    return "BlockHEaderUndefined";
}

void BlockHeader::recalculateHash()
{
    FH32* newHash = new FH32("0x" + dev::toString(dev::sha3(asRLPStream().out())));
    m_hash = spFH32(newHash);
}

bool BlockHeader::hasUncles() const
{
    return m_sha3Uncles->asString() != C_EMPTY_LIST_HASH;
}



}  // namespace test::teststruct
