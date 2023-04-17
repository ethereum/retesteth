#include "SetChainParamsArgs.h"
#include <retesteth/testStructures/Common.h>
#include <retesteth/Constants.h>

using namespace dataobject;
using namespace test::teststruct;

namespace
{
void requireSetChainParamsScheme(DataObject const& _data)
{
    REQUIRE_JSONFIELDS(_data, "SetChainParamsArgs " + _data.getKey(),
        {{"params", {{DataType::Object}, jsonField::Required}},
            {"accounts", {{DataType::Object}, jsonField::Required}},
            {"genesis", {{DataType::Object}, jsonField::Required}},
            {"sealEngine", {{DataType::String}, jsonField::Required}}});
}

spSetChainParamsArgsGenesis readGenesis(DataObject const& _data)
{
    if (_data.count("baseFeePerGas"))
    {
        if (_data.count("currentRandom"))
        {
            if (_data.count("withdrawalsRoot"))
                return spSetChainParamsArgsGenesis(new SetChainParamsArgsGenesisShanghai(_data));
            else
                return spSetChainParamsArgsGenesis(new SetChainParamsArgsGenesisMerge(_data));
        }
        else
            return spSetChainParamsArgsGenesis(new SetChainParamsArgsGenesis1559(_data));
    }
    return spSetChainParamsArgsGenesis(new SetChainParamsArgsGenesis(_data));
}

}

