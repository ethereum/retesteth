#pragma once
#include <devcore/Common.h>
#include <testeth/ethObjects/object.h>
#include <testeth/TestOutputHelper.h>
#include <testeth/TestHelper.h>

using namespace dev;
namespace  test {
	class account : public object
	{
		public:
		account(DataObject const& _account):
			object(_account)
		{
            requireJsonFields(_account, "account " + _account.getKey(), {
				{"balance", {{DataType::String}} },
				{"code", {{DataType::String}} },
				{"nonce", {{DataType::String}} },
				{"storage", {{DataType::Object}} }
            });
            // make all fields hex
            m_data.setKey(validateAddress(m_data.getKey()));
		}

        static std::string validateAddress(std::string const& _address)
        {
            static std::string addressAlphabet = "0123456789abcdef";
            std::string rightAddress;
            if(_address.length() == 40)
                rightAddress = "0x" + _address;
            else if (_address.length() == 42)
                rightAddress = _address;
            assert(rightAddress.length() == 42);
            assert(rightAddress[0] == '0' && rightAddress[1] == 'x');
            for (size_t i = 2; i < rightAddress.length(); i++)
                assert(addressAlphabet.find(rightAddress[i]) != std::string::npos);
            return rightAddress;
        }
    };

    class expectAccount: public object
    {
        public:
        expectAccount(DataObject const& _account):
            object(_account)
        {
            m_shouldNotExist = _account.count("shouldNotExist");
            m_hasBalance = _account.count("balance");
            m_hasNonce = _account.count("nonce");
            m_hasCode = _account.count("code");
            m_hasStorage = _account.count("storage");
            m_data.setKey(account::validateAddress(m_data.getKey()));
        }

        bool shouldNotExist() const { return m_shouldNotExist; }
        bool hasBalance() const { return m_hasBalance; }
        bool hasNonce() const { return m_hasNonce; }
        bool hasCode() const { return m_hasCode; }
        bool hasStorage() const { return m_hasStorage; }
        std::string const& address() const { return getData().getKey(); }
        void compareStorage(DataObject const& _storage) const
        {
            BOOST_REQUIRE(_storage.type() == DataType::Object);
            BOOST_REQUIRE(hasStorage());
            DataObject const& myStorage = m_data.at("storage");
            for (auto const& element: myStorage.getSubObjects())
            {
                BOOST_CHECK_MESSAGE(!_storage.count(element.getKey()),
                   TestOutputHelper::get().testName() + " Missing expected storage key in the post state: "
                   + address() + " expected to have storage set at " + element.getKey() + " key");

                std::string valueInStorage = _storage.at(element.getKey()).asString();
                BOOST_CHECK_MESSAGE(valueInStorage == element.asString(),
                   TestOutputHelper::get().testName() + " Check State: " << address()
                   << ": incorrect storage [" << element.getKey() << "] = " << valueInStorage
                   << ", expected [" << element.getKey() << "] = " << element.asString());
            }
            BOOST_CHECK_MESSAGE(myStorage.getSubObjects().size() == _storage.getSubObjects().size(), TestOutputHelper::get().testName()
                 + address() + " storage has more storage records then expected!");
        }

        private:
        bool m_shouldNotExist;
        bool m_hasBalance;
        bool m_hasNonce;
        bool m_hasCode;
        bool m_hasStorage;
    };
}
