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
struct Account
{
    Account(DataObject const&);
    VALUE const& balance() const { return m_balance; }
    VALUE const& nonce() const { return m_nonce; }
    Storage const& storage() const { return m_storage; }
    BYTES const& bytes() const { return m_code; }

private:
    VALUE m_balance;
    VALUE m_nonce;
    Storage m_storage;
    BYTES m_code;
};


}  // namespace teststruct
}  // namespace test
