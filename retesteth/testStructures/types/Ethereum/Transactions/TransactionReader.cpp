#include "TransactionReader.h"
#include "TransactionAccessList.h"
#include "TransactionBaseFee.h"
#include "TransactionBlob.h"
#include <retesteth/EthChecks.h>
#include <retesteth/helpers/TestHelper.h>
#include <retesteth/Constants.h>

using namespace std;
using namespace dataobject;
using namespace test::teststruct;
using namespace test::teststruct::constnames;

namespace
{

spTransaction _readTransaction(TransactionType _t, dev::RLP const& _rlp)
{
    spTransaction spTr;
    switch (_t)
    {
    case TransactionType::LEGACY:
        spTr = spTransaction(new TransactionLegacy(_rlp));
        break;
    case TransactionType::ACCESSLIST:
        spTr = spTransaction(new TransactionAccessList(_rlp));
        break;
    case TransactionType::BASEFEE:
        spTr = spTransaction(new TransactionBaseFee(_rlp));
        break;
    case TransactionType::BLOB:
        spTr = spTransaction(new TransactionBlob(_rlp));
        break;
    }
    return spTr;
}

}  // namespace
namespace test::teststruct
{
// The transaction here might be  01 + rlp (tx.data)
// OR                             rlp (01 + rlp (tx.data))
// Depends on where we decode it from (block or eth_sendRawTransaction)

spTransaction readTransaction(BYTES const& _rlp)
{
    string const& sRLP = _rlp.asString();
    dev::bytes decodeRLP = test::sfromHex(sRLP);
    try
    {
        dev::RLP rlp(decodeRLP, dev::RLP::VeryStrict);
        return readTransaction(rlp);
    }
    catch (...)
    {
        // Make an attempt to read as typedTransaction wrapped in RLP
        dev::RLPStream str;
        str << decodeRLP;
        try
        {
            dev::RLP rlp(str.out(), dev::RLP::VeryStrict);
            return readTransaction(rlp);
        }
        catch (std::exception const& _ex)
        {
            throw test::UpwardsException(
                string("readTransaction(BYTES const&) error building RLP: ") + _ex.what() + "\n" + _rlp.asString());
        }
    }
    return spTransaction(0);
}

spTransaction readTransaction(dev::RLP const& _rlp)
{
    if (_rlp.isData())
    {
        dev::bytesConstRef const& p = _rlp.payload();
        dev::RLP realRLP(p.cropped(1, p.size() - 1), dev::RLP::VeryStrict);
        dev::bytesConstRef const& type = p.cropped(0, 1);
        const int itype = (int)type.toString()[0];
        switch (itype)
        {
        case 1:
            return _readTransaction(TransactionType::ACCESSLIST, realRLP);
        case 2:
            return _readTransaction(TransactionType::BASEFEE, realRLP);
        case 3:
            return _readTransaction(TransactionType::BLOB, realRLP);
        default:
            throw test::UpwardsException("readTransaction(dev::RLP const& _rlp) unknown transaction type!");
        }
        return spTransaction(0);
    }
    else
        return _readTransaction(TransactionType::LEGACY, _rlp);
}

spTransaction readTransaction(spDataObjectMove _filledData)
{
    if (_filledData.getPointer()->count(c_blobVersionedHashes))
        return spTransaction(new TransactionBlob(_filledData));
    if (_filledData.getPointer()->count(c_maxPriorityFeePerGas))
        return spTransaction(new TransactionBaseFee(_filledData));
    if (_filledData.getPointer()->count(c_accessList))
        return spTransaction(new TransactionAccessList(_filledData));
    return spTransaction(new TransactionLegacy(_filledData));
}

}  // namespace teststruct
