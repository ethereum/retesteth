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

    // blockchain tests data for block hash
    // data["bloom"] = m_genesisHeader.getData().at("bloom");
    // data["gasUsed"] = m_genesisHeader.getData().at("gasUsed");
    // data["hash"] = m_genesisHeader.getData().at("hash");
    // data["number"] = m_genesisHeader.getData().at("number");
    // data["parentHash"] = m_genesisHeader.getData().at("parentHash");
    // data["receiptTrie"] = m_genesisHeader.getData().at("receiptTrie");
    // data["stateRoot"] = m_genesisHeader.getData().at("stateRoot");
    // data["transactionsTrie"] = m_genesisHeader.getData().at("transactionsTrie");
    // data["uncleHash"] = m_genesisHeader.getData().at("uncleHash");

    genesis["genesis"] = data;
    genesis["accounts"] = m_pre.getDataForRPC(getData().at("network").asString());
    return genesis;
}
