#include "BlockHeader.h"
#include <libdevcore/CommonIO.h>
#include <libdevcore/SHA3.h>

namespace test::teststruct
{
std::string BlockHeader::TypeToString(BlockType _t)
{
    switch (_t)
    {
    case BlockType::BlockHeader1559:
        return "BlockHeader1559";
    case BlockType::BlockHeaderLegacy:
        return "BlockHeaderLegacy";
    case BlockType::BlockHeaderMerge:
        return "BlockHeaderMerge";
    default:
        return "UnparsedBlockType";
    }
    return "UnparsedBlockType";
}

std::string BlockHeader::BlockTypeToString(BlockType _bl)
{
    switch (_bl)
    {
    case BlockType::BlockHeader1559:
        return "BlockHeader1559";
    case BlockType::BlockHeaderLegacy:
        return "BlockHeaderLegacy";
    case BlockType::BlockHeaderMerge:
        return "BlockHeaderMerge";
    default:
        return "BlockHeaderUndefined";
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
    return m_sha3Uncles->asString() != "0x1dcc4de8dec75d7aab85b567b6ccd41ad312451b948a7413f0a142fd40d49347";
}

}  // namespace test::teststruct
