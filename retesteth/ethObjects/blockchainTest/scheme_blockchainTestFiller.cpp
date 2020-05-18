#include "scheme_blockchainTestFiller.h"
using namespace test;

scheme_blockchainTestFiller::fieldChecker::fieldChecker(DataObject const& _test)
{
    requireJsonFields(_test, "blockchainTestFiller " + _test.getKey(),
        {{"_info", {{DataType::Object}, jsonField::Optional}},
            {"blocks", {{DataType::Array}, jsonField::Required}},
            {"expect", {{DataType::Array}, jsonField::Required}},
            {"genesisBlockHeader", {{DataType::Object}, jsonField::Required}},
            {"pre", {{DataType::Object}, jsonField::Required}},
            {"sealEngine", {{DataType::String}, jsonField::Optional}},
            {"exceptions", {{DataType::Array}, jsonField::Optional}}});
}

scheme_blockchainTestFiller::scheme_blockchainTestFiller(DataObject const& _test)
  : scheme_blockchainTestBase(_test), m_checker(_test), m_expectSection(_test.atKey("expect"))
{
    m_totalUncleCount = 0;
    for (auto const& bl : m_data.atKey("blocks").getSubObjects())
    {
        blockSection block = blockSection(bl);
        m_blocks.push_back(block);
        m_totalUncleCount += block.getUncles().size();
    }
}

void scheme_blockchainTestFiller::blockSection::parseBlockHeaderException(DataObject const& _data)
{
    (void)_data;
    /*
    if (!_data.count("blockHeader"))
        return;
    if (_data.atKey("blockHeader").count("expectException"))
    {
        ClientConfig const& cfg = Options::get().getDynamicOptions().getCurrentConfig();
        std::map<std::string, std::string> translatedNetsExceptions;
        for (auto const& expect :
            _data.atKey("blockHeader").atKey("expectException").getSubObjects())
        {
            if (inArray(cfg.getAdditionalNetworks(), expect.getKey()))
            {
                translatedNetsExceptions.emplace(expect.getKey(), expect.asString());
                continue;
            }

            std::set<string> const nets = translateNetworks({expect.getKey()}, cfg.getNetworks());
            for (auto const& net : nets)
                translatedNetsExceptions.emplace(net, expect.asString());
        }

        for (auto const& expect : translatedNetsExceptions)
        {
            checkAllowedNetwork(expect.first, cfg.getNetworksPlusAdditional());
            m_expectException[expect.first] = expect.second;
        }
    }*/
}

string scheme_blockchainTestFiller::blockSection::m_defaultChainName = "default";
scheme_blockchainTestFiller::blockSection::blockSection(DataObject const& _data)
  : object(_data), m_noExceptionString("NoException")
{
    /*
    if (_data.getSubObjects().size() <= 2 && _data.count("rlp"))
    {
        requireJsonFields(_data, "blockchainTestFiller::blocks section",
            {{"rlp", {{DataType::String}, jsonField::Required}},
                {"blockHeader", {{DataType::Object}, jsonField::Optional}}});
        parseBlockHeaderException(_data);
        return;
    }

    requireJsonFields(_data, "blockchainTestFiller::blocks section",
        {{"blockHeader", {{DataType::Object}, jsonField::Optional}},
            {"blockHeaderPremine", {{DataType::Object}, jsonField::Optional}},
            {"blocknumber", {{DataType::String}, jsonField::Optional}},
            {"chainname", {{DataType::String}, jsonField::Optional}},
            {"chainnetwork", {{DataType::String}, jsonField::Optional}},
            {"donotimportonclient", {{DataType::String}, jsonField::Optional}},
            {"transactions", {{DataType::Array}, jsonField::Required}},
            {"uncleHeaders", {{DataType::Array}, jsonField::Required}}});

    if (m_data.count("chainnetwork"))
    {
        // Chain network can be any of Forknames + additional Forks
        ClientConfig const& cfg = Options::get().getDynamicOptions().getCurrentConfig();
        m_chainNetwork = m_data.atKey("chainnetwork").asString();
        test::checkAllowedNetwork(m_chainNetwork, cfg.getNetworksPlusAdditional());
    }

    if (_data.count("blockHeader"))
    {
        requireJsonFields(_data.atKey("blockHeader"),
            "blockchainTestFiller::blocks::blockHeader section",
            {{"extraData", {{DataType::String}, jsonField::Optional}},
                {"difficulty", {{DataType::String}, jsonField::Optional}},
                {"gasLimit", {{DataType::String}, jsonField::Optional}},
                {"number", {{DataType::String}, jsonField::Optional}},
                {"bloom", {{DataType::String}, jsonField::Optional}},
                {"coinbase", {{DataType::String}, jsonField::Optional}},
                {"gasUsed", {{DataType::String}, jsonField::Optional}},
                {"mixHash", {{DataType::String}, jsonField::Optional}},
                {"timestamp", {{DataType::String}, jsonField::Optional}},
                {"nonce", {{DataType::String}, jsonField::Optional}},
                {"parentHash", {{DataType::String}, jsonField::Optional}},
                {"receiptTrie", {{DataType::String}, jsonField::Optional}},
                {"stateRoot", {{DataType::String}, jsonField::Optional}},
                {"transactionsTrie", {{DataType::String}, jsonField::Optional}},
                {"uncleHash", {{DataType::String}, jsonField::Optional}},
                {"RelTimestamp", {{DataType::String}, jsonField::Optional}},
                {"updatePoW", {{DataType::String}, jsonField::Optional}},
                {"expectException", {{DataType::Object}, jsonField::Optional}}});

        parseBlockHeaderException(_data);
    }

    for (auto const& tr : _data.atKey("transactions").getSubObjects())
        m_transactons.push_back(tr);

    for (auto const& uncle : _data.atKey("uncleHeaders").getSubObjects())
        m_uncles.push_back(uncle);
        */
}

string scheme_blockchainTestFiller::getSealEngine() const
{
    if (m_data.count("sealEngine"))
        return m_data.atKey("sealEngine").asString();
    else
        return scheme_blockchainTestBase::m_sNoProof;
}
