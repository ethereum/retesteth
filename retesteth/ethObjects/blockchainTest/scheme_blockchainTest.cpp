#include "scheme_blockchainTest.h"
using namespace test;

scheme_blockchainTest::fieldChecker::fieldChecker(DataObject const& _test)
{
    requireJsonFields(_test, "blockchainTest " + _test.getKey(),
        {{"_info", {DataType::Object}}, {"blocks", {DataType::Array}},
            {"genesisBlockHeader", {DataType::Object}}, {"genesisRLP", {DataType::String}},
            {"lastblockhash", {DataType::String}}, {"network", {DataType::String}},
            {"postState", {DataType::Object}}, {"pre", {DataType::Object}}},
        {"genesisRLP"});
}

scheme_blockchainTest::scheme_blockchainTest(DataObject const& _test)
  : object(_test),
    m_checker(_test),
    m_pre(_test.at("pre")),
    m_post(_test.at("postState")),
    m_genesisHeader(_test.at("genesisBlockHeader"))
{
    for (auto const& data : _test.at("blocks").getSubObjects())
    {
        ETH_CHECK_MESSAGE(data.count("rlp"), "Block element missing rlp field!");
        ETH_CHECK_MESSAGE(
            data.at("rlp").type() == DataType::String, "Block rlp field must be string!");
        m_blockRLPs.push_back(data.at("rlp").asString());
    }
}

DataObject scheme_blockchainTest::getGenesisForRPC()
{
    DataObject genesis = prepareGenesisParams(getData().at("network").asString());
    // genesis["genesis"] = getEnv().getDataForRPC();

    DataObject data;
    data["author"] = m_genesisHeader.getData().at("coinbase");
    data["difficulty"] = m_genesisHeader.getData().at("difficulty");
    data["gasLimit"] = m_genesisHeader.getData().at("gasLimit");
    data["nonce"] = m_genesisHeader.getData().at("nonce");
    data["extraData"] = m_genesisHeader.getData().at("extraData");
    data["timestamp"] = m_genesisHeader.getData().at("timestamp");
    data["mixHash"] = m_genesisHeader.getData().at("mixHash");

    genesis["genesis"] = data;
    genesis["accounts"] = m_pre.getDataForRPC(getData().at("network").asString());
    return genesis;
}
