#include "BlockchainTestFillerEnv.h"
#include <retesteth/EthChecks.h>
#include <retesteth/testStructures/Common.h>
#include <retesteth/Constants.h>
using namespace std;
using namespace dataobject;
using namespace test::teststruct;
using namespace test::teststruct::constnames;

namespace  {
void requireLegacyBlockchainHeader(spDataObject const& _data)
{
    REQUIRE_JSONFIELDS(_data, "GenesisBlockHeader(BlockchainTestFillerEnvLegacy) " + _data->getKey(),
        {{"bloom", {{DataType::String}, jsonField::Optional}},
            {c_logsBloom, {{DataType::String}, jsonField::Optional}},
            {c_coinbase, {{DataType::String}, jsonField::Optional}},
            {c_author, {{DataType::String}, jsonField::Optional}},
            {c_miner, {{DataType::String}, jsonField::Optional}},
            {c_difficulty, {{DataType::String}, jsonField::Required}},
            {c_extraData, {{DataType::String}, jsonField::Required}},
            {c_gasLimit, {{DataType::String}, jsonField::Required}},
            {c_gasUsed, {{DataType::String}, jsonField::Required}},
            {c_hash, {{DataType::String}, jsonField::Optional}},
            {c_mixHash, {{DataType::String}, jsonField::Optional}},
            {c_nonce, {{DataType::String}, jsonField::Optional}},
            {c_number, {{DataType::String}, jsonField::Required}},
            {c_parentHash, {{DataType::String}, jsonField::Required}},
            {c_receiptTrie, {{DataType::String}, jsonField::Optional}},
            {c_receiptsRoot, {{DataType::String}, jsonField::Optional}},
            {c_stateRoot, {{DataType::String}, jsonField::Required}},
            {c_timestamp, {{DataType::String}, jsonField::Required}},
            {c_transactionsTrie, {{DataType::String}, jsonField::Optional}},
            {c_transactionsRoot, {{DataType::String}, jsonField::Optional}},
            {c_sha3Uncles, {{DataType::String}, jsonField::Optional}},
            {c_uncleHash, {{DataType::String}, jsonField::Optional}}});
}

void require1559BlockchainHeader(spDataObject const& _data)
{
    REQUIRE_JSONFIELDS(_data, "GenesisBlockHeader(BlockchainTestFillerEnv1559) " + _data->getKey(),
        {{c_bloom, {{DataType::String}, jsonField::Optional}},
            {c_logsBloom, {{DataType::String}, jsonField::Optional}},
            {c_coinbase, {{DataType::String}, jsonField::Optional}},
            {c_author, {{DataType::String}, jsonField::Optional}},
            {c_miner, {{DataType::String}, jsonField::Optional}},
            {c_difficulty, {{DataType::String}, jsonField::Required}},
            {c_extraData, {{DataType::String}, jsonField::Required}},
            {c_gasLimit, {{DataType::String}, jsonField::Required}},
            {c_baseFeePerGas, {{DataType::String}, jsonField::Required}},
            {c_gasUsed, {{DataType::String}, jsonField::Required}},
            {c_hash, {{DataType::String}, jsonField::Optional}},
            {c_mixHash, {{DataType::String}, jsonField::Optional}},
            {c_nonce, {{DataType::String}, jsonField::Optional}},
            {c_number, {{DataType::String}, jsonField::Required}},
            {c_parentHash, {{DataType::String}, jsonField::Required}},
            {c_receiptTrie, {{DataType::String}, jsonField::Optional}},
            {c_receiptsRoot, {{DataType::String}, jsonField::Optional}},
            {c_stateRoot, {{DataType::String}, jsonField::Required}},
            {c_timestamp, {{DataType::String}, jsonField::Required}},
            {c_transactionsTrie, {{DataType::String}, jsonField::Optional}},
            {c_transactionsRoot, {{DataType::String}, jsonField::Optional}},
            {c_sha3Uncles, {{DataType::String}, jsonField::Optional}},
            {c_uncleHash, {{DataType::String}, jsonField::Optional}}});
}

void requireMergeBlockchainHeader(spDataObject const& _data)
{
    REQUIRE_JSONFIELDS(_data, "GenesisBlockHeader(BlockchainTestFillerEnvMerge) " + _data->getKey(),
        {{c_bloom, {{DataType::String}, jsonField::Optional}},
            {c_logsBloom, {{DataType::String}, jsonField::Optional}},
            {c_coinbase, {{DataType::String}, jsonField::Optional}},
            {c_author, {{DataType::String}, jsonField::Optional}},
            {c_miner, {{DataType::String}, jsonField::Optional}},
            {c_difficulty, {{DataType::String}, jsonField::Required}},
            {c_extraData, {{DataType::String}, jsonField::Required}},
            {c_gasLimit, {{DataType::String}, jsonField::Required}},
            {c_baseFeePerGas, {{DataType::String}, jsonField::Required}},
            {c_gasUsed, {{DataType::String}, jsonField::Required}},
            {c_hash, {{DataType::String}, jsonField::Optional}},
            {c_mixHash, {{DataType::String}, jsonField::Optional}},
            {c_nonce, {{DataType::String}, jsonField::Optional}},
            {c_number, {{DataType::String}, jsonField::Required}},
            {c_parentHash, {{DataType::String}, jsonField::Required}},
            {c_receiptTrie, {{DataType::String}, jsonField::Optional}},
            {c_receiptsRoot, {{DataType::String}, jsonField::Optional}},
            {c_stateRoot, {{DataType::String}, jsonField::Required}},
            {c_timestamp, {{DataType::String}, jsonField::Required}},
            {c_transactionsTrie, {{DataType::String}, jsonField::Optional}},
            {c_transactionsRoot, {{DataType::String}, jsonField::Optional}},
            {c_sha3Uncles, {{DataType::String}, jsonField::Optional}},
            {c_uncleHash, {{DataType::String}, jsonField::Optional}}});
}

void requireShanghaiBlockchainHeader(spDataObject const& _data)
{
    REQUIRE_JSONFIELDS(_data, "GenesisBlockHeader(BlockchainTestFillerEnvShanghai) " + _data->getKey(),
        {{c_bloom, {{DataType::String}, jsonField::Optional}},
            {c_logsBloom, {{DataType::String}, jsonField::Optional}},
            {c_coinbase, {{DataType::String}, jsonField::Optional}},
            {c_author, {{DataType::String}, jsonField::Optional}},
            {c_miner, {{DataType::String}, jsonField::Optional}},
            {c_difficulty, {{DataType::String}, jsonField::Required}},
            {c_extraData, {{DataType::String}, jsonField::Required}},
            {c_gasLimit, {{DataType::String}, jsonField::Required}},
            {c_baseFeePerGas, {{DataType::String}, jsonField::Required}},
            {c_gasUsed, {{DataType::String}, jsonField::Required}},
            {c_hash, {{DataType::String}, jsonField::Optional}},
            {c_mixHash, {{DataType::String}, jsonField::Optional}},
            {c_nonce, {{DataType::String}, jsonField::Optional}},
            {c_number, {{DataType::String}, jsonField::Required}},
            {c_parentHash, {{DataType::String}, jsonField::Required}},
            {c_receiptTrie, {{DataType::String}, jsonField::Optional}},
            {c_receiptsRoot, {{DataType::String}, jsonField::Optional}},
            {c_stateRoot, {{DataType::String}, jsonField::Required}},
            {c_timestamp, {{DataType::String}, jsonField::Required}},
            {c_transactionsTrie, {{DataType::String}, jsonField::Optional}},
            {c_transactionsRoot, {{DataType::String}, jsonField::Optional}},
            {c_withdrawalsRoot, {{DataType::String}, jsonField::Required}},
            {c_sha3Uncles, {{DataType::String}, jsonField::Optional}},
            {c_uncleHash, {{DataType::String}, jsonField::Optional}}});
}

void require4844BlockchainHeader(spDataObject const& _data)
{
    REQUIRE_JSONFIELDS(_data, "GenesisBlockHeader(BlockchainTestFillerEnv4844) " + _data->getKey(),
        {{c_bloom, {{DataType::String}, jsonField::Optional}},
            {c_logsBloom, {{DataType::String}, jsonField::Optional}},
            {c_coinbase, {{DataType::String}, jsonField::Optional}},
            {c_author, {{DataType::String}, jsonField::Optional}},
            {c_miner, {{DataType::String}, jsonField::Optional}},
            {c_difficulty, {{DataType::String}, jsonField::Required}},
            {c_extraData, {{DataType::String}, jsonField::Required}},
            {c_gasLimit, {{DataType::String}, jsonField::Required}},
            {c_baseFeePerGas, {{DataType::String}, jsonField::Required}},
            {c_gasUsed, {{DataType::String}, jsonField::Required}},
            {c_hash, {{DataType::String}, jsonField::Optional}},
            {c_mixHash, {{DataType::String}, jsonField::Optional}},
            {c_nonce, {{DataType::String}, jsonField::Optional}},
            {c_number, {{DataType::String}, jsonField::Required}},
            {c_parentHash, {{DataType::String}, jsonField::Required}},
            {c_receiptTrie, {{DataType::String}, jsonField::Optional}},
            {c_receiptsRoot, {{DataType::String}, jsonField::Optional}},
            {c_stateRoot, {{DataType::String}, jsonField::Required}},
            {c_timestamp, {{DataType::String}, jsonField::Required}},
            {c_transactionsTrie, {{DataType::String}, jsonField::Optional}},
            {c_transactionsRoot, {{DataType::String}, jsonField::Optional}},
            {c_withdrawalsRoot, {{DataType::String}, jsonField::Required}},
            {c_blobGasUsed, {{DataType::String}, jsonField::Required}},
            {c_excessBlobGas, {{DataType::String}, jsonField::Required}},
            {c_parentBeaconBlockRoot, {{DataType::String}, jsonField::Required}},
            {c_sha3Uncles, {{DataType::String}, jsonField::Optional}},
            {c_uncleHash, {{DataType::String}, jsonField::Optional}}});
}

void convertDecFieldsToHex(spDataObject& _data)
{
    (*_data).atKeyUnsafe(c_coinbase).performModifier(mod_valueInsertZeroXPrefix);
    (*_data).atKeyUnsafe(c_difficulty).performModifier(mod_valueToCompactEvenHexPrefixed);
    (*_data).atKeyUnsafe(c_gasLimit).performModifier(mod_valueToCompactEvenHexPrefixed);
    (*_data).atKeyUnsafe(c_number).performModifier(mod_valueToCompactEvenHexPrefixed);
    (*_data).atKeyUnsafe(c_timestamp).performModifier(mod_valueToCompactEvenHexPrefixed);
    (*_data).atKeyUnsafe(c_parentHash).performModifier(mod_valueInsertZeroXPrefix);
    if (_data->count(c_baseFeePerGas))
        (*_data).atKeyUnsafe(c_baseFeePerGas).performModifier(mod_valueToCompactEvenHexPrefixed);
    if (_data->count(c_excessBlobGas))
        (*_data).atKeyUnsafe(c_excessBlobGas).performModifier(mod_valueToCompactEvenHexPrefixed);
    if (_data->count(c_blobGasUsed))
        (*_data).atKeyUnsafe(c_blobGasUsed).performModifier(mod_valueToCompactEvenHexPrefixed);
    (*_data).performModifier(mod_valueToLowerCase);
}

spDataObject formatRawDataToRPCformat(spDataObject& _data)
{
    // Format the env to RPC format
    spDataObject out;
    (*out).atKeyPointer("currentCoinbase") = (*_data).atKeyPointerUnsafe(c_coinbase);
    (*out).atKeyPointer("currentDifficulty") = (*_data).atKeyPointerUnsafe(c_difficulty);
    (*out).atKeyPointer("currentNumber") = (*_data).atKeyPointerUnsafe(c_number);
    (*out).atKeyPointer("currentTimestamp") = (*_data).atKeyPointerUnsafe(c_timestamp);
    (*out).atKeyPointer("currentGasLimit") = (*_data).atKeyPointerUnsafe(c_gasLimit);
    (*out).atKeyPointer("previousHash") = (*_data).atKeyPointerUnsafe(c_parentHash);
    if (_data->count(c_baseFeePerGas))
        (*out).atKeyPointer("currentBaseFee") = (*_data).atKeyPointerUnsafe(c_baseFeePerGas);
    if (_data->count(c_excessBlobGas))
        (*out).atKeyPointer(c_currentExcessBlobGas) = (*_data).atKeyPointerUnsafe(c_excessBlobGas);
    if (_data->count(c_blobGasUsed))
        (*out).atKeyPointer(c_currentBlobGasUsed) = (*_data).atKeyPointerUnsafe(c_blobGasUsed);
    if (_data->count(c_parentBeaconBlockRoot))
        (*out).atKeyPointer(c_currentBeaconRoot) = (*_data).atKeyPointerUnsafe(c_parentBeaconBlockRoot);
    return out;
}

}

