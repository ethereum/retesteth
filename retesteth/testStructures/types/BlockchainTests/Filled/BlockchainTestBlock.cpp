#include "BlockchainTestBlock.h"
#include <retesteth/EthChecks.h>
#include <retesteth/testStructures/Common.h>

namespace test
{
namespace teststruct
{
BlockchainTestBlock::BlockchainTestBlock(DataObject const& _data)
{
    try
    {
        if (_data.count("chainname"))
            m_chainName = _data.atKey("chainname").asString();
        if (_data.count("blocknumber"))
        {
            DataObject tmpD = _data.atKey("blocknumber");
            tmpD.performModifier(mod_valueToCompactEvenHexPrefixed);
            m_blockNumber = spVALUE(new VALUE(tmpD));
        }
        if (_data.count("blockHeader"))
        {
            m_blockHeader = readBlockHeader(_data.atKey("blockHeader"));

            for (auto const& tr : _data.atKey("transactions").getSubObjects())
                m_transactions.push_back(readTransaction(tr));

            // Can't really check that yet. the field is for custom test runners
            if (_data.count("transactionSequence"))
                for (auto const& tr : _data.atKey("transactionSequence").getSubObjects())
                    m_transactionSequence.push_back(readTransaction(BYTES(tr.atKey("rawBytes"))));

            for (auto const& un : _data.atKey("uncleHeaders").getSubObjects())
                m_uncles.push_back(readBlockHeader(un));
        }
        m_rlp = spBYTES(new BYTES(_data.atKey("rlp").asString()));

        requireJsonFields(_data, "BlockchainTestBlock " + _data.getKey(),
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
    }
    catch (std::exception const& _ex)
    {
        throw UpwardsException(string("BlockchainTestBlock convertion error: ") + _ex.what() + _data.asJson());
    }
}

}  // namespace teststruct
}  // namespace test
