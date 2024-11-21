#include "BlockchainTestEnv.h"
#include <retesteth/EthChecks.h>
#include <retesteth/testStructures/Common.h>
#include <retesteth/Constants.h>
using namespace std;
using namespace dataobject;
using namespace test::teststruct;
using namespace test::teststruct::constnames;

namespace {
void requireLegacyBlockchainHeader(DataObject const& _data)
{
    REQUIRE_JSONFIELDS(_data, "GenesisBlockHeader(BlockchainTestEnvLegacy) " + _data.getKey(),
        {{c_bloom, {{DataType::String}, jsonField::Optional}},
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

void require1559BlockchainHeader(DataObject const& _data)
{
    REQUIRE_JSONFIELDS(_data, "GenesisBlockHeader(BlockchainTestEnv1559) " + _data.getKey(),
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

void requireParisBlockchainHeader(DataObject const& _data)
{
    REQUIRE_JSONFIELDS(_data, "GenesisBlockHeader(BlockchainTestEnvParis) " + _data.getKey(),
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

void requireShanghaiBlockchainHeader(DataObject const& _data)
{
    REQUIRE_JSONFIELDS(_data, "GenesisBlockHeader(BlockchainTestEnvShanghai) " + _data.getKey(),
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

void require4844BlockchainHeader(DataObject const& _data)
{
    REQUIRE_JSONFIELDS(_data, "GenesisBlockHeader(BlockchainTestEnv4844) " + _data.getKey(),
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

void requirePragueBlockchainHeader(DataObject const& _data)
{
    REQUIRE_JSONFIELDS(_data, "GenesisBlockHeader(BlockchainTestEnvPrague) " + _data.getKey(),
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
            {c_requestsHash, {{DataType::String}, jsonField::Required}},
            {c_sha3Uncles, {{DataType::String}, jsonField::Optional}},
            {c_uncleHash, {{DataType::String}, jsonField::Optional}}});
}

}

namespace test::teststruct
{

void BlockchainTestEnvPrague::initializePragueFields(DataObject const& _data)
{
    m_currentRequestsHash = sFH32(_data.atKey(c_requestsHash));
}

void BlockchainTestEnv4844::initialize4844Fields(DataObject const& _data)
{
    m_currentExcessBlobGas = sVALUE(_data.atKey(c_excessBlobGas));
    m_currentBlobGasUsed = sVALUE(_data.atKey(c_blobGasUsed));
    m_currentBeaconRoot = sFH32(_data.atKey(c_parentBeaconBlockRoot));
}

void BlockchainTestEnvShanghai::initializeShanghaiFields(DataObject const& _data)
{
    m_currentWithdrawalsRoot = sFH32(_data.atKey(c_withdrawalsRoot));
}

void BlockchainTestEnvParis::initializeParisFields(DataObject const& _data)
{
    m_currentBaseFee = sVALUE(_data.atKey(c_baseFeePerGas));
    m_currentRandom = sFH32(_data.atKey(c_mixHash));
    m_currentDifficulty = sVALUE(DataObject("0x020000"));
}

void BlockchainTestEnv1559::initialize1559Fields(DataObject const& _data)
{
    m_currentDifficulty = sVALUE(_data.atKey(c_difficulty));
    m_currentBaseFee = sVALUE(_data.atKey(c_baseFeePerGas));
}

void BlockchainTestEnvLegacy::initializeLegacyFields(DataObject const& _data)
{
    m_currentDifficulty = sVALUE(_data.atKey(c_difficulty));
}

BlockchainTestEnvLegacy::BlockchainTestEnvLegacy(DataObject const& _data)
{
    try {
        requireLegacyBlockchainHeader(_data);
        initializeCommonFields(_data);
        initializeLegacyFields(_data);
    }
    catch (std::exception const& _ex)
    {
        throw UpwardsException(string("BlockchainTestEnv(legacy) convertion error: ") + _ex.what() + _data.asJson());
    }
}

BlockchainTestEnv1559::BlockchainTestEnv1559(DataObject const& _data)
{
    try {
        require1559BlockchainHeader(_data);
        initializeCommonFields(_data);
        initialize1559Fields(_data);
    }
    catch (std::exception const& _ex)
    {
        throw UpwardsException(string("BlockchainTestEnv(1559) convertion error: ") + _ex.what() + _data.asJson());
    }
}

BlockchainTestEnvParis::BlockchainTestEnvParis(DataObject const& _data)
{
    try {
        requireParisBlockchainHeader(_data);
        initializeCommonFields(_data);
        initializeParisFields(_data);
    }
    catch (std::exception const& _ex)
    {
        throw UpwardsException(string("BlockchainTestEnv(Paris) convertion error: ") + _ex.what() + _data.asJson());
    }
}

BlockchainTestEnvShanghai::BlockchainTestEnvShanghai(DataObject const& _data)
  : BlockchainTestEnvParis()
{
    try {
        requireShanghaiBlockchainHeader(_data);
        initializeCommonFields(_data);
        initializeParisFields(_data);
        initializeShanghaiFields(_data);
    }
    catch (std::exception const& _ex)
    {
        throw UpwardsException(string("BlockchainTestEnv(Shanghai) convertion error: ") + _ex.what() + _data.asJson());
    }
}

BlockchainTestEnv4844::BlockchainTestEnv4844(DataObject const& _data)
{
    try {
        require4844BlockchainHeader(_data);
        initializeCommonFields(_data);
        initializeParisFields(_data);
        initializeShanghaiFields(_data);
        initialize4844Fields(_data);
    }
    catch (std::exception const& _ex)
    {
        throw UpwardsException(string("BlockchainTestEnv(4844) convertion error: ") + _ex.what() + _data.asJson());
    }
}

BlockchainTestEnvPrague::BlockchainTestEnvPrague(DataObject const& _data) :
    BlockchainTestEnv4844()
{
    try {
        requirePragueBlockchainHeader(_data);
        initializeCommonFields(_data);
        initializeParisFields(_data);
        initializeShanghaiFields(_data);
        initialize4844Fields(_data);
        initializePragueFields(_data);
    }
    catch (std::exception const& _ex)
    {
        throw UpwardsException(string("BlockchainTestEnv(Prague) convertion error: ") + _ex.what() + _data.asJson());
    }
}


void BlockchainTestEnv::initializeCommonFields(DataObject const& _data)
{
    m_currentNumber = sVALUE(_data.atKey(c_number));
    m_currentTimestamp = sVALUE(_data.atKey(c_timestamp));
    m_currentExtraData = sBYTES(_data.atKey(c_extraData));
    m_currentNonce = sFH8(_data.atKey(c_nonce));
    m_currentMixHash = sFH32(_data.atKey(c_mixHash));
    m_currentGasLimit = sVALUE(_data.atKey(c_gasLimit));
    m_currentCoinbase = sFH20(_data.atKey(c_coinbase));
}

}  // namespace teststruct
