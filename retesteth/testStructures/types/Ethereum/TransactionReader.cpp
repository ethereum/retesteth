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
    if (_rlp.asString().substr(2, 2) == "01")
    {
        dev::bytes decodeRLP = test::sfromHex(_rlp.asString().substr(4));
        dev::RLP rlp(decodeRLP, dev::RLP::VeryStrict);
        return _readTransaction(TransactionType::ACCESSLIST, rlp);
    }
    else
    {
        dev::bytes decodeRLP = test::sfromHex(_rlp.asString());
        dev::RLP rlp(decodeRLP, dev::RLP::VeryStrict);
        return _readTransaction(TransactionType::LEGACY, rlp);
    }
}

spTransaction readTransaction(dev::RLP const& _rlp)
{
    // TODO: Blokchain Tests do not support new transaction type!
    return _readTransaction(TransactionType::LEGACY, _rlp);
}

}  // namespace teststruct
}  // namespace test
