#include "TransactionReader.h"
#include "TransactionAccessList.h"
#include <retesteth/TestHelper.h>

using namespace dataobject;
using namespace test::teststruct;

namespace
{

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
    dev::bytes decodeRLP = test::sfromHex(_rlp.asString());
    dev::RLP rlp(decodeRLP, dev::RLP::VeryStrict);
    return readTransaction(rlp);
}

spTransaction readTransaction(dev::RLP const& _rlp)
{
    if (_rlp.isData())
    {
        dev::bytesConstRef const& p = _rlp.payload();
        dev::RLP realRLP(p.cropped(1, p.size() - 1), dev::RLP::VeryStrict);
        return _readTransaction(TransactionType::ACCESSLIST, realRLP);
    }
    else
        return _readTransaction(TransactionType::LEGACY, _rlp);
}

spTransaction readTransaction(DataObject const& _filledData)
{
    if (_filledData.count("accessList"))
        return spTransaction(new TransactionAccessList(_filledData));
    return spTransaction(new Transaction(_filledData));
}

}  // namespace teststruct
}  // namespace test
