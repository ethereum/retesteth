#pragma once
#include "../object.h"
#include <retesteth/TestHelper.h>

using namespace dev;
namespace  test {
	enum CompareResult
	{
		Success,
		AccountShouldNotExist,
		MissingExpectedAccount,
		IncorrectBalance,
		IncorrectNonce,
		IncorrectCode,
		IncorrectStorage,
		None
	};

	class scheme_account : public object
	{
		public:
		scheme_account(DataObject const& _account):
			object(_account)
		{
            requireJsonFields(_account, "account " + _account.getKey(), {
                {"balance", {DataType::String} },
                {"code", {DataType::String} },
                {"nonce", {DataType::String} },
                {"storage", {DataType::Object} }
            });

			// Compile the code
			m_data["code"] = test::replaceCode(m_data.at("code").asString());
            m_data["balance"] = dev::toCompactHexPrefixed(
                u256(m_data.at("balance").asString()), 1);  // fix odd strings

            // Make all fields hex
            m_data.setKey(makeHexAddress(m_data.getKey()));
            makeAllFieldsHex(m_data);
		}
    };
}
