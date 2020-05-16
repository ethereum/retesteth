#include "Account.h"
#include "../basetypes/BYTES.h"
#include "../basetypes/VALUE.h"
#include "Storage.h"

namespace test
{
namespace teststruct
{
Account::Account(DataObject const& _data) : m_storage(_data.atKey("storage"))
{
    m_balance = VALUE(_data.atKey("balance"));
    m_nonce = VALUE(_data.atKey("nonce"));
    m_code = BYTES(_data.atKey("code"));
}

}  // namespace teststruct
}  // namespace test
