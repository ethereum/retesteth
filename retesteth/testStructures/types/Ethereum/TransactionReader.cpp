#include "TransactionReader.h"
#include "TransactionAccessList.h"
#include <retesteth/TestHelper.h>

using namespace dataobject;
using namespace test::teststruct;

namespace
{
enum class TransactionType
{
    LEGACY,
    ACCESSLIST
};

TransactionType getTxTypeFromRLP(dev::RLP const& _rlp)
{
    // see what is tx type
    if (_rlp.itemCount() == 9)
        return TransactionType::LEGACY;
    else
        return TransactionType::ACCESSLIST;
}

spTransaction _readTransaction(TransactionType _t, dev::RLP const& _rlp)
{
    spTransaction spTr;
    switch (_t)
    {
    case TransactionType::LEGACY:
        spTr = spTransaction(new Transaction(_rlp));
        break;
    case TransactionType::ACCESSLIST:
        spTr = spTransaction(new TransactionAccessList(_rlp));
        break;
    }
    return spTr;
}

}  // namespace
namespace test
{
namespace teststruct
{
spTransaction readTransaction(BYTES const& _rlp)
{
    // see what is tx type
    dev::bytes decodeRLP = test::sfromHex(_rlp.asString());
    dev::RLP rlp(decodeRLP, dev::RLP::VeryStrict);

    TransactionType trType = getTxTypeFromRLP(rlp);
    return _readTransaction(trType, rlp);
}

spTransaction readTransaction(dev::RLP const& _rlp)
{
    TransactionType trType = getTxTypeFromRLP(_rlp);
    return _readTransaction(trType, _rlp);
}

}  // namespace teststruct
}  // namespace test
