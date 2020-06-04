#pragma once
#include "Base/AccountBase.h"
#include <retesteth/dataObject/DataObject.h>
#include <retesteth/dataObject/SPointer.h>
using namespace dataobject;
using namespace test::teststruct;

namespace test
{
namespace teststruct
{
// Ethereum account description
// Describe the strict account defenition
// Constructor require all fields to be set
struct Account : AccountBase
{
    Account(DataObject const&);
    Account(FH20 const& _addr, VALUE const& _balance, VALUE const& _nonce, BYTES const& _code, Storage const& _storage);
    DataObject const asDataObject(ExportOrder order = ExportOrder::Default) const override;

private:
    Account() {}
};

typedef GCP_SPointer<Account> spAccount;


}  // namespace teststruct
}  // namespace test
