#pragma once
#include "../../../basetypes.h"
#include "../Storage.h"
#include <retesteth/dataObject/DataObject.h>
#include <retesteth/dataObject/SPointer.h>
using namespace dataobject;
using namespace test::teststruct;

namespace test
{
namespace teststruct
{

// Ethereum account description
struct AccountBase : GCP_SPointerBase
{
    bool hasStorage() const { return !m_storage.isEmpty(); }
    bool hasCode() const { return !m_code.isEmpty(); }
    bool hasNonce() const { return !m_nonce.isEmpty(); }
    bool hasBalance() const { return !m_balance.isEmpty(); }
    bool shouldNotExist() const { return m_shouldNotExist; }

    VALUE const& balance() const { return m_balance; }
    VALUE const& nonce() const { return m_nonce; }
    Storage const& storage() const { return m_storage; }
    BYTES const& code() const { return m_code; }
    FH20 const& address() const { return m_address; }

    virtual DataObject const asDataObject(ExportOrder order = ExportOrder::Default) const = 0;
    virtual ~AccountBase() {}

protected:
    AccountBase() {}
    bool m_shouldNotExist = false;
    spFH20 m_address;
    spVALUE m_balance;
    spVALUE m_nonce;
    spStorage m_storage;
    spBYTES m_code;
};

typedef GCP_SPointer<AccountBase> spAccountBase;

}  // namespace teststruct
}  // namespace test
