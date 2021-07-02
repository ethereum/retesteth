#include "Account.h"
#include <retesteth/TestHelper.h>
#include <retesteth/testStructures/Common.h>

namespace test
{
namespace teststruct
{
Account::Account(FH20 const& _addr, VALUE const& _balance, VALUE const& _nonce, BYTES const& _code, Storage const& _storage)
{
    m_address = spFH20(new FH20(_addr));
    m_balance = spVALUE(new VALUE(_balance));
    m_nonce = spVALUE(new VALUE(_nonce));
    m_code = spBYTES(new BYTES(_code));
    m_storage = spStorage(new Storage(_storage));
    if (m_nonce.getCContent() > c_maxNonce)
        ETH_ERROR_MESSAGE("Account `" + m_address->asString() + "` requires nonce <= (2**64)-1");
}

Account::Account(DataObject const& _data)
{
    m_address = spFH20(new FH20(_data.getKey()));
    m_balance = spVALUE(new VALUE(_data.atKey("balance")));
    m_nonce = spVALUE(new VALUE(_data.atKey("nonce")));
    m_code = spBYTES(new BYTES(_data.atKey("code")));
    m_storage = spStorage(new Storage(_data.atKey("storage")));
    if (m_nonce.getCContent() > c_maxNonce)
        ETH_ERROR_MESSAGE("Account `" + m_address->asString() + "` requires nonce <= (2**64)-1");
    requireJsonFields(_data, "Account " + _data.getKey(),
        {{"balance", {{DataType::String}, jsonField::Required}},
         {"code", {{DataType::String}, jsonField::Required}},
         {"nonce", {{DataType::String}, jsonField::Required}},
         {"storage", {{DataType::Object}, jsonField::Required}}});
}

const DataObject Account::asDataObject(ExportOrder _order) const
{
    DataObject out;
    string const& addr = m_address->asString();
    out["balance"] = m_balance->asString();
    out["code"] = m_code->asString();
    out["nonce"] = m_nonce->asString();
    out["storage"] = m_storage->asDataObject();
    out.setKey(addr);

    if (_order == ExportOrder::OldStyle)
    {
        out.setKeyPos("code", 0);
        out.setKeyPos("nonce", 1);
        out.setKeyPos("balance", 2);
    }
    return out;
}

}  // namespace teststruct
}  // namespace test
