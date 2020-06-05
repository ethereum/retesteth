#include "BlockchainTestFillerBlock.h"
#include <retesteth/EthChecks.h>
#include <retesteth/testStructures/Common.h>
#include <retesteth/configs/ClientConfig.h>
#include <retesteth/Options.h>

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
        {
            DataObject tmpD = _data.atKey("blocknumber");
            tmpD.performModifier(mod_valueToCompactEvenHexPrefixed);
            m_blockNumber = spVALUE(new VALUE(tmpD));
        }

        if (_data.count("chainnetwork"))
            m_network = spFORK(new FORK(_data.atKey("chainnetwork")));

        if (_data.count("transactions"))
            for (auto const& tr : _data.atKey("transactions").getSubObjects())
                m_transactions.push_back(BlockchainTestFillerTransaction(tr));

        if (_data.count("uncleHeaders"))
            for (auto const& un : _data.atKey("uncleHeaders").getSubObjects())
                m_uncles.push_back(BlockchainTestFillerUncle(un));

        if (_data.count("blockHeader"))
        {
            DataObject tmpD = convertDecBlockheaderIncompleteToHex(_data.atKey("blockHeader"));
            if (tmpD.getSubObjects().size() > 0)
                m_blockHeaderIncomplete = spBlockHeaderIncomplete(new BlockHeaderIncomplete(tmpD));

            if (_data.atKey("blockHeader").count("expectException"))
            {
                for (auto const& rec : _data.atKey("blockHeader").atKey("expectException").getSubObjects())
                {
                    // Parse ">=Frontier" : "EXCEPTION"
                    ClientConfig const& cfg = Options::get().getDynamicOptions().getCurrentConfig();
                    std::set<string> forksString = { rec.getKey() };
                    std::vector<FORK> parsedForks = cfg.translateNetworks(forksString);
                    for (auto const& el : parsedForks)
                        m_expectExceptions.emplace(el, rec.asString());
                }
            }
            m_relTimeStamp = 0;
            if (_data.atKey("blockHeader").count("RelTimestamp"))
                m_relTimeStamp = hexOrDecStringToInt(_data.atKey("blockHeader").atKey("RelTimestamp").asString());
        }
        requireJsonFields(_data, "BlockchainTestFillerBlock " + _data.getKey(),
            {{"rlp", {{DataType::String}, jsonField::Optional}},
                {"chainname", {{DataType::String}, jsonField::Optional}},
                {"donotimportonclient", {{DataType::String}, jsonField::Optional}},
                {"blocknumber", {{DataType::String}, jsonField::Optional}},
                {"chainnetwork", {{DataType::String}, jsonField::Optional}},
                {"transactions", {{DataType::Array}, jsonField::Optional}},
                {"uncleHeaders", {{DataType::Array}, jsonField::Optional}},
                {"blockHeader", {{DataType::Object}, jsonField::Optional}}});
    }
    catch (std::exception const& _ex)
    {
        throw BaseEthException(string("BlockchainTestFillerBlock convertion error: ") + _ex.what() + _data.asJson());
    }
}

}  // namespace teststruct
}  // namespace test
