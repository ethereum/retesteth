#include "BlockchainTestFillerUncle.h"
#include <retesteth/EthChecks.h>
#include <retesteth/testStructures/Common.h>

using namespace std;
namespace test::teststruct
{
BlockchainTestFillerUncle::BlockchainTestFillerUncle(DataObject const& _data)
{
    try
    {
        REQUIRE_JSONFIELDS(_data, "BlockchainTestFillerUncle " + _data.getKey(),
            {{"populateFromBlock", {{DataType::String}, jsonField::Optional}},
                {"sameAsPreviousBlockUncle", {{DataType::String}, jsonField::Optional}},
                {"sameAsBlock", {{DataType::String}, jsonField::Optional}},
                {"sameAsPreviousSibling", {{DataType::String}, jsonField::Optional}},
                {"RelTimestampFromPopulateBlock", {{DataType::String}, jsonField::Optional}},
                {"chainname", {{DataType::String}, jsonField::Optional}},
                {"bloom", {{DataType::String}, jsonField::Optional}},
                //{"logsBloom", {{DataType::String}, jsonField::Optional}},
                {"coinbase", {{DataType::String}, jsonField::Optional}},
                //{"author", {{DataType::String}, jsonField::Optional}},
                //{"miner", {{DataType::String}, jsonField::Optional}},
                {"difficulty", {{DataType::String}, jsonField::Optional}},
                {"extraData", {{DataType::String}, jsonField::Optional}},
                {"gasLimit", {{DataType::String}, jsonField::Optional}},
                {"gasUsed", {{DataType::String}, jsonField::Optional}},
                {"hash", {{DataType::String}, jsonField::Optional}},
                {"mixHash", {{DataType::String}, jsonField::Optional}},
                {"nonce", {{DataType::String}, jsonField::Optional}},
                {"number", {{DataType::String}, jsonField::Optional}},
                {"parentHash", {{DataType::String}, jsonField::Optional}},
                {"receiptTrie", {{DataType::String}, jsonField::Optional}},
                //{"receiptsRoot", {{DataType::String}, jsonField::Optional}},
                {"stateRoot", {{DataType::String}, jsonField::Optional}},
                {"timestamp", {{DataType::String}, jsonField::Optional}},
                {"transactionsTrie", {{DataType::String}, jsonField::Optional}},
                {"remove", {{DataType::Array}, jsonField::Optional}},
                {"baseFeePerGas", {{DataType::String}, jsonField::Optional}},
                //{"transactionsRoot", {{DataType::String}, jsonField::Optional}},
                //{"sha3Uncles", {{DataType::String}, jsonField::Optional}},
                {"uncleHash", {{DataType::String}, jsonField::Optional}}});

        if (_data.count("populateFromBlock"))
        {
            // Indicates that uncle will be generated from the blocknumber `populateFromBlock`
            m_typeOfUncleSection = UncleType::PopulateFromBlock;
            m_populateFromBlock = hexOrDecStringToInt(_data.atKey("populateFromBlock").asString());

            if (_data.count("RelTimestampFromPopulateBlock"))
            {
                m_hasRelTimestampFromPopulateBlock = true;
                m_relTimestampFromPopulateBlock = hexOrDecStringToInt(_data.atKey("RelTimestampFromPopulateBlock").asString());
            }
            else
                m_relTimestampFromPopulateBlock = 0;
        }
        else if (_data.count("sameAsPreviousBlockUncle"))
        {
            // Indicates that uncle will be exactly the same as previous block `sameAsPreviousBlockUncle` uncle number
            m_typeOfUncleSection = UncleType::SameAsPreviousBlockUncle;
            m_sameAsPreviousBlockUncle = hexOrDecStringToInt(_data.atKey("sameAsPreviousBlockUncle").asString());
        }
        else if (_data.count("sameAsBlock"))
        {
            // Indicates that uncle will be exactly same as block `sameAsBlock`
            m_typeOfUncleSection = UncleType::SameAsBlock;
            m_sameAsBlock = hexOrDecStringToInt(_data.atKey("sameAsBlock").asString());
        }
        else if (_data.count("sameAsPreviousSibling"))
        {
            // Indicates that uncle will be exactly same as previous uncle `sameAsPreviousSibling` from same block
            m_typeOfUncleSection = UncleType::SameAsPreviousSibling;
            m_sameAsPreviousSibling = hexOrDecStringToInt(_data.atKey("sameAsPreviousSibling").asString());
            if (m_sameAsPreviousSibling == 0)
                ETH_ERROR_MESSAGE("sameAsPreviousSibling:  1 - first uncle, 2 - second uncle");
            else if (m_sameAsPreviousSibling > 5)
                ETH_ERROR_MESSAGE("m_sameAsPreviousSibling sanity error!");
        }

        m_chainName = string();
        if (_data.count("chainname"))
            m_chainName = _data.atKey("chainname").asString();

        // Fields that are to overwrite in uncle's header that will be constructed by test
        // map 'fieldName' -> 'field new value'
        spDataObject _tmpD = convertDecBlockheaderIncompleteToHex(_data);
        DataObject& tmpD = *_tmpD;
        tmpD.removeKey("populateFromBlock");                // BlockchainTestFiller fields
        tmpD.removeKey("sameAsPreviousBlockUncle");         // BlockchainTestFiller fields
        tmpD.removeKey("sameAsBlock");                      // BlockchainTestFiller fields
        tmpD.removeKey("RelTimestampFromPopulateBlock");    // BlockchainTestFiller fields
        tmpD.removeKey("sameAsPreviousSibling");            // BlockchainTestFiller fields

        if (tmpD.getSubObjects().size() > 0)
            m_headerIncomplete = spBlockHeaderIncomplete(new BlockHeaderIncomplete(tmpD));
    }
    catch (std::exception const& _ex)
    {
        throw UpwardsException(string("BlockchainTestFillerUncle convertion error: ") + _ex.what() + _data.asJson());
    }
}

}  // namespace teststruct
