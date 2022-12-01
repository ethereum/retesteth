#include "BlockchainTestEnv.h"
#include <retesteth/EthChecks.h>
#include <retesteth/testStructures/Common.h>
using namespace std;
using namespace dataobject;
using namespace test::teststruct;

namespace {
void requireLegacyBlockchainHeader(DataObject const& _data)
{
    REQUIRE_JSONFIELDS(_data, "GenesisBlockHeader(BlockchainTestEnvLegacy) " + _data.getKey(),
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

void require1559BlockchainHeader(DataObject const& _data)
{
    REQUIRE_JSONFIELDS(_data, "GenesisBlockHeader(BlockchainTestEnv1559) " + _data.getKey(),
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

void requireMergeBlockchainHeader(DataObject const& _data)
{
    REQUIRE_JSONFIELDS(_data, "GenesisBlockHeader(BlockchainTestEnvMerge) " + _data.getKey(),
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
}

namespace test
{
namespace teststruct
{

void BlockchainTestEnvMerge::initializeMergeFields(DataObject const& _data)
{
    m_currentBaseFee = spVALUE(new VALUE(_data.atKey("baseFeePerGas")));
    m_currentRandom = spFH32(new FH32(_data.atKey("mixHash")));
    m_currentDifficulty = spVALUE(new VALUE(DataObject("0x020000")));
}

void BlockchainTestEnv1559::initialize1559Fields(DataObject const& _data)
{
    m_currentDifficulty = spVALUE(new VALUE(_data.atKey("difficulty")));
    m_currentBaseFee = spVALUE(new VALUE(_data.atKey("baseFeePerGas")));
}

void BlockchainTestEnvLegacy::initializeLegacyFields(DataObject const& _data)
{
    m_currentDifficulty = spVALUE(new VALUE(_data.atKey("difficulty")));
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

BlockchainTestEnvMerge::BlockchainTestEnvMerge(DataObject const& _data)
{
    try {
        requireMergeBlockchainHeader(_data);
        initializeCommonFields(_data);
        initializeMergeFields(_data);
    }
    catch (std::exception const& _ex)
    {
        throw UpwardsException(string("BlockchainTestEnv(Merge) convertion error: ") + _ex.what() + _data.asJson());
    }
}

void BlockchainTestEnv::initializeCommonFields(DataObject const& _data)
{
    m_currentNumber = spVALUE(new VALUE(_data.atKey("number")));
    m_currentTimestamp = spVALUE(new VALUE(_data.atKey("timestamp")));
    m_previousHash = spFH32(new FH32(_data.atKey("parentHash")));
    m_currentExtraData = spBYTES(new BYTES(_data.atKey("extraData")));
    m_currentNonce = spFH8(new FH8(_data.atKey("nonce")));
    m_currentMixHash = spFH32(new FH32(_data.atKey("mixHash")));
    m_currentGasLimit = spVALUE(new VALUE(_data.atKey("gasLimit")));
    m_currentCoinbase = spFH20(new FH20(_data.atKey("coinbase")));
}

}  // namespace teststruct
}  // namespace test
