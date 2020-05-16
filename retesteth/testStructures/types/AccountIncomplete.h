#pragma once
#include "../basetypes/BYTES.h"
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
struct AccountIncomplete
{
    AccountIncomplete(DataObject const&);
    bool hasStorage() { return m_storage.isEmpty(); }
    bool hasCode() { return m_code.isEmpty(); }
    bool hasNonce() { return m_nonce.isEmpty(); }
    bool hasBalance() { return m_balance.isEmpty(); }
    VALUE const& balance() const { return *m_balance; }
    VALUE const& nonce() const { return *m_nonce; }
    Storage const& storage() const { return *m_storage; }
    BYTES const& bytes() const { return *m_code; }

private:
    GCP_SPointer<VALUE> m_balance;
    GCP_SPointer<VALUE> m_nonce;
    GCP_SPointer<Storage> m_storage;
    GCP_SPointer<BYTES> m_code;
};


}  // namespace teststruct
}  // namespace test
