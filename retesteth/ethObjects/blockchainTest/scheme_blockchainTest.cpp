#include "scheme_blockchainTest.h"
using namespace test;

scheme_blockchainTest::fieldChecker::fieldChecker(DataObject const& _test)
{
    requireJsonFields(_test, "blockchainTest " + _test.getKey(),
        {{"_info", {{DataType::Object}, jsonField::Required}},
            {"blocks", {{DataType::Array}, jsonField::Required}},
            {"genesisBlockHeader", {{DataType::Object}, jsonField::Required}},
            {"genesisRLP", {{DataType::String}, jsonField::Optional}},
            {"lastblockhash", {{DataType::String}, jsonField::Required}},
            {"network", {{DataType::String}, jsonField::Required}},
            {"postState", {{DataType::Object}, jsonField::Required}},
            {"pre", {{DataType::Object}, jsonField::Required}},
            {"sealEngine", {{DataType::String}, jsonField::Optional}}});
}

scheme_blockchainTest::scheme_blockchainTest(DataObject const& _test)
  : object(_test),
    m_checker(_test),
    m_pre(_test.atKey("pre")),
    m_post(_test.atKey("postState")),
    m_genesisHeader(_test.atKey("genesisBlockHeader"))
{
    for (auto const& data : _test.atKey("blocks").getSubObjects())
    {
        ETH_ERROR_REQUIRE_MESSAGE(data.count("rlp"), "Block element missing rlp field!");
        ETH_ERROR_REQUIRE_MESSAGE(
            data.atKey("rlp").type() == DataType::String, "Block rlp field must be string!");
        m_blockRLPs.push_back(data.atKey("rlp").asString());
    }

    if (_test.count("sealEngine"))
        m_sealEngine = _test.atKey("sealEngine").asString();
    else
        m_sealEngine = "NoProof";
}

DataObject scheme_blockchainTest::getGenesisForRPC()
{
    DataObject genesis = prepareGenesisParams(getData().atKey("network").asString(), m_sealEngine);
    // genesis["genesis"] = getEnv().getDataForRPC();

    DataObject data;
    data["author"] = m_genesisHeader.getData().atKey("coinbase");
    data["difficulty"] = m_genesisHeader.getData().atKey("difficulty");
    data["gasLimit"] = m_genesisHeader.getData().atKey("gasLimit");
    data["nonce"] = m_genesisHeader.getData().atKey("nonce");
    data["extraData"] = m_genesisHeader.getData().atKey("extraData");
    data["timestamp"] = m_genesisHeader.getData().atKey("timestamp");
    data["mixHash"] = m_genesisHeader.getData().atKey("mixHash");

    genesis["genesis"] = data;
    genesis["accounts"] = m_pre.getDataForRPC(getData().atKey("network").asString());
    return genesis;
}
