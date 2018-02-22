#pragma once
#include <devcore/Common.h>
#include <testeth/TestHelper.h>

using namespace dev;
namespace  test {
	class account
	{
		public:
		account(DataObject const& _account):
			m_account(_account)
		{
			test::requireJsonFields(_account, "account " + _account.getKey(), {
				{"balance", DataType::String},
				{"code", DataType::String},
				{"nonce", DataType::String},
				{"storage", DataType::Object}
			});
		}
		private:
		DataObject m_account;

	};
}
