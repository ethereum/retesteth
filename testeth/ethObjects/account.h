#pragma once
#include <devcore/Common.h>
#include <testeth/TestHelper.h>
#include <testeth/ethObjects/object.h>

using namespace dev;
namespace  test {
	class account : public object
	{
		public:
		account(DataObject const& _account):
			object(_account)
		{
			test::requireJsonFields(_account, "account " + _account.getKey(), {
				{"balance", {{DataType::String}} },
				{"code", {{DataType::String}} },
				{"nonce", {{DataType::String}} },
				{"storage", {{DataType::Object}} }
			});
		}
	};
}
