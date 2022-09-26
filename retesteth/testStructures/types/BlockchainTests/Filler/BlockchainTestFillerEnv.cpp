#include "BlockchainTestFillerEnv.h"
#include <retesteth/EthChecks.h>
#include <retesteth/testStructures/Common.h>
using namespace std;
using namespace dataobject;
using namespace test::teststruct;

namespace  {
void requireLegacyBlockchainHeader(spDataObject const& _data)
{
    REQUIRE_JSONFIELDS(_data, "GenesisBlockHeader(BlockchainTestFillerEnvLegacy) " + _data->getKey(),
        {{"bloom", {{DataType::String}, jsonField::Optional}},
            {"logsBloom", {{DataType::String}, jsonField::Optional}},
            {"coinbase", {{DataType::String}, jsonField::Optional}},
            {"author", {{DataType::String}, jsonField::Optional}},
            {"miner", {{DataType::String}, jsonField::Optional}},
            {"difficulty", {{DataType::String}, jsonField::Required}},
            {"extraData", {{DataType::String}, jsonField::Required}},
            {"gasLimit", {{DataType::String}, jsonField::Required}},
            {"gasUsed", {{DataType::String}, jsonField::Required}},
            {"hash", {{DataType::String}, jsonField::Optional}},
            {"mixHash", {{DataType::String}, jsonField::Optional}},
            {"nonce", {{DataType::String}, jsonField::Optional}},
            {"number", {{DataType::String}, jsonField::Required}},
            {"parentHash", {{DataType::String}, jsonField::Required}},
            {"receiptTrie", {{DataType::String}, jsonField::Optional}},
            {"receiptsRoot", {{DataType::String}, jsonField::Optional}},
            {"stateRoot", {{DataType::String}, jsonField::Required}},
            {"timestamp", {{DataType::String}, jsonField::Required}},
            {"transactionsTrie", {{DataType::String}, jsonField::Optional}},
            {"transactionsRoot", {{DataType::String}, jsonField::Optional}},
            {"sha3Uncles", {{DataType::String}, jsonField::Optional}},
            {"uncleHash", {{DataType::String}, jsonField::Optional}}});
}

void require1559BlockchainHeader(spDataObject const& _data)
{
    REQUIRE_JSONFIELDS(_data, "GenesisBlockHeader(BlockchainTestFillerEnv1559) " + _data->getKey(),
        {{"bloom", {{DataType::String}, jsonField::Optional}},
            {"logsBloom", {{DataType::String}, jsonField::Optional}},
            {"coinbase", {{DataType::String}, jsonField::Optional}},
            {"author", {{DataType::String}, jsonField::Optional}},
            {"miner", {{DataType::String}, jsonField::Optional}},
            {"difficulty", {{DataType::String}, jsonField::Required}},
            {"extraData", {{DataType::String}, jsonField::Required}},
            {"gasLimit", {{DataType::String}, jsonField::Required}},
            {"baseFeePerGas", {{DataType::String}, jsonField::Required}},
            {"gasUsed", {{DataType::String}, jsonField::Required}},
            {"hash", {{DataType::String}, jsonField::Optional}},
            {"mixHash", {{DataType::String}, jsonField::Optional}},
            {"nonce", {{DataType::String}, jsonField::Optional}},
            {"number", {{DataType::String}, jsonField::Required}},
            {"parentHash", {{DataType::String}, jsonField::Required}},
            {"receiptTrie", {{DataType::String}, jsonField::Optional}},
            {"receiptsRoot", {{DataType::String}, jsonField::Optional}},
            {"stateRoot", {{DataType::String}, jsonField::Required}},
            {"timestamp", {{DataType::String}, jsonField::Required}},
            {"transactionsTrie", {{DataType::String}, jsonField::Optional}},
            {"transactionsRoot", {{DataType::String}, jsonField::Optional}},
            {"sha3Uncles", {{DataType::String}, jsonField::Optional}},
            {"uncleHash", {{DataType::String}, jsonField::Optional}}});
}

void requireMergeBlockchainHeader(spDataObject const& _data)
{
    REQUIRE_JSONFIELDS(_data, "GenesisBlockHeader(BlockchainTestFillerEnvMerge) " + _data->getKey(),
        {{"bloom", {{DataType::String}, jsonField::Optional}},
            {"logsBloom", {{DataType::String}, jsonField::Optional}},
            {"coinbase", {{DataType::String}, jsonField::Optional}},
            {"author", {{DataType::String}, jsonField::Optional}},
            {"miner", {{DataType::String}, jsonField::Optional}},
            {"difficulty", {{DataType::String}, jsonField::Required}},
            {"extraData", {{DataType::String}, jsonField::Required}},
            {"gasLimit", {{DataType::String}, jsonField::Required}},
            {"baseFeePerGas", {{DataType::String}, jsonField::Required}},
            {"gasUsed", {{DataType::String}, jsonField::Required}},
            {"hash", {{DataType::String}, jsonField::Optional}},
            {"mixHash", {{DataType::String}, jsonField::Optional}},
            {"nonce", {{DataType::String}, jsonField::Optional}},
            {"number", {{DataType::String}, jsonField::Required}},
            {"parentHash", {{DataType::String}, jsonField::Required}},
            {"receiptTrie", {{DataType::String}, jsonField::Optional}},
            {"receiptsRoot", {{DataType::String}, jsonField::Optional}},
            {"stateRoot", {{DataType::String}, jsonField::Required}},
            {"timestamp", {{DataType::String}, jsonField::Required}},
            {"transactionsTrie", {{DataType::String}, jsonField::Optional}},
            {"transactionsRoot", {{DataType::String}, jsonField::Optional}},
            {"sha3Uncles", {{DataType::String}, jsonField::Optional}},
            {"uncleHash", {{DataType::String}, jsonField::Optional}}});
}

void convertDecFieldsToHex(spDataObject& _data)
{
    (*_data).atKeyUnsafe("coinbase").performModifier(mod_valueInsertZeroXPrefix);
    (*_data).atKeyUnsafe("difficulty").performModifier(mod_valueToCompactEvenHexPrefixed);
    (*_data).atKeyUnsafe("gasLimit").performModifier(mod_valueToCompactEvenHexPrefixed);
    (*_data).atKeyUnsafe("number").performModifier(mod_valueToCompactEvenHexPrefixed);
    (*_data).atKeyUnsafe("timestamp").performModifier(mod_valueToCompactEvenHexPrefixed);
    (*_data).atKeyUnsafe("parentHash").performModifier(mod_valueInsertZeroXPrefix);
    if (_data->count("baseFeePerGas"))
        (*_data).atKeyUnsafe("baseFeePerGas").performModifier(mod_valueToCompactEvenHexPrefixed);
    (*_data).performModifier(mod_valueToLowerCase);
}

spDataObject formatRawDataToRPCformat(spDataObject& _data)
{
    // Format the env to RPC format
    spDataObject out;
    (*out).atKeyPointer("currentCoinbase") = (*_data).atKeyPointerUnsafe("coinbase");
    (*out).atKeyPointer("currentDifficulty") = (*_data).atKeyPointerUnsafe("difficulty");
    (*out).atKeyPointer("currentNumber") = (*_data).atKeyPointerUnsafe("number");
    (*out).atKeyPointer("currentTimestamp") = (*_data).atKeyPointerUnsafe("timestamp");
    (*out).atKeyPointer("currentGasLimit") = (*_data).atKeyPointerUnsafe("gasLimit");
    (*out).atKeyPointer("previousHash") = (*_data).atKeyPointerUnsafe("parentHash");
    if (_data->count("baseFeePerGas"))
        (*out).atKeyPointer("currentBaseFee") = (*_data).atKeyPointerUnsafe("baseFeePerGas");
    return out;
}

}

