#include "SetChainParamsArgs.h"
#include <retesteth/testStructures/Common.h>
#include <retesteth/Constants.h>

using namespace dataobject;
using namespace test::teststruct;
using namespace test::teststruct::constnames;

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
    if (_data.count(c_baseFeePerGas))
    {
        if (_data.count("currentRandom"))
        {
            if (_data.count(c_withdrawalsRoot))
            {
                if (_data.count(c_currentExcessBlobGas))
                    return spSetChainParamsArgsGenesis(new SetChainParamsArgsGenesis4844(_data));
                else
                    return spSetChainParamsArgsGenesis(new SetChainParamsArgsGenesisShanghai(_data));
            }
            else
                return spSetChainParamsArgsGenesis(new SetChainParamsArgsGenesisMerge(_data));
        }
        else
            return spSetChainParamsArgsGenesis(new SetChainParamsArgsGenesis1559(_data));
    }
    return spSetChainParamsArgsGenesis(new SetChainParamsArgsGenesis(_data));
}

}

namespace test::teststruct
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

SetChainParamsArgsGenesis4844::SetChainParamsArgsGenesis4844(DataObject const& _data)
  : SetChainParamsArgsGenesis(_data, false)
{
    REQUIRE_JSONFIELDS(_data, "SetChainParamsArgs::genesis(4844 Cancun) ",
        {
            {"author", {{DataType::String}, jsonField::Required}},
            {"gasLimit", {{DataType::String}, jsonField::Required}},
            {"baseFeePerGas", {{DataType::String}, jsonField::Required}},
            {"currentRandom", {{DataType::String}, jsonField::Required}},
            {"withdrawalsRoot", {{DataType::String}, jsonField::Required}},
            {"extraData", {{DataType::String}, jsonField::Required}},
            {"timestamp", {{DataType::String}, jsonField::Required}},
            {"nonce", {{DataType::String}, jsonField::Required}},
            {"mixHash", {{DataType::String}, jsonField::Required}},
            {c_currentBlobGasUsed, {{DataType::String}, jsonField::Required}},
            {c_currentExcessBlobGas, {{DataType::String}, jsonField::Required}},
            {c_currentBeaconRoot, {{DataType::String}, jsonField::Required}}
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
    (*out)["genesis"][c_author] = m_genesis->author().asString();
    (*out)["genesis"][c_difficulty] = m_genesis->difficulty().asString();
    (*out)["genesis"][c_gasLimit] = m_genesis->gasLimit().asString();

    if (isBlockExportBasefee(m_genesis))
    {
        BlockHeader1559 const& newbl = BlockHeader1559::castFrom(m_genesis);
        (*out)["genesis"][c_baseFeePerGas] = newbl.baseFee().asString();
        if (isBlockExportWithdrawals(m_genesis))
        {
            BlockHeaderShanghai const& newbl = BlockHeaderShanghai::castFrom(m_genesis);
            (*out)["genesis"][c_withdrawalsRoot] = newbl.withdrawalsRoot().asString();
        }
        if (isBlockExportExcessBlobGas(m_genesis))
        {
            BlockHeader4844 const& newbl = BlockHeader4844::castFrom(m_genesis);
            (*out)["genesis"][c_currentExcessBlobGas] = newbl.excessBlobGas().asString();
            (*out)["genesis"][c_currentBlobGasUsed] = newbl.blobGasUsed().asString();
            (*out)["genesis"][c_currentBeaconRoot] = newbl.parentBeaconBlockRoot().asString();
        }
    }

    (*out)["genesis"][c_extraData] = m_genesis->extraData().asString();
    (*out)["genesis"][c_timestamp] = m_genesis->timestamp().asString();
    (*out)["genesis"][c_nonce] = m_genesis->nonce().asString();
    (*out)["genesis"][c_mixHash] = m_genesis->mixHash().asString();
    return out;
}

spDataObject SetChainParamsArgsGenesis::buildCommonBlockHeader() const
{
    spDataObject _fullBlockHeader;
    DataObject& fullBlockHeader = _fullBlockHeader.getContent();
    fullBlockHeader[c_author] = m_dataRef.atKey(c_author).asString();
    fullBlockHeader[c_gasLimit] = m_dataRef.atKey(c_gasLimit).asString();
    fullBlockHeader[c_extraData] = m_dataRef.atKey(c_extraData).asString();
    fullBlockHeader[c_timestamp] = m_dataRef.atKey(c_timestamp).asString();
    fullBlockHeader[c_nonce] = m_dataRef.atKey(c_nonce).asString();
    fullBlockHeader[c_mixHash] = m_dataRef.atKey(c_mixHash).asString();

    // Fields that are ommited in RPC setChainParams, use default fields of empty block
    fullBlockHeader[c_bloom] = FH256::zero().asString();
    fullBlockHeader[c_gasUsed] = "0x00";
    fullBlockHeader[c_number] = "0x00";
    fullBlockHeader[c_receiptTrie] = C_WITHDRAWALS_EMPTY_ROOT;
    fullBlockHeader[c_stateRoot] = FH32::zero().asString();
    fullBlockHeader[c_transactionsTrie] = C_WITHDRAWALS_EMPTY_ROOT;
    fullBlockHeader[c_uncleHash] = C_EMPTY_LIST_HASH;
    fullBlockHeader[c_parentHash] = FH32::zero().asString();
    return _fullBlockHeader;
}

spBlockHeader SetChainParamsArgsGenesis::constructBlockHeader() const
{
    spDataObject header = _constructBlockHeader();
    return readBlockHeader(header);
}

spDataObject SetChainParamsArgsGenesis::_constructBlockHeader() const
{
    spDataObject header = buildCommonBlockHeader();
    (*header)[c_difficulty] = m_dataRef.atKey(c_difficulty).asString();
    return header;
}

spDataObject SetChainParamsArgsGenesis1559::_constructBlockHeader() const
{
    spDataObject header = buildCommonBlockHeader();
    (*header)[c_difficulty] = m_dataRef.atKey(c_difficulty).asString();
    (*header)[c_baseFeePerGas] = m_dataRef.atKey(c_baseFeePerGas).asString();
    return header;
}

spDataObject SetChainParamsArgsGenesisMerge::_constructBlockHeader() const
{
    spDataObject header = buildCommonBlockHeader();
    (*header)[c_difficulty] = "0x00";
    auto const curRandomU256 = dev::u256(m_dataRef.atKey("currentRandom").asString());
    (*header)[c_mixHash] = dev::toCompactHexPrefixed(curRandomU256, 32);
    (*header)[c_baseFeePerGas] = m_dataRef.atKey(c_baseFeePerGas).asString();
    return header;
}

spDataObject SetChainParamsArgsGenesisShanghai::_constructBlockHeader() const
{
    spDataObject header = buildCommonBlockHeader();
    (*header)[c_difficulty] = "0x00";
    auto const curRandomU256 = dev::u256(m_dataRef.atKey("currentRandom").asString());
    (*header)[c_mixHash] = dev::toCompactHexPrefixed(curRandomU256, 32);
    (*header)[c_baseFeePerGas] = m_dataRef.atKey(c_baseFeePerGas).asString();
    (*header)[c_withdrawalsRoot] = m_dataRef.atKey(c_withdrawalsRoot).asString();
    return header;
}

spDataObject SetChainParamsArgsGenesis4844::_constructBlockHeader() const
{
    spDataObject header = buildCommonBlockHeader();
    (*header)[c_difficulty] = "0x00";
    auto const curRandomU256 = dev::u256(m_dataRef.atKey("currentRandom").asString());
    (*header)[c_mixHash] = dev::toCompactHexPrefixed(curRandomU256, 32);
    (*header)[c_baseFeePerGas] = m_dataRef.atKey(c_baseFeePerGas).asString();
    (*header)[c_withdrawalsRoot] = m_dataRef.atKey(c_withdrawalsRoot).asString();
    (*header)[c_blobGasUsed] =  m_dataRef.atKey(c_currentBlobGasUsed).asString();
    (*header)[c_excessBlobGas] = m_dataRef.atKey(c_currentExcessBlobGas).asString();
    (*header)[c_parentBeaconBlockRoot] = m_dataRef.atKey(c_currentBeaconRoot).asString();
    return header;
}

}  // namespace teststruct
