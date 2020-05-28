#include "AccountIncomplete.h"
#include "../../basetypes.h"
#include "Storage.h"

namespace test
{
namespace teststruct
{
AccountIncomplete::AccountIncomplete(DataObject const& _data)
{
    m_address = spFH20(new FH20(_data.getKey()));
    m_shouldNotExist = _data.count("shouldnotexist");
    if (_data.count("storage"))
        m_storage = spStorage(new Storage(_data.atKey("storage")));
    if (_data.count("balance"))
        m_balance = spVALUE(new VALUE(_data.atKey("balance")));
    if (_data.count("nonce"))
        m_nonce = spVALUE(new VALUE(_data.atKey("nonce")));
    if (_data.count("code"))
        m_code = spBYTES(new BYTES(_data.atKey("code")));
}

const DataObject AccountIncomplete::asDataObject() const
{
    DataObject out;
    string const& addr = m_address.getCContent().asString();
    if (!m_balance.isEmpty())
        out["balance"] = m_balance.getCContent().asString();
    if (!m_nonce.isEmpty())
        out["nonce"] = m_nonce.getCContent().asString();
    if (!m_code.isEmpty())
        out["code"] = m_code.getCContent().asString();
    if (!m_storage.isEmpty())
        out["storage"] = m_storage.getCContent().asDataObject();
    if (m_shouldNotExist)
        out["shouldnotexist"] = "1";
    out.setKey(addr);
    return out;
}


}  // namespace teststruct
}  // namespace test