namespace test
{
namespace teststruct
{

void BlockchainTestFillerEnv::initializeCommonFields(spDataObject const& _data, SealEngine _sEngine)
{
    m_currentNumber = spVALUE(new VALUE(_data->atKey("number")));
    m_currentTimestamp = spVALUE(new VALUE(_data->atKey("timestamp")));
    m_previousHash = spFH32(new FH32(_data->atKey("parentHash")));
    m_currentExtraData = spBYTES(new BYTES(_data->atKey("extraData")));
    m_currentCoinbase = spFH20(new FH20(_data->atKey("coinbase")));
    m_currentGasLimit = spVALUE(new VALUE(_data->atKey("gasLimit")));
    if (m_currentGasLimit.getCContent() > dev::bigint("0x7fffffffffffffff"))
        throw test::UpwardsException("currentGasLimit must be < 0x7fffffffffffffff");
    if (_sEngine == SealEngine::NoProof)
    {
        m_currentNonce = spFH8(new FH8(FH8::zero()));
        m_currentMixHash = spFH32(new FH32(FH32::zero()));
    }
    else
    {
        m_currentNonce = spFH8(new FH8(_data->atKey("nonce")));
        m_currentMixHash = spFH32(new FH32(_data->atKey("mixHash")));
    }

    m_currentDifficulty = spVALUE(new VALUE(DataObject("0x020000")));
    m_currentBaseFee = spVALUE(new VALUE(DataObject("0x10")));
    auto const& difficulty = m_currentDifficulty->asString();
    m_currentRandom = spFH32(new FH32(dev::toCompactHexPrefixed(dev::u256(difficulty), 32)));
}

void BlockchainTestFillerEnvMerge::initializeMergeFields(DataObject const& _data)
{
    m_currentBaseFee = spVALUE(new VALUE(_data.atKey("baseFeePerGas")));
    m_currentRandom = spFH32(new FH32(_data.atKey("mixHash")));
}

void BlockchainTestFillerEnv1559::initialize1559Fields(DataObject const& _data)
{
    m_currentDifficulty = spVALUE(new VALUE(_data.atKey("difficulty")));
    m_currentBaseFee = spVALUE(new VALUE(_data.atKey("baseFeePerGas")));
}

void BlockchainTestFillerEnvLegacy::initializeLegacyFields(DataObject const& _data)
{
    m_currentDifficulty = spVALUE(new VALUE(_data.atKey("difficulty")));
}

BlockchainTestFillerEnvMerge::BlockchainTestFillerEnvMerge(spDataObjectMove _data, SealEngine _sEngine)
{
    try {

        m_raw = _data.getPointer();
        requireMergeBlockchainHeader(m_raw);
        convertDecFieldsToHex(m_raw);
        initializeCommonFields(m_raw, _sEngine);
        initializeMergeFields(m_raw);
        m_raw = formatRawDataToRPCformat(m_raw);
    }
    catch (std::exception const& _ex)
    {
        throw UpwardsException(string("BlockchainTestFillerEnv(Merge) convertion error: ") + _ex.what());
    }
}

BlockchainTestFillerEnv1559::BlockchainTestFillerEnv1559(spDataObjectMove _data, SealEngine _sEngine)
{
    try {
        m_raw = _data.getPointer();
        require1559BlockchainHeader(m_raw);
        convertDecFieldsToHex(m_raw);
        initializeCommonFields(m_raw, _sEngine);
        initialize1559Fields(m_raw);
        m_raw = formatRawDataToRPCformat(m_raw);
    }
    catch (std::exception const& _ex)
    {
        throw UpwardsException(string("BlockchainTestFillerEnv(1559) convertion error: ") + _ex.what());
    }
}

BlockchainTestFillerEnvLegacy::BlockchainTestFillerEnvLegacy(spDataObjectMove _data, SealEngine _sEngine)
{
    try {
        m_raw = _data.getPointer();
        requireLegacyBlockchainHeader(m_raw);
        convertDecFieldsToHex(m_raw);
        initializeCommonFields(m_raw, _sEngine);
        initializeLegacyFields(m_raw);
        m_raw = formatRawDataToRPCformat(m_raw);
    }
    catch (std::exception const& _ex)
    {
        throw UpwardsException(string("BlockchainTestFillerEnv(legacy) convertion error: ") + _ex.what());
    }
}

}  // namespace teststruct
}  // namespace test
