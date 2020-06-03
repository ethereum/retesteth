#include "Account.h"
#include <retesteth/TestHelper.h>
#include <retesteth/ethObjects/object.h>

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
}

Account::Account(DataObject const& _data)
{
    m_address = spFH20(new FH20(_data.getKey()));
    m_balance = spVALUE(new VALUE(_data.atKey("balance")));
    m_nonce = spVALUE(new VALUE(_data.atKey("nonce")));
    m_code = spBYTES(new BYTES(_data.atKey("code")));
    m_storage = spStorage(new Storage(_data.atKey("storage")));
    requireJsonFields(_data, "Account " + _data.getKey(),
        {{"balance", {DataType::String}},
         {"code", {DataType::String}},
         {"nonce", {DataType::String}},
         {"storage", {DataType::Object}}});
}

const DataObject Account::asDataObject() const
{
    DataObject out;
    string const& addr = m_address.getCContent().asString();
    out["balance"] = m_balance.getCContent().asString();
    out["code"] = m_code.getCContent().asString();
    out["nonce"] = m_nonce.getCContent().asString();
    out["storage"] = m_storage.getCContent().asDataObject();
    out.setKey(addr);
    return out;
}

}  // namespace teststruct
}  // namespace test