namespace test::teststruct
{

void BlockchainTestFillerEnv::initializeCommonFields(spDataObject const& _data, SealEngine _sEngine)
{
    m_currentNumber = sVALUE(_data->atKey(c_number));
    m_currentTimestamp = sVALUE(_data->atKey(c_timestamp));
    m_previousHash = sFH32(_data->atKey(c_parentHash));
    m_currentExtraData = sBYTES(_data->atKey(c_extraData));
    m_currentCoinbase = sFH20(_data->atKey(c_coinbase));
    m_currentGasLimit = sVALUE(_data->atKey(c_gasLimit));
    if (m_currentGasLimit.getCContent() > dev::bigint("0x7fffffffffffffff"))
        ETH_WARNING("currentGasLimit must be <= 0x7fffffffffffffff");
    if (_sEngine == SealEngine::NoProof)
    {
        m_currentNonce = spFH8(FH8::zero().copy());
        m_currentMixHash = spFH32(FH32::zero().copy());
    }
    else
    {
        m_currentNonce = sFH8(_data->atKey(c_nonce));
        m_currentMixHash = sFH32(_data->atKey(c_mixHash));
    }

    m_currentDifficulty = sVALUE(DataObject("0x020000"));
    m_currentBaseFee = sVALUE(DataObject("0x10"));
    auto const& difficulty = m_currentDifficulty->asString();
    m_currentRandom = sFH32(dev::toCompactHexPrefixed(dev::u256(difficulty), 32));
    m_currentWithdrawalsRoot = sFH32(DataObject(C_WITHDRAWALS_EMPTY_ROOT));
    m_currentBlobGasUsed = sVALUE(DataObject("0x00"));
    m_currentExcessBlobGas = sVALUE(DataObject("0x00"));
    m_currentBeaconRoot = spFH32(FH32::zero().copy());
}

void BlockchainTestFillerEnv4844::initialize4844Fields(DataObject const& _data)
{
    m_currentExcessBlobGas = sVALUE(_data.atKey(c_excessBlobGas));
    m_currentBlobGasUsed = sVALUE(_data.atKey(c_blobGasUsed));
    m_currentBeaconRoot = sFH32(_data.atKey(c_parentBeaconBlockRoot));
}


void BlockchainTestFillerEnvShanghai::initializeShanghaiFields(DataObject const& _data)
{
    m_currentWithdrawalsRoot = sFH32(_data.atKey(c_withdrawalsRoot));
}

void BlockchainTestFillerEnvMerge::initializeMergeFields(DataObject const& _data)
{
    m_currentBaseFee = sVALUE(_data.atKey(c_baseFeePerGas));
    m_currentRandom = sFH32(_data.atKey(c_mixHash));
}

void BlockchainTestFillerEnv1559::initialize1559Fields(DataObject const& _data)
{
    m_currentDifficulty = sVALUE(_data.atKey(c_difficulty));
    m_currentBaseFee = sVALUE(_data.atKey(c_baseFeePerGas));
}

void BlockchainTestFillerEnvLegacy::initializeLegacyFields(DataObject const& _data)
{
    m_currentDifficulty = sVALUE(_data.atKey(c_difficulty));
}

BlockchainTestFillerEnv4844::BlockchainTestFillerEnv4844(spDataObjectMove _data, SealEngine _sEngine)
{
    try {
        m_raw = _data.getPointer();
        require4844BlockchainHeader(m_raw);
        convertDecFieldsToHex(m_raw);
        initializeCommonFields(m_raw, _sEngine);
        initializeMergeFields(m_raw);
        initializeShanghaiFields(m_raw);
        initialize4844Fields(m_raw);
        m_raw = formatRawDataToRPCformat(m_raw);
    }
    catch (std::exception const& _ex)
    {
        throw UpwardsException(string("BlockchainTestFillerEnv(4844) convertion error: ") + _ex.what());
    }
}


BlockchainTestFillerEnvShanghai::BlockchainTestFillerEnvShanghai(spDataObjectMove _data, SealEngine _sEngine)
    : BlockchainTestFillerEnvMerge()
{
    try {
        m_raw = _data.getPointer();
        requireShanghaiBlockchainHeader(m_raw);
        convertDecFieldsToHex(m_raw);
        initializeCommonFields(m_raw, _sEngine);
        initializeMergeFields(m_raw);
        initializeShanghaiFields(m_raw);
        m_raw = formatRawDataToRPCformat(m_raw);
    }
    catch (std::exception const& _ex)
    {
        throw UpwardsException(string("BlockchainTestFillerEnv(Shanghai) convertion error: ") + _ex.what());
    }
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

BlockchainTestFillerEnv* readBlockchainFillerTestEnv(spDataObjectMove _data, SealEngine _sEngine)
{
    auto const& data = _data.getPointer();
    if (data->count("baseFeePerGas"))
    {
        spDataObject diff = data->atKey("difficulty").copy();
        (*diff).performModifier(mod_valueToCompactEvenHexPrefixed);
        if (VALUE(diff->asString()) != 0)
            return new BlockchainTestFillerEnv1559(_data, _sEngine);
        else
        {
            if (data->count(c_withdrawalsRoot))
            {
                if (data->count(c_excessBlobGas))
                    return new BlockchainTestFillerEnv4844(_data, _sEngine);
                else
                    return new BlockchainTestFillerEnvShanghai(_data, _sEngine);
            }
            else
                return new BlockchainTestFillerEnvMerge(_data, _sEngine);
        }
    }
    return new BlockchainTestFillerEnvLegacy(_data, _sEngine);
}
}  // namespace teststruct
