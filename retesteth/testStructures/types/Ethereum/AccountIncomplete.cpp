#include "AccountIncomplete.h"
#include "../../basetypes.h"
#include "Storage.h"
#include <retesteth/EthChecks.h>
#include <retesteth/testStructures/Common.h>

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
    {
        m_nonce = spVALUE(new VALUE(_data.atKey("nonce")));
        if (m_nonce.getCContent() > c_maxNonce)
            ETH_ERROR_MESSAGE("AccountIncomplete `" + m_address->asString() +  "` requires nonce <= (2**64)-1");
    }
    if (_data.count("code"))
        m_code = spBYTES(new BYTES(_data.atKey("code")));
    requireJsonFields(_data, "AccountIncomplete " + _data.getKey(),
        {{"shouldnotexist", {{DataType::String}, jsonField::Optional}},
         {"storage", {{DataType::Object}, jsonField::Optional}},
         {"balance", {{DataType::String}, jsonField::Optional}},
         {"nonce", {{DataType::String}, jsonField::Optional}},
         {"code", {{DataType::String}, jsonField::Optional}}});
    ETH_ERROR_REQUIRE_MESSAGE(_data.getSubObjects().size() > 0, "AccountIncomplete must have at least one object!");
}

spDataObject AccountIncomplete::asDataObject(ExportOrder) const
{
    spDataObject _out(new DataObject());
    DataObject& out = _out.getContent();
    string const& addr = m_address->asString();
    if (!m_balance.isEmpty())
        out["balance"] = m_balance->asString();
    if (!m_nonce.isEmpty())
        out["nonce"] = m_nonce->asString();
    if (!m_code.isEmpty())
        out["code"] = m_code->asString();
    if (!m_storage.isEmpty())
        out.atKeyPointer("storage") = m_storage->asDataObject();
    if (m_shouldNotExist)
        out["shouldnotexist"] = "1";
    out.setKey(addr);
    return _out;
}


}  // namespace teststruct
}  // namespace test
