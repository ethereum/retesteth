#pragma once
#include <libdevcore/RLP.h>
#include <retesteth/dataObject/DataObject.h>
#include <retesteth/ethObjects/common.h>
#include <string>

using namespace dataobject;
namespace toolimpl
{
// DataObject modifiers
void mod_valuesToLowerCase(DataObject&);
void mod_removeLeadingZerosFromHexValuesEVEN(DataObject& _obj);

// Reading dev::RLP into string
std::string rlpToString(dev::RLP const&, bool _corretHexOdd = false);

// Sanitize ethereum objects from RLP
class BlockHeadFromRLP
{
public:
    BlockHeadFromRLP(dev::RLP const&);

private:
    class validator
    {
    public:
        validator(dev::RLP const&);
    };
    validator m_validator;
    DataObject rlpToData(dev::RLP const&);

public:
    scheme_blockHeader header;
};

class TransactionFromRLP
{
public:
    TransactionFromRLP(dev::RLP const&);

private:
    class validator
    {
    public:
        validator(dev::RLP const&);
    };
    validator m_validator;
    DataObject rlpToData(dev::RLP const&);

public:
    scheme_transaction transaction;
};
}  // namespace toolimpl
