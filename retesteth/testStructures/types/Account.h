#pragma once
#include "../basetypes/BYTES.h"
#include "../basetypes/FH20.h"
#include "../basetypes/VALUE.h"
#include "Storage.h"
#include <retesteth/dataObject/DataObject.h>
using namespace dataobject;
using namespace test::teststruct;

namespace test
{
namespace teststruct
{
// Ethereum account description
struct Account
{
    Account(DataObject const&);
    VALUE const& balance() const { return *m_balance.getCPtr(); }
    VALUE const& nonce() const { return *m_nonce.getCPtr(); }
    Storage const& storage() const { return *m_storage.getCPtr(); }
    BYTES const& bytes() const { return *m_code.getCPtr(); }
    FH20 const& address() const { return *m_address.getCPtr(); }
    DataObject asDataObject() const;

private:
    Account() {}
    GCP_SPointer<VALUE> m_balance;
    GCP_SPointer<VALUE> m_nonce;
    GCP_SPointer<Storage> m_storage;
    GCP_SPointer<BYTES> m_code;
    GCP_SPointer<FH20> m_address;
};


}  // namespace teststruct
}  // namespace test
