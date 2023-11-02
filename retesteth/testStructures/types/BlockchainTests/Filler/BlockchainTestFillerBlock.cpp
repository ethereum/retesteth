#include "BlockchainTestFillerBlock.h"
#include <retesteth/EthChecks.h>
#include <retesteth/testStructures/Common.h>
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
                {"donotstackvalidtrxs", {{DataType::String}, jsonField::Optional}},
                {"blocknumber", {{DataType::String}, jsonField::Optional}},
                {"chainnetwork", {{DataType::String}, jsonField::Optional}},
                {"transactions", {{DataType::Array}, jsonField::Optional}},
                {"uncleHeaders", {{DataType::Array}, jsonField::Optional}},
                {"withdrawals", {{DataType::Array}, jsonField::Optional}},
                {"expectException", {{DataType::Object}, jsonField::Optional}},
                {"blockHeader", {{DataType::Object}, jsonField::Optional}}});

        m_hasBigInt = _data->performSearch(src_findBigInt);
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
        m_doNotStackValidTrxs = _data->count("donotstackvalidtrxs");

        if (_data->count("blocknumber"))
        {
            DataObject& bNumber = _data.getContent().atKeyUnsafe("blocknumber");
            bNumber.performModifier(mod_valueToCompactEvenHexPrefixed);
            m_blockNumber = spVALUE(new VALUE(bNumber));
        }

        if (_data->count("chainnetwork"))
            m_network = spFORK(new FORK(_data->atKey("chainnetwork")));

        if (_data->count("transactions"))
        {
            string const c_transactions = "transactions";
            m_transactions.reserve(_data->atKey(c_transactions).getSubObjects().size());
            for (auto& tr : (*_data).atKeyUnsafe(c_transactions).getSubObjectsUnsafe())
                m_transactions.emplace_back(BlockchainTestFillerTransaction(dataobject::move(tr), _nonceMap));
        }

        if (_data->count("withdrawals"))
            for (auto& wt : (*_data).atKeyUnsafe("withdrawals").getSubObjectsUnsafe())
                m_withdrawals.emplace_back(BlockchainTestFillerWithdrawal(dataobject::move(wt)));

        if (_data->count("uncleHeaders"))
            for (auto const& un : _data->atKey("uncleHeaders").getSubObjects())
                m_uncles.emplace_back(BlockchainTestFillerUncle(un));

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

BlockchainTestFillerBlock::BlockchainTestFillerBlock(BlockchainTestFillerBlock const& _other, bool _managed)
{
    _managed = true;
    (void) _managed;
    m_chainName = _other.chainName();

    //spBYTES m_rlp;
    if (_other.hasNumber())
        m_blockNumber = sVALUE(VALUE(_other.number()));

    if (_other.hasChainNet())
        m_network = spFORK(new FORK(_other.chainNet()));

    //bool m_hasBigInt = false;
    m_doNotImportOnClient = _other.isDoNotImportOnClient();

    for (auto const& un : _other.uncles())
        m_uncles.emplace_back(un);

    //std::vector<BlockchainTestFillerTransaction> m_transactions;

    for (auto const& wt : _other.withdrawals())
        m_withdrawals.emplace_back(wt);

    //m_expectExceptions = _other.getExpectExceptions();
    m_overwriteHeaderByForkMap = _other.getHeaderOverwriteMap();
}

void BlockchainTestFillerBlock::addException(FORK const& _net, std::string const& _ex)
{
    m_expectExceptions.emplace(_net, _ex);
}

}  // namespace teststruct
