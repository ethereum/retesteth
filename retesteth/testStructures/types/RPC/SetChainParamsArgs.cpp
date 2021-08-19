#include "SetChainParamsArgs.h"
#include <retesteth/EthChecks.h>
#include <retesteth/testStructures/Common.h>

namespace test
{
namespace teststruct
{
SetChainParamsArgs::SetChainParamsArgs(spDataObject& _data)
{
    requireJsonFields(_data, "SetChainParamsArgs " + _data->getKey(),
        {{"params", {{DataType::Object}, jsonField::Required}},
            {"accounts", {{DataType::Object}, jsonField::Required}},
            {"genesis", {{DataType::Object}, jsonField::Required}},
            {"sealEngine", {{DataType::String}, jsonField::Required}}});

    m_params = MOVE(_data, "params").getPointer();
    m_preState = spState(new State(MOVE(_data, "accounts")));
    m_sealEngine = sealEngineFromStr(_data->atKey("sealEngine").asString());

    // Fill up required fields for blocheader info
    DataObject const& genesis = _data->atKey("genesis");
    spDataObject _fullBlockHeader(new DataObject());
    DataObject& fullBlockHeader = _fullBlockHeader.getContent();
    fullBlockHeader["author"] = genesis.atKey("author").asString();
    fullBlockHeader["difficulty"] = genesis.atKey("difficulty").asString();
    fullBlockHeader["gasLimit"] = genesis.atKey("gasLimit").asString();
    if (genesis.count("baseFeePerGas"))
        fullBlockHeader["baseFeePerGas"] = genesis.atKey("baseFeePerGas").asString();

    fullBlockHeader["extraData"] = genesis.atKey("extraData").asString();
    fullBlockHeader["timestamp"] = genesis.atKey("timestamp").asString();
    fullBlockHeader["nonce"] = genesis.atKey("nonce").asString();
    fullBlockHeader["mixHash"] = genesis.atKey("mixHash").asString();

    // Fields that are ommited in RPC setChainParams, use default fields of empty block
    fullBlockHeader["bloom"] = FH256::zero().asString();
    fullBlockHeader["gasUsed"] = "0x00";
    fullBlockHeader["number"] = "0x00";
    fullBlockHeader["receiptTrie"] = "0x56e81f171bcc55a6ff8345e692c0f86e5b48e01b996cadc001622fb5e363b421";
    fullBlockHeader["stateRoot"] = FH32::zero().asString();
    fullBlockHeader["transactionsTrie"] = "0x56e81f171bcc55a6ff8345e692c0f86e5b48e01b996cadc001622fb5e363b421";
    fullBlockHeader["uncleHash"] = "0x1dcc4de8dec75d7aab85b567b6ccd41ad312451b948a7413f0a142fd40d49347";
    fullBlockHeader["parentHash"] = FH32::zero().asString();

    m_genesis = readBlockHeader(fullBlockHeader);
}

spDataObject SetChainParamsArgs::asDataObject() const
{
    spDataObject out (new DataObject());
    (*out)["params"].copyFrom(m_params);
    (*out).atKeyPointer("accounts") = m_preState->asDataObject();
    (*out)["sealEngine"] = sealEngineToStr(m_sealEngine);
    (*out)["genesis"]["author"] = m_genesis->author().asString();
    (*out)["genesis"]["difficulty"] = m_genesis->difficulty().asString();
    (*out)["genesis"]["gasLimit"] = m_genesis->gasLimit().asString();

    if (m_genesis->type() == BlockType::BlockHeader1559)
    {
        BlockHeader1559 const& newbl = BlockHeader1559::castFrom(m_genesis);
        (*out)["genesis"]["baseFeePerGas"] = newbl.baseFee().asString();
    }

    (*out)["genesis"]["extraData"] = m_genesis->extraData().asString();
    (*out)["genesis"]["timestamp"] = m_genesis->timestamp().asString();
    (*out)["genesis"]["nonce"] = m_genesis->nonce().asString();
    (*out)["genesis"]["mixHash"] = m_genesis->mixHash().asString();
    return out;
}

}  // namespace teststruct
}  // namespace test
