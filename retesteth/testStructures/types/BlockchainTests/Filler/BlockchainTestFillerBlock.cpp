#include "BlockchainTestFillerBlock.h"
#include <retesteth/EthChecks.h>

namespace test
{
namespace teststruct
{
BlockchainTestFillerBlock::BlockchainTestFillerBlock(DataObject const& _data)
  : m_chainName(BlockchainTestFillerBlock::defaultChainName())
{
    try
    {
        if (_data.count("rlp"))
        {
            m_rlp = spBYTES(new BYTES(_data.atKey("rlp")));
            ETH_ERROR_REQUIRE_MESSAGE(_data.getSubObjects().size() <= 2,
                "Block described as raw rlp can have only `rlp` and `blockHeader` sections!");
        }

        if (_data.count("chainname"))
            m_chainName = _data.atKey("chainname").asString();
        else
            m_chainName = BlockchainTestFillerBlock::defaultChainName();

        m_doNotImportOnClient = _data.count("donotimportonclient");

        if (_data.count("blocknumber"))
            m_blockNumber = spVALUE(new VALUE(_data.atKey("blocknumber").asString()));

        if (_data.count("chainnetwork"))
            m_network = spFORK(new FORK(_data.atKey("chainnetwork")));

        for (auto const& tr : _data.atKey("transactions").getSubObjects())
            m_transactions.push_back(BlockchainTestFillerTransaction(tr));

        for (auto const& un : _data.atKey("uncleHeaders").getSubObjects())
            m_uncles.push_back(BlockchainTestFillerUncle(un));

        if (_data.count("blockHeader"))
        {
            m_blockHeaderIncomplete = spBlockHeaderIncomplete(new BlockHeaderIncomplete(_data.atKey("blockHeader")));
            if (_data.atKey("blockHeader").count("expectException"))
            {
                for (auto const& rec : _data.atKey("blockHeader").atKey("expectException").getSubObjects())
                    m_expectExceptions.emplace(FORK(rec.getKey()), rec.asString());
            }
            if (_data.atKey("blockHeader").count("RelTimestamp"))
                m_relTimeStamp = spVALUE(new VALUE(_data.atKey("blockHeader").atKey("RelTimestamp")));
        }
    }
    catch (std::exception const& _ex)
    {
        throw BaseEthException(string("BlockchainTestFillerBlock convertion error: ") + _ex.what());
    }
}

}  // namespace teststruct
}  // namespace test
