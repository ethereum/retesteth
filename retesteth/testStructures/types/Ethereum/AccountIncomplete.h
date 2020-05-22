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
struct AccountIncomplete : GCP_SPointerBase
{
    AccountIncomplete(DataObject const&);
    bool hasStorage() const { return !m_storage.isEmpty(); }
    bool hasCode() const { return !m_code.isEmpty(); }
    bool hasNonce() const { return !m_nonce.isEmpty(); }
    bool hasBalance() const { return !m_balance.isEmpty(); }
    bool shouldNotExist() const { return m_shouldNotExist; }

    VALUE const& balance() const { return m_balance.getCContent(); }
    VALUE const& nonce() const { return m_nonce.getCContent(); }
    Storage const& storage() const { return m_storage.getCContent(); }
    BYTES const& code() const { return m_code.getCContent(); }
    FH20 const& address() const { return m_address.getCContent(); }

    DataObject asDataObject() const;

private:
    bool m_shouldNotExist;
    spFH20 m_address;
    spVALUE m_balance;
    spVALUE m_nonce;
    spStorage m_storage;
    spBYTES m_code;
};

typedef GCP_SPointer<AccountIncomplete> spAccountIncomplete;


}  // namespace teststruct
}  // namespace test
