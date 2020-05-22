#pragma once
#include "../../basetypes.h"
#include "Storage.h"
#include <retesteth/dataObject/DataObject.h>
#include <retesteth/dataObject/SPointer.h>
using namespace dataobject;
using namespace test::teststruct;

namespace test
{
namespace teststruct
{
// Ethereum account description
struct Account : GCP_SPointerBase
{
    Account(DataObject const&);
    Account(FH20 const& _addr, VALUE const& _balance, VALUE const& _nonce, BYTES const& _code, Storage const& _storage);
    VALUE const& balance() const { return m_balance.getCContent(); }
    VALUE const& nonce() const { return m_nonce.getCContent(); }
    Storage const& storage() const { return m_storage.getCContent(); }
    BYTES const& code() const { return m_code.getCContent(); }
    FH20 const& address() const { return m_address.getCContent(); }
    DataObject asDataObject() const;

private:
    Account() {}
    spFH20 m_address;
    spVALUE m_balance;
    spVALUE m_nonce;
    spBYTES m_code;
    spStorage m_storage;
};

typedef GCP_SPointer<Account> spAccount;


}  // namespace teststruct
}  // namespace test
