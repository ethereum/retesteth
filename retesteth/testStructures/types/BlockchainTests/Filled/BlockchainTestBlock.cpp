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
                {"transactions", {{DataType::Array}, jsonField::Optional}},
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
                m_transactions.push_back(readTransaction(dataobject::move(tr)));

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
                m_uncles.push_back(readBlockHeader(un));
        }
        m_rlp = spBYTES(new BYTES(_data->atKey("rlp").asString()));
    }
    catch (std::exception const& _ex)
    {
        throw UpwardsException(string("BlockchainTestBlock convertion error: ") + _ex.what() + _data->asJson());
    }
}

}  // namespace teststruct
