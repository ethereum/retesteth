#include "BlockchainTestFillerBlock.h"
#include <retesteth/EthChecks.h>
#include <retesteth/testStructures/Common.h>
#include <retesteth/configs/ClientConfig.h>
#include <retesteth/Options.h>
using namespace std;

namespace test::teststruct
{
BlockchainTestFillerBlock::BlockchainTestFillerBlock(spDataObject& _data, NonceMap& _nonceMap)
  : m_chainName(BlockchainTestFillerBlock::defaultChainName())
{
    try
    {
        REQUIRE_JSONFIELDS(_data, "BlockchainTestFillerBlock " + _data->getKey(),
            {{"rlp", {{DataType::String}, jsonField::Optional}},
                {"chainname", {{DataType::String}, jsonField::Optional}},
                {"donotimportonclient", {{DataType::String}, jsonField::Optional}},
                {"blocknumber", {{DataType::String}, jsonField::Optional}},
                {"chainnetwork", {{DataType::String}, jsonField::Optional}},
                {"transactions", {{DataType::Array}, jsonField::Optional}},
                {"uncleHeaders", {{DataType::Array}, jsonField::Optional}},
                {"expectException", {{DataType::Object}, jsonField::Optional}},
                {"blockHeader", {{DataType::Object}, jsonField::Optional}}});

        if (_data->count("rlp"))
        {
            m_rlp = spBYTES(new BYTES(_data->atKey("rlp")));
            ETH_ERROR_REQUIRE_MESSAGE(_data->getSubObjects().size() <= 2,
                "Block described as raw rlp can have only `rlp` and `blockHeader` sections!");
        }

        if (_data->count("chainname"))
            m_chainName = _data->atKey("chainname").asString();
        else
            m_chainName = BlockchainTestFillerBlock::defaultChainName();

        m_doNotImportOnClient = _data->count("donotimportonclient");

        if (_data->count("blocknumber"))
        {
            DataObject& bNumber = _data.getContent().atKeyUnsafe("blocknumber");
            bNumber.performModifier(mod_valueToCompactEvenHexPrefixed);
            m_blockNumber = spVALUE(new VALUE(bNumber));
        }

        if (_data->count("chainnetwork"))
            m_network = spFORK(new FORK(_data->atKey("chainnetwork")));

        if (_data->count("transactions"))
            for (auto& tr : (*_data).atKeyUnsafe("transactions").getSubObjectsUnsafe())
                m_transactions.push_back(BlockchainTestFillerTransaction(dataobject::move(tr), _nonceMap));

        if (_data->count("uncleHeaders"))
            for (auto const& un : _data->atKey("uncleHeaders").getSubObjects())
                m_uncles.push_back(BlockchainTestFillerUncle(un));

        if (_data->count("expectException"))
            readExpectExceptions(_data->atKey("expectException"), m_expectExceptions);

        if (_data->count("blockHeader"))
        {
            if (_data->atKey("blockHeader").getSubObjects().size() &&
                _data->atKey("blockHeader").getSubObjects().at(0)->type() == DataType::Object)
            {
                // Read overwrite rules specific to fork
                for (auto const& rec : _data->atKey("blockHeader").getSubObjects())
                {
                    // Parse ">=Frontier" : "BlockHeaderOverwrite"
                    ClientConfig const& cfg = Options::get().getDynamicOptions().getCurrentConfig();
                    std::set<string> forksString = {rec->getKey()};
                    std::vector<FORK> parsedForks = cfg.translateNetworks(forksString);
                    for (auto const& el : parsedForks)
                        m_overwriteHeaderByForkMap.emplace(el, spBlockHeaderOverwrite(new BlockHeaderOverwrite(rec)));
                }
            }
            else
            {
                // Treat the whole blockHeader section as the overwrite rule for all forks
                ClientConfig const& cfg = Options::get().getDynamicOptions().getCurrentConfig();
                spBlockHeaderOverwrite overwrite(new BlockHeaderOverwrite(_data->atKey("blockHeader")));
                for (auto const& el : cfg.cfgFile().allowedForks())
                    m_overwriteHeaderByForkMap.emplace(el, overwrite);
            }
        }
    }
    catch (std::exception const& _ex)
    {
        throw UpwardsException(string("BlockchainTestFillerBlock convertion error: ") + _ex.what() + _data->asJson());
    }
}

}  // namespace teststruct
