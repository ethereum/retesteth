#include "Account.h"
#include "../basetypes/BYTES.h"
#include "../basetypes/VALUE.h"
#include "Storage.h"
#include <retesteth/TestHelper.h>
#include <retesteth/ethObjects/object.h>

namespace test
{
namespace teststruct
{
Account::Account(DataObject const& _data)
{
    m_balance = spVALUE(new VALUE(_data.atKey("balance")));
    m_nonce = spVALUE(new VALUE(_data.atKey("nonce")));
    m_code = GCP_SPointer<BYTES>(new BYTES(_data.atKey("code")));
    m_address = GCP_SPointer<FH20>(new FH20(_data.getKey()));
    m_storage = GCP_SPointer<Storage>(new Storage(_data.atKey("storage")));
}

DataObject Account::asDataObject() const
{
    DataObject out;
    string const& addr = m_address.getCPtr()->asString();
    out["balance"] = m_balance.getCPtr()->asString();
    out["nonce"] = m_nonce.getCPtr()->asString();
    out["code"] = m_code.getCPtr()->asString();
    out["storage"] = m_storage.getCPtr()->asDataObject();
    out.setKey(addr);
    return out;
}

}  // namespace teststruct
}  // namespace test
