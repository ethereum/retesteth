#include "BlockHeader.h"
#include <libdevcore/CommonIO.h>
#include <libdevcore/SHA3.h>
#include <retesteth/Constants.h>
#include <retesteth/EthChecks.h>

using namespace std;

namespace test::teststruct
{

std::string BlockHeader::BlockTypeToString(BlockType _bl)
{
    switch (_bl)
    {
        case BlockType::BlockHeader1559: return "BlockHeader1559";
        case BlockType::BlockHeaderLegacy: return "BlockHeaderLegacy";
        case BlockType::BlockHeaderMerge: return "BlockHeaderMerge";
        case BlockType::BlockHeaderShanghai: return "BlockHeaderShanghai";
        default: return "UnparsedBlockType";
    };
    return "UnparsedBlockType";
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

void BlockHeader::fromData(DataObject const& _data)
{
    try
    {
        checkDataScheme(_data);
        _fromData(_data);
        if (m_hash.isEmpty())
            recalculateHash();
    }
    catch (std::exception const& _ex)
    {
        throw test::UpwardsException(BlockTypeToString(type()) + string(" parse error: ") + _ex.what());
    }
}


}  // namespace test::teststruct
