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
            {{c_accounts, {DataType::Object}}, {c_next, {DataType::String}}});
        makeAllFieldsHex(m_data);

        for (auto const& element : m_data.atKey(c_accounts).getSubObjects())
            m_accountMap.addSubObject(element.getKey(), element.asString());
    }

    DataObject const& getAccountMap() const { return m_accountMap; }

    bool isNextKey() const
    {
        if (m_data.atKey(c_next).asString() == "0x00" ||
            m_data.atKey(c_next).asString() ==
                "0x0000000000000000000000000000000000000000000000000000000000000000")
            return false;
        return true;
    }

private:
    const string c_next = "next";
    const string c_accounts = "accounts";
    DataObject m_accountMap;
};
}  // namespace test
