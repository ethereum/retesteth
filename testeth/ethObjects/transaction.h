#pragma once
#include <testeth/DataObject.h>
#include <testeth/TestHelper.h>
#include <testeth/ethObjects/object.h>

namespace test {
	class transaction: public object
	{
		public:
		transaction(DataObject const& _transaction):
			object(_transaction)
		{
			test::requireJsonFields(_transaction, "transaction", {
				{"data", {{DataType::String}} },
				{"gasLimit", {{DataType::String}} },
				{"gasPrice", {{DataType::String}} },
				{"nonce", {{DataType::String}} },
				{"secretKey", {{DataType::String}} },
				{"to", {{DataType::String}} },
				{"value", {{DataType::String}} }
			});
			m_data["version"] = "0x01";

			// convert into rpc format
			m_data["gas"] = m_data["gasLimit"].asString();
		}
	};
}

