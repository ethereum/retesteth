#pragma once
#include "object.h"
#include <libdevcore/Common.h>
#include <retesteth/TestOutputHelper.h>
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

	class account : public object
	{
		public:
		account(DataObject const& _account):
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

            // Make all fields hex
            m_data.setKey(makeHexAddress(m_data.getKey()));
            makeAllFieldsHex(m_data);
		}
    };
}
