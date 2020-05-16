#include "AccountIncomplete.h"
#include "../basetypes/BYTES.h"
#include "../basetypes/VALUE.h"
#include "Storage.h"

namespace test
{
namespace teststruct
{
AccountIncomplete::AccountIncomplete(DataObject const& _data)
{
    if (_data.count("storage"))
        m_storage = GCP_SPointer<Storage>(new Storage(_data.atKey("storage")));
    if (_data.count("balance"))
        m_balance = GCP_SPointer<VALUE>(new VALUE(_data.atKey("balance")));
    if (_data.count("nonce"))
        m_nonce = GCP_SPointer<VALUE>(new VALUE(_data.atKey("nonce")));
    if (_data.count("code"))
        m_code = GCP_SPointer<BYTES>(new BYTES(_data.atKey("code")));
}

}  // namespace teststruct
}  // namespace test
