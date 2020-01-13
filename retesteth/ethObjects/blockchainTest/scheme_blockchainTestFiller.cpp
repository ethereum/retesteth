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
            {"sealEngine", {{DataType::String}, jsonField::Optional}}});
}

scheme_blockchainTestFiller::scheme_blockchainTestFiller(DataObject const& _test)
  : scheme_blockchainTestBase(_test), m_checker(_test), m_expectSection(_test.atKey("expect"))
{
    for (auto const& bl : m_data.atKey("blocks").getSubObjects())
        m_blocks.push_back(blockSection(bl));
}

scheme_blockchainTestFiller::blockSection::blockSection(DataObject const& _data) : object(_data)
{
    requireJsonFields(_data, "blockchainTestFiller::blocks section",
        {{"blockHeader", {{DataType::Object}, jsonField::Optional}},
            {"blockHeaderPremine", {{DataType::Object}, jsonField::Optional}},
            {"blocknumber", {{DataType::String}, jsonField::Optional}},
            {"chainname", {{DataType::String}, jsonField::Optional}},
            {"transactions", {{DataType::Array}, jsonField::Required}},
            {"uncleHeaders", {{DataType::Array}, jsonField::Required}}});

    for (auto const& tr : _data.atKey("transactions").getSubObjects())
        m_transactons.push_back(tr);

    m_noExceptionString = "NoException";
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

        if (_data.atKey("blockHeader").count("expectException"))
        {
            for (auto const& expect :
                _data.atKey("blockHeader").atKey("expectException").getSubObjects())
            {
                checkAllowedNetwork(
                    expect.getKey(), Options::getDynamicOptions().getCurrentConfig().getNetworks());
                m_expectException[expect.getKey()] = expect.asString();
            }
        }
    }

    for (auto const& uncle : _data.atKey("uncleHeaders").getSubObjects())
        m_uncles.push_back(uncle);
}

string scheme_blockchainTestFiller::getSealEngine() const
{
    if (m_data.count("sealEngine"))
        return m_data.atKey("sealEngine").asString();
    else
        return "NoProof";
}
