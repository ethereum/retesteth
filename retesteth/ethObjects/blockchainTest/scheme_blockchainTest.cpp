#include "scheme_blockchainTest.h"
using namespace test;

string const scheme_blockchainTest::m_sNoProof = "NoPfoof";

scheme_blockchainTestBase::fieldChecker::fieldChecker(DataObject const& _test)
{
    requireJsonFields(_test, "blockchainTestBase " + _test.getKey(),
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

scheme_blockchainTest::fieldChecker::fieldChecker(DataObject const& _test, bool _isLegacyTest)
{
    requireJsonFields(_test, "blockchainTest" + _test.getKey(),
        {{"_info", {{DataType::Object}, jsonField::Required}},
            {"blocks", {{DataType::Array}, jsonField::Required}},
            {"genesisBlockHeader", {{DataType::Object}, jsonField::Required}},
            {"genesisRLP",
                {{DataType::String}, _isLegacyTest ? jsonField::Optional : jsonField::Required}},
            {"lastblockhash", {{DataType::String}, jsonField::Required}},
            {"network", {{DataType::String}, jsonField::Required}},
            {"postState", {{DataType::Object}, jsonField::Optional}},
            {"postStateHash", {{DataType::String}, jsonField::Optional}},
            {"pre", {{DataType::Object}, jsonField::Required}},
            {"sealEngine", {{DataType::String}, jsonField::Optional}}});
}

scheme_blockchainTestBase::scheme_blockchainTestBase(DataObject const& _test, bool _isLegacyTest)
  : object(_test),
    m_checker(_test),
    m_pre(_test.atKey("pre")),
    m_genesisHeader(_test.atKey("genesisBlockHeader")),
    m_isLegacyTest(_isLegacyTest)
{
    if (_test.count("sealEngine"))
        m_sealEngine = _test.atKey("sealEngine").asString();
    else
        m_sealEngine = "NoProof";
}

scheme_blockchainTest::scheme_blockchainTest(DataObject const& _test, bool _isLegacyTest)
  : scheme_blockchainTestBase(_test, _isLegacyTest),
    m_checker(_test, _isLegacyTest),
    m_post(_test.count("postState") ? _test.atKey("postState") : _test.atKey("postStateHash"))
{
    // Validate blocks section at the filled blockchain test
    for (auto const& blockSection : _test.atKey("blocks").getSubObjects())
    {
        // Valid block json description
        if (blockSection.count("blockHeader"))
        {
            requireJsonFields(blockSection, "blockchainTestValidblock " + _test.getKey(),
                {{"blockHeader", {{DataType::Object}, jsonField::Required}},
                    {"rlp", {{DataType::String}, jsonField::Required}},
                    {"transactions", {{DataType::Array}, jsonField::Required}},
                    {"uncleHeaders", {{DataType::Array}, jsonField::Optional}},
                    {"blocknumber", {{DataType::String}, jsonField::Optional}},
                    {"chainname", {{DataType::String}, jsonField::Optional}},
                    {"chainnetwork", {{DataType::String}, jsonField::Optional}}});
            scheme_blockHeader(blockSection.atKey("blockHeader"));
            for (auto const& trSection: blockSection.atKey("transactions").getSubObjects())
                (void)scheme_transaction(trSection);
        }
        else
        {
            // Invalid block json description
            if (_isLegacyTest)
                requireJsonFields(blockSection,
                    "blockchainTestInvalidblock Legacy " + _test.getKey(),
                    {{"rlp", {{DataType::String}, jsonField::Required}},
                        {"blocknumber", {{DataType::String}, jsonField::Optional}},
                        {"chainname", {{DataType::String}, jsonField::Optional}},
                        {"expectException", {{DataType::String}, jsonField::Optional}},
                        {"chainnetwork", {{DataType::String}, jsonField::Optional}},
                        {"expectExceptionALL", {{DataType::String}, jsonField::Optional}},
                        {"expectExceptionByzantium", {{DataType::String}, jsonField::Optional}},
                        {"expectExceptionHomestead", {{DataType::String}, jsonField::Optional}},
                        {"expectExceptionEIP150", {{DataType::String}, jsonField::Optional}},
                        {"expectExceptionEIP158", {{DataType::String}, jsonField::Optional}},
                        {"expectExceptionFrontier", {{DataType::String}, jsonField::Optional}},
                        {"expectExceptionConstantinople",
                            {{DataType::String}, jsonField::Optional}},
                        {"expectExceptionConstantinopleFix",
                            {{DataType::String}, jsonField::Optional}},
                        {"expectExceptionIstanbul", {{DataType::String}, jsonField::Optional}}});
            else
                requireJsonFields(blockSection, "blockchainTestInvalidblock " + _test.getKey(),
                    {{"rlp", {{DataType::String}, jsonField::Required}},
                        {"blocknumber", {{DataType::String}, jsonField::Optional}},
                        {"chainname", {{DataType::String}, jsonField::Optional}},
                        {"expectException", {{DataType::String}, jsonField::Optional}},
                        {"chainnetwork", {{DataType::String}, jsonField::Optional}}});
       }
    }
    makeAllFieldsHex(m_data);
}

DataObject scheme_blockchainTestBase::getGenesisForRPC(
    string const& _network, string const& _sealEngine) const
{
    string net = (_network.empty() ? getData().atKey("network").asString() : _network);
    DataObject genesis = prepareGenesisParams(net, m_sealEngine);

    DataObject data;
    data["author"] = m_genesisHeader.getData().atKey("coinbase");
    data["difficulty"] = m_genesisHeader.getData().atKey("difficulty");
    data["gasLimit"] = m_genesisHeader.getData().atKey("gasLimit");
    data["extraData"] = m_genesisHeader.getData().atKey("extraData");
    data["timestamp"] = m_genesisHeader.getData().atKey("timestamp");

    if (m_isLegacyTest)
    {
        data["nonce"] = m_genesisHeader.getData().atKey("nonce");
        data["mixHash"] = m_genesisHeader.getData().atKey("mixHash");
    }
    else
    {
        data["nonce"] = (_sealEngine == "NoProof") ? DataObject("0x0000000000000000") :
                                                     m_genesisHeader.getData().atKey("nonce");
        data["mixHash"] =
            (_sealEngine == "NoProof") ?
                DataObject("0x0000000000000000000000000000000000000000000000000000000000000000") :
                m_genesisHeader.getData().atKey("mixHash");
    }

    object::makeAllFieldsHex(data);
    genesis["genesis"] = data;
    for (auto const& acc : m_pre.getData().getSubObjects())
        genesis["accounts"].addSubObject(acc);
    return genesis;
}
