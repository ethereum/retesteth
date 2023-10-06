#include "BlockchainTestBlock.h"
#include <retesteth/EthChecks.h>
#include <retesteth/testStructures/Common.h>
using namespace std;

namespace test::teststruct
{
BlockchainTestBlock::BlockchainTestBlock(spDataObject& _data)
{
    try
    {
        REQUIRE_JSONFIELDS(_data, "BlockchainTestBlock " + _data->getKey(),
            {{"rlp", {{DataType::String}, jsonField::Required}},
                {"chainname", {{DataType::String}, jsonField::Optional}},    // User information
                {"blocknumber", {{DataType::String}, jsonField::Optional}},  // User information
                {"rlp_decoded", {{DataType::Object}, jsonField::Optional}},  // Decoding of invalid blocks
                {"transactions", {{DataType::Array}, jsonField::Optional}},
                {"withdrawals", {{DataType::Array}, jsonField::Optional}},
                {"hasBigInt", {{DataType::String}, jsonField::Optional}},
                {"transactionSequence", {{DataType::Array}, jsonField::Optional}},
                {"uncleHeaders", {{DataType::Array}, jsonField::Optional}},
                {"expectException", {{DataType::String}, jsonField::Optional}},                   // User information
                {"expectExceptionALL", {{DataType::String}, jsonField::Optional}},                // Legacy field
                {"expectExceptionByzantium", {{DataType::String}, jsonField::Optional}},          // Legacy field
                {"expectExceptionHomestead", {{DataType::String}, jsonField::Optional}},          // Legacy field
                {"expectExceptionEIP150", {{DataType::String}, jsonField::Optional}},             // Legacy field
                {"expectExceptionEIP158", {{DataType::String}, jsonField::Optional}},             // Legacy field
                {"expectExceptionFrontier", {{DataType::String}, jsonField::Optional}},           // Legacy field
                {"expectExceptionConstantinople", {{DataType::String}, jsonField::Optional}},     // Legacy field
                {"expectExceptionConstantinopleFix", {{DataType::String}, jsonField::Optional}},  // Legacy field
                {"expectExceptionIstanbul", {{DataType::String}, jsonField::Optional}},           // Legacy field
                {"blockHeader", {{DataType::Object}, jsonField::Optional}}});

        if (_data->count("expectException"))
            m_exception = _data->atKey("expectException").asString();
        if (_data->count("chainname"))
            m_chainName = _data->atKey("chainname").asString();
        if (_data->count("blocknumber"))
        {
            _data.getContent().atKeyUnsafe("blocknumber").performModifier(mod_valueToCompactEvenHexPrefixed);
            m_blockNumber = spVALUE(new VALUE(_data->atKey("blocknumber")));
        }
        if (_data->count("blockHeader"))
        {
            m_blockHeader = readBlockHeader(_data->atKey("blockHeader"));

            for (auto& tr : _data.getContent().atKeyUnsafe("transactions").getSubObjectsUnsafe())
                m_transactions.emplace_back(readTransaction(dataobject::move(tr)));

            if (_data->count("withdrawals"))
            {
                for (auto const& wt : _data->atKey("withdrawals").getSubObjects())
                    m_withdrawals.emplace_back(spWithdrawal(new Withdrawal(wt)));
            }

            if (_data->count("transactionSequence"))
            {
                for (auto const& tr : _data->atKey("transactionSequence").getSubObjects())
                {
                    string const sException = tr->count("exception") ? tr->atKey("exception").asString() : string();
                    m_transactionSequence.push_back(
                        {readTransaction(BYTES(tr->atKey("rawBytes"))), sException});
                }
            }

            for (auto const& un : _data->atKey("uncleHeaders").getSubObjects())
                m_uncles.emplace_back(readBlockHeader(un));
        }
        m_rlp = sBYTES(_data->atKey("rlp").asString());

        if (_data->count("rlp_decoded"))
        {
            if (_data->count("blockHeader"))
                ETH_ERROR_MESSAGE("Invalid block has blockHeader outside of rlp_decoded!");
            if (_data->count("transactions"))
                ETH_ERROR_MESSAGE("Invalid block has transactions outside of rlp_decoded!");
            if (_data->count("uncleHeaders"))
                ETH_ERROR_MESSAGE("Invalid block has uncles outside of rlp_decoded!");
            if (_data->count("withdrawals"))
                ETH_ERROR_MESSAGE("Invalid block has withdrawals outside of rlp_decoded!");

            auto& rlpDecoded = _data.getContent().atKeyUnsafe("rlp_decoded");
            m_hasRlpDecoded = true;
            REQUIRE_JSONFIELDS(rlpDecoded, "BlockchainTestBlock::rlp_decoded",
            {{"blockHeader", {{DataType::Object}, jsonField::Required}},
             {"transactions", {{DataType::Array}, jsonField::Required}},
             {"uncleHeaders", {{DataType::Array}, jsonField::Required}},
            {"withdrawals", {{DataType::Array}, jsonField::Required}}});

            m_blockHeader = readBlockHeader(rlpDecoded.atKey("blockHeader"));
            for (auto& el : rlpDecoded.atKeyUnsafe("transactions").getSubObjectsUnsafe())
                m_transactions.emplace_back(readTransaction(dataobject::move(el)));
            for (auto const& el : rlpDecoded.atKey("uncleHeaders").getSubObjects())
                m_uncles.emplace_back(readBlockHeader(el));
            for (auto const& el : rlpDecoded.atKey("withdrawals").getSubObjects())
                m_withdrawals.emplace_back(spWithdrawal(new Withdrawal(el)));
        }
    }
    catch (std::exception const& _ex)
    {
        throw UpwardsException(string("BlockchainTestBlock convertion error: ") + _ex.what() + _data->asJson());
    }
}

}  // namespace teststruct
