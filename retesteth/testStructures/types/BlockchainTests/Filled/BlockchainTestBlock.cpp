#include "BlockchainTestBlock.h"

namespace test
{
namespace teststruct
{
BlockchainTestBlock::BlockchainTestBlock(DataObject const& _data)
{
    if (_data.count("chainname"))
        m_chainName = _data.atKey("chainname").asString();
    if (_data.count("blocknumber"))
        m_blockNumber = spVALUE(new VALUE(_data.atKey("blocknumber").asString()));
    if (_data.count("blockHeader"))
    {
        m_blockHeader = spBlockHeader(new BlockHeader(_data.atKey("blockHeader")));

        for (auto const& tr : _data.atKey("transactions").getSubObjects())
            m_transactions.push_back(Transaction(tr));

        for (auto const& un : _data.atKey("uncleHeaders").getSubObjects())
            m_uncles.push_back(BlockHeader(un));
    }
    m_rlp = spBYTES(new BYTES(_data.atKey("rlp").asString()));
}

}  // namespace teststruct
}  // namespace test
