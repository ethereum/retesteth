#pragma once
#include "../object.h"

#include <libdevcore/Address.h>
#include <libdevcore/RLP.h>
#include <libdevcore/SHA3.h>
#include <retesteth/TestHelper.h>

namespace test
{
class scheme_debugAccountRange : public object
{
public:
    scheme_debugAccountRange(DataObject const& _receipt) : object(_receipt)
    {
        requireJsonFields(_receipt, "debug_accountRange_rpc_response",
            {{"addressMap", {DataType::Object}}, {"nextKey", {DataType::String}}});
        makeAllFieldsHex(m_data);

        for (auto const& element : m_data.atKey("addressMap").getSubObjects())
            m_accountMap.addSubObject(element.getKey(), element.asString());
    }

    DataObject const& getAccountMap() const { return m_accountMap; }

    bool isNextKey() const
    {
        if (m_data.atKey("nextKey").asString() == "0x00" || m_data.atKey("nextKey").asString() == "0x0000000000000000000000000000000000000000000000000000000000000000")
            return false;
        return true;
    }

private:
    DataObject m_accountMap;
};
}  // namespace test
