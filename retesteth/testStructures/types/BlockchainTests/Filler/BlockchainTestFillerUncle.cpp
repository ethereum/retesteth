#include "BlockchainTestFillerUncle.h"
#include <retesteth/EthChecks.h>
#include <retesteth/ethObjects/object.h>

namespace test
{
namespace teststruct
{
BlockchainTestFillerUncle::BlockchainTestFillerUncle(DataObject const& _data)
{
    if (_data.count("populateFromBlock"))
    {
        // Indicates that uncle will be generated from the blocknumber `populateFromBlock`
        m_typeOfUncleSection = UncleType::PopulateFromBlock;
        m_populateFromBlock = test::hexOrDecStringToInt(_data.atKey("populateFromBlock").asString());

        m_relTimestampFromPopulateBlock = test::hexOrDecStringToInt(_data.atKey("RelTimestampFromPopulateBlock").asString());
    }
    else if (_data.count("sameAsPreviousBlockUncle"))
    {
        // Indicates that uncle will be exactly the same as previous block `sameAsPreviousBlockUncle` uncle number
        m_typeOfUncleSection = UncleType::SameAsPreviousBlockUncle;
        m_sameAsPreviousBlockUncle = test::hexOrDecStringToInt(_data.atKey("sameAsPreviousBlockUncle").asString());
    }
    else if (_data.count("sameAsBlock"))
    {
        // Indicates that uncle will be exactly same as block `sameAsBlock`
        m_typeOfUncleSection = UncleType::SameAsBlock;
        m_sameAsBlock = test::hexOrDecStringToInt(_data.atKey("sameAsBlock").asString());
    }
    else if (_data.count("sameAsPreviousSibling"))
    {
        // Indicates that uncle will be exactly same as previous uncle `sameAsPreviousSibling` from same block
        m_typeOfUncleSection = UncleType::SameAsPreviousSibling;
        m_sameAsPreviousSibling = test::hexOrDecStringToInt(_data.atKey("sameAsPreviousSibling").asString());
        if (m_sameAsPreviousSibling == 0)
            ETH_ERROR_MESSAGE("sameAsPreviousSibling:  1 - first uncle, 2 - second uncle");
        else if (m_sameAsPreviousSibling > 5)
            ETH_ERROR_MESSAGE("m_sameAsPreviousSibling sanity error!");
    }

    if (_data.count("chainname"))
        m_chainName = _data.atKey("chainname").asString();


    // Fields that are to overwrite in uncle's header that will be constructed by test
    // map 'fieldName' -> 'field new value'
    if (_data.count("overwriteAndRedoPoW"))
        m_headerIncomplete = spBlockHeaderIncomplete(new BlockHeaderIncomplete(_data));
}

}  // namespace teststruct
}  // namespace test
