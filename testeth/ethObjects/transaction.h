#pragma once
#include <testeth/DataObject.h>
#include <testeth/TestHelper.h>

namespace test {
	class transaction
	{
		public:
		transaction(DataObject const& _transaction):
			m_transaction(_transaction)
		{
			test::requireJsonFields(_transaction, "transaction", {
				{"data", DataType::String},
				{"gasLimit", DataType::String},
				{"gasPrice", DataType::String},
				{"nonce", DataType::String},
				{"secretKey", DataType::String},
				{"to", DataType::String},
				{"value", DataType::String}
			});
		}
		private:
		DataObject m_transaction;
	};
}

