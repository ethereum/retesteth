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
    m_balance = GCP_SPointer<VALUE>(new VALUE(_data.atKey("balance")));
    m_nonce = GCP_SPointer<VALUE>(new VALUE(_data.atKey("nonce")));
    m_code = GCP_SPointer<BYTES>(new BYTES(_data.atKey("code")));
    m_address = GCP_SPointer<FH20>(new FH20(_data.getKey()));
    m_storage = GCP_SPointer<Storage>(new Storage(_data.atKey("storage")));
}

DataObject Account::asDataObject() const
{
    DataObject out;
    string const& addr = m_address.getCPtr()->asString();
    out[addr]["balance"] = m_balance.getCPtr()->asString();
    out[addr]["nonce"] = m_nonce.getCPtr()->asString();
    out[addr]["code"] = m_code.getCPtr()->asString();
    out[addr]["storage"] = m_storage.getCPtr()->asDataObject();
    return out;
}

}  // namespace teststruct
}  // namespace test
