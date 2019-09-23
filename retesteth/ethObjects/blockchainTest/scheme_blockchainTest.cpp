#include "scheme_blockchainTest.h"
using namespace test;


scheme_blockchainTestBase::fieldChecker::fieldChecker(DataObject const& _test)
{
    requireJsonFields(_test, "blockchainTest " + _test.getKey(),
        {{"_info", {{DataType::Object}, jsonField::Optional}},
            {"blocks", {{DataType::Array}, jsonField::Required}},
            {"expect", {{DataType::Array}, jsonField::Optional}},
            {"genesisBlockHeader", {{DataType::Object}, jsonField::Required}},
            {"genesisRLP", {{DataType::String}, jsonField::Optional}},
            {"lastblockhash", {{DataType::String}, jsonField::Optional}},
            {"network", {{DataType::String}, jsonField::Optional}},
            {"hash", {{DataType::String}, jsonField::Optional}},
            {"gasUsed", {{DataType::String}, jsonField::Optional}},
            {"postState", {{DataType::Object}, jsonField::Optional}},
            {"postStateHash", {{DataType::String}, jsonField::Optional}},
            {"pre", {{DataType::Object}, jsonField::Required}},
            {"sealEngine", {{DataType::String}, jsonField::Optional}}});
}

scheme_blockchainTest::fieldChecker::fieldChecker(DataObject const& _test)
{
    requireJsonFields(_test, "blockchainTest " + _test.getKey(),
        {{"_info", {{DataType::Object}, jsonField::Required}},
            {"blocks", {{DataType::Array}, jsonField::Required}},
            {"genesisBlockHeader", {{DataType::Object}, jsonField::Required}},
            {"genesisRLP", {{DataType::String}, jsonField::Optional}},
            {"lastblockhash", {{DataType::String}, jsonField::Required}},
            {"network", {{DataType::String}, jsonField::Required}},
            {"postState", {{DataType::Object}, jsonField::Optional}},
            {"postStateHash", {{DataType::String}, jsonField::Optional}},
            {"pre", {{DataType::Object}, jsonField::Required}},
            {"sealEngine", {{DataType::String}, jsonField::Optional}}});
}

scheme_blockchainTestBase::scheme_blockchainTestBase(DataObject const& _test)
  : object(_test),
    m_checker(_test),
    m_pre(_test.atKey("pre")),
    m_genesisHeader(_test.atKey("genesisBlockHeader"))
{
    if (_test.count("sealEngine"))
        m_sealEngine = _test.atKey("sealEngine").asString();
    else
        m_sealEngine = "NoProof";
}

scheme_blockchainTest::scheme_blockchainTest(DataObject const& _test)
  : scheme_blockchainTestBase(_test), m_checker(_test),
    m_post(_test.count("postState") ? _test.atKey("postState") : _test.atKey("postStateHash"))
{
    // Validate blocks section at the filled blockchain test
    for (auto const& blockSection : _test.atKey("blocks").getSubObjects())
    {
        requireJsonFields(blockSection, "blockchainTest " + _test.getKey(),
            {   {"blockHeader", {{DataType::Object}, jsonField::Required}},
                {"rlp", {{DataType::String}, jsonField::Required}},
                {"transactions", {{DataType::Array}, jsonField::Required}},
                {"uncleHeaders", {{DataType::Array}, jsonField::Optional}},
                {"blocknumber", {{DataType::String}, jsonField::Optional}},
                {"chainname", {{DataType::String}, jsonField::Optional}}});

        scheme_blockHeader(blockSection.atKey("blockHeader"));
        m_blockRLPs.push_back(blockSection.atKey("rlp").asString());
        for (auto const& trSection: blockSection.atKey("transactions").getSubObjects())
            m_transactions.push_back(scheme_transaction(trSection));
    }
    makeAllFieldsHex(m_data);
}

DataObject scheme_blockchainTestBase::getGenesisForRPC(string const& _network) const
{
    string net = (_network.empty() ? getData().atKey("network").asString() : _network);
    DataObject genesis = prepareGenesisParams(net, m_sealEngine);

    DataObject data;
    data["author"] = m_genesisHeader.getData().atKey("coinbase");
    data["difficulty"] = m_genesisHeader.getData().atKey("difficulty");
    data["gasLimit"] = m_genesisHeader.getData().atKey("gasLimit");
    data["nonce"] = m_genesisHeader.getData().atKey("nonce");
    data["extraData"] = m_genesisHeader.getData().atKey("extraData");
    data["timestamp"] = m_genesisHeader.getData().atKey("timestamp");
    data["mixHash"] = m_genesisHeader.getData().atKey("mixHash");
    object::makeAllFieldsHex(data);

    genesis["genesis"] = data;
    for (auto const& acc : m_pre.getData().getSubObjects())
        genesis["accounts"].addSubObject(acc);
    return genesis;
}