namespace test
{
namespace teststruct
{

SetChainParamsArgsGenesis::SetChainParamsArgsGenesis(DataObject const& _data)
  : m_dataRef(_data)
{
    REQUIRE_JSONFIELDS(_data, "SetChainParamsArgs::genesis(legacy) ",
        {
            {"author", {{DataType::String}, jsonField::Required}},
            {"difficulty", {{DataType::String}, jsonField::Required}},
            {"gasLimit", {{DataType::String}, jsonField::Required}},
            {"extraData", {{DataType::String}, jsonField::Required}},
            {"timestamp", {{DataType::String}, jsonField::Required}},
            {"nonce", {{DataType::String}, jsonField::Required}},
            {"mixHash", {{DataType::String}, jsonField::Required}}
        });
}

SetChainParamsArgsGenesis1559::SetChainParamsArgsGenesis1559(DataObject const& _data)
    : SetChainParamsArgsGenesis(_data, false)
{
    REQUIRE_JSONFIELDS(_data, "SetChainParamsArgs::genesis(1559) ",
        {
            {"author", {{DataType::String}, jsonField::Required}},
            {"difficulty", {{DataType::String}, jsonField::Required}},
            {"gasLimit", {{DataType::String}, jsonField::Required}},
            {"baseFeePerGas", {{DataType::String}, jsonField::Required}},
            {"extraData", {{DataType::String}, jsonField::Required}},
            {"timestamp", {{DataType::String}, jsonField::Required}},
            {"nonce", {{DataType::String}, jsonField::Required}},
            {"mixHash", {{DataType::String}, jsonField::Required}}
        });
}

SetChainParamsArgsGenesisMerge::SetChainParamsArgsGenesisMerge(DataObject const& _data)
  : SetChainParamsArgsGenesis(_data, false)
{
    REQUIRE_JSONFIELDS(_data, "SetChainParamsArgs::genesis(Merge) ",
        {
            {"author", {{DataType::String}, jsonField::Required}},
            {"gasLimit", {{DataType::String}, jsonField::Required}},
            {"baseFeePerGas", {{DataType::String}, jsonField::Required}},
            {"currentRandom", {{DataType::String}, jsonField::Required}},
            {"extraData", {{DataType::String}, jsonField::Required}},
            {"timestamp", {{DataType::String}, jsonField::Required}},
            {"nonce", {{DataType::String}, jsonField::Required}},
            {"mixHash", {{DataType::String}, jsonField::Required}}
        });
}

SetChainParamsArgsGenesisShanghai::SetChainParamsArgsGenesisShanghai(DataObject const& _data)
  : SetChainParamsArgsGenesis(_data, false)
{
    REQUIRE_JSONFIELDS(_data, "SetChainParamsArgs::genesis(Shanghai) ",
        {
            {"author", {{DataType::String}, jsonField::Required}},
            {"gasLimit", {{DataType::String}, jsonField::Required}},
            {"baseFeePerGas", {{DataType::String}, jsonField::Required}},
            {"currentRandom", {{DataType::String}, jsonField::Required}},
            {"withdrawalsRoot", {{DataType::String}, jsonField::Required}},
            {"extraData", {{DataType::String}, jsonField::Required}},
            {"timestamp", {{DataType::String}, jsonField::Required}},
            {"nonce", {{DataType::String}, jsonField::Required}},
            {"mixHash", {{DataType::String}, jsonField::Required}}
        });
}

SetChainParamsArgs::SetChainParamsArgs(spDataObject& _data)
{
    requireSetChainParamsScheme(_data);

    m_params = MOVE(_data, "params").getPointer();
    m_preState = spState(new State(MOVE(_data, "accounts")));
    m_sealEngine = sealEngineFromStr(_data->atKey("sealEngine").asString());

    spSetChainParamsArgsGenesis genesis = readGenesis(_data->atKey("genesis"));
    m_genesis = genesis->constructBlockHeader();
}

spDataObject SetChainParamsArgs::asDataObject() const
{
    spDataObject out;
    (*out)["params"].copyFrom(m_params);
    (*out).atKeyPointer("accounts") = m_preState->asDataObject();
    (*out)["sealEngine"] = sealEngineToStr(m_sealEngine);
    (*out)["genesis"]["author"] = m_genesis->author().asString();
    (*out)["genesis"]["difficulty"] = m_genesis->difficulty().asString();
    (*out)["genesis"]["gasLimit"] = m_genesis->gasLimit().asString();

    if (isBlockExportBasefee(m_genesis))
    {
        BlockHeader1559 const& newbl = BlockHeader1559::castFrom(m_genesis);
        (*out)["genesis"]["baseFeePerGas"] = newbl.baseFee().asString();
        if (isBlockExportWithdrawals(m_genesis))
        {
            BlockHeaderShanghai const& newbl = BlockHeaderShanghai::castFrom(m_genesis);
            (*out)["genesis"]["withdrawalsRoot"] = newbl.withdrawalsRoot().asString();
        }
    }

    (*out)["genesis"]["extraData"] = m_genesis->extraData().asString();
    (*out)["genesis"]["timestamp"] = m_genesis->timestamp().asString();
    (*out)["genesis"]["nonce"] = m_genesis->nonce().asString();
    (*out)["genesis"]["mixHash"] = m_genesis->mixHash().asString();
    return out;
}

spDataObject SetChainParamsArgsGenesis::buildCommonBlockHeader() const
{
    spDataObject _fullBlockHeader;
    DataObject& fullBlockHeader = _fullBlockHeader.getContent();
    fullBlockHeader["author"] = m_dataRef.atKey("author").asString();
    fullBlockHeader["gasLimit"] = m_dataRef.atKey("gasLimit").asString();
    fullBlockHeader["extraData"] = m_dataRef.atKey("extraData").asString();
    fullBlockHeader["timestamp"] = m_dataRef.atKey("timestamp").asString();
    fullBlockHeader["nonce"] = m_dataRef.atKey("nonce").asString();
    fullBlockHeader["mixHash"] = m_dataRef.atKey("mixHash").asString();

    // Fields that are ommited in RPC setChainParams, use default fields of empty block
    fullBlockHeader["bloom"] = FH256::zero().asString();
    fullBlockHeader["gasUsed"] = "0x00";
    fullBlockHeader["number"] = "0x00";
    fullBlockHeader["receiptTrie"] = C_WITHDRAWALS_EMPTY_ROOT;
    fullBlockHeader["stateRoot"] = FH32::zero().asString();
    fullBlockHeader["transactionsTrie"] = C_WITHDRAWALS_EMPTY_ROOT;
    fullBlockHeader["uncleHash"] = C_EMPTY_LIST_HASH;
    fullBlockHeader["parentHash"] = FH32::zero().asString();
    return _fullBlockHeader;
}

spBlockHeader SetChainParamsArgsGenesis::constructBlockHeader() const
{
    spDataObject header = buildCommonBlockHeader();
    (*header)["difficulty"] = m_dataRef.atKey("difficulty").asString();
    return readBlockHeader(header);
}

spBlockHeader SetChainParamsArgsGenesis1559::constructBlockHeader() const
{
    spDataObject header = buildCommonBlockHeader();
    (*header)["difficulty"] = m_dataRef.atKey("difficulty").asString();
    (*header)["baseFeePerGas"] = m_dataRef.atKey("baseFeePerGas").asString();
    return readBlockHeader(header);
}

spBlockHeader SetChainParamsArgsGenesisMerge::constructBlockHeader() const
{
    spDataObject header = buildCommonBlockHeader();
    (*header)["baseFeePerGas"] = m_dataRef.atKey("baseFeePerGas").asString();
    (*header)["difficulty"] = "0x00";
    auto const curRandomU256 = dev::u256(m_dataRef.atKey("currentRandom").asString());
    (*header)["mixHash"] = dev::toCompactHexPrefixed(curRandomU256, 32);
    return readBlockHeader(header);
}

spBlockHeader SetChainParamsArgsGenesisShanghai::constructBlockHeader() const
{
    spDataObject header = buildCommonBlockHeader();
    (*header)["baseFeePerGas"] = m_dataRef.atKey("baseFeePerGas").asString();
    (*header)["difficulty"] = "0x00";
    auto const curRandomU256 = dev::u256(m_dataRef.atKey("currentRandom").asString());
    (*header)["mixHash"] = dev::toCompactHexPrefixed(curRandomU256, 32);
    (*header)["withdrawalsRoot"] = m_dataRef.atKey("withdrawalsRoot").asString();
    return readBlockHeader(header);
}

}  // namespace teststruct
}  // namespace test
