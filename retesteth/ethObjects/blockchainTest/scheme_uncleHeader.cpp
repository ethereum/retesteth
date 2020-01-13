#include "scheme_uncleHeader.h"
using namespace test;

scheme_uncleHeader::scheme_uncleHeader(DataObject const& _data) : object(_data)
{
    jsonField isPopulateFromBlockRequired = jsonField::Optional;
    jsonField isSameAsPreviousBlockUncleRequired = jsonField::Optional;
    if (!_data.count("populateFromBlock"))
    {
        m_typeOfUncleSection = typeOfUncleSection::SameAsPreviousBlockUncle;
        isSameAsPreviousBlockUncleRequired = jsonField::Required;
    }
    if (!_data.count("sameAsPreviousBlockUncle"))
    {
        m_typeOfUncleSection = typeOfUncleSection::PopulateFromBlock;
        isPopulateFromBlockRequired = jsonField::Required;
    }
    if (_data.count("sameAsBlock"))
    {
        m_typeOfUncleSection = typeOfUncleSection::SameAsBlock;
        isPopulateFromBlockRequired = jsonField::Optional;  // !Even should not exist!
        isSameAsPreviousBlockUncleRequired = jsonField::Optional;
    }
    if (_data.count("sameAsPreviousSibling"))
    {
        m_typeOfUncleSection = typeOfUncleSection::SameAsPreviousSibling;
        isPopulateFromBlockRequired = jsonField::Optional;  // !Even should not exist!
        isSameAsPreviousBlockUncleRequired = jsonField::Optional;
    }
    if (_data.count("RelTimestampFromPopulateBlock"))
        isPopulateFromBlockRequired = jsonField::Required;

    requireJsonFields(_data, "blockchainTest::uncleblockHeader",
        {{"populateFromBlock", {{DataType::String}, isPopulateFromBlockRequired}},
            {"RelTimestampFromPopulateBlock", {{DataType::String}, jsonField::Optional}},
            {"sameAsPreviousBlockUncle", {{DataType::String}, isSameAsPreviousBlockUncleRequired}},
            {"sameAsPreviousSibling", {{DataType::String}, jsonField::Optional}},
            {"overwriteAndRedoPoW", {{DataType::String}, jsonField::Optional}},
            {"sameAsBlock", {{DataType::String}, jsonField::Optional}},
            {"chainname", {{DataType::String}, jsonField::Optional}},
            {"bloom", {{DataType::String}, jsonField::Optional}},
            {"coinbase", {{DataType::String}, jsonField::Optional}},
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
            {"stateRoot", {{DataType::String}, jsonField::Optional}},
            {"timestamp", {{DataType::String}, jsonField::Optional}},
            {"transactionsTrie", {{DataType::String}, jsonField::Optional}},
            {"uncleHash", {{DataType::String}, jsonField::Optional}}});
}

string const& scheme_uncleHeader::getRelTimestampFromPopulateBlock() const
{
    if (m_data.count("RelTimestampFromPopulateBlock"))
        return m_data.atKey("RelTimestampFromPopulateBlock").asString();
    return object::emptyString;
}

size_t scheme_uncleHeader::getPopulateFrom() const
{
    return (size_t)atoi(m_data.atKey("populateFromBlock").asString().c_str());
}

size_t scheme_uncleHeader::getSameAsBlock() const
{
    return (size_t)atoi(m_data.atKey("sameAsBlock").asString().c_str());
}

size_t scheme_uncleHeader::getSameAsPreviousBlockUncle() const
{
    return (size_t)atoi(m_data.atKey("sameAsPreviousBlockUncle").asString().c_str());
}

size_t scheme_uncleHeader::getSameAsPreviousSibling() const
{
    return (size_t)atoi(m_data.atKey("sameAsPreviousSibling").asString().c_str());
}

string const& scheme_uncleHeader::getOverwrite() const
{
    if (m_data.count("overwriteAndRedoPoW"))
        return m_data.atKey("overwriteAndRedoPoW").asString();
    return object::emptyString;
}

string const& scheme_uncleHeader::getChainName() const
{
    if (m_data.count("chainname"))
        return m_data.atKey("chainname").asString();
    return object::emptyString;
}
