#pragma once
#include <testeth/DataObject.h>
#include <testeth/ethObjects/account.h>
#include <testeth/ethObjects/object.h>
#include <testeth/ethObjects/postSectionElement.h>

namespace test {
    enum CompareResult
    {
        Success,
        AccountShouldNotExist,
        MissingExpectedAccount,
        IncorrectBalance,
        IncorrectNonce,
        IncorrectCode,
        None
    };

	class state: public object
	{
		public:
        state(DataObject const& _state):
			object(_state)
		{
            for (auto const& accountObj : _state.getSubObjects())
                m_accounts.push_back(account(accountObj));
            refreshData();
		}

        std::vector<account> const& getAccounts() {return m_accounts; }
        bool hasAccount(std::string const& _address) const
        {
            for (auto const& a: m_accounts)
            {
                if (a.getData().getKey() == _address)
                    return true;
            }
            return false;
        }

        account const& getAccount(std::string const& _account) const
        {
            assert(hasAccount(_account));
            for (auto const& a: m_accounts)
            {
                if (a.getData().getKey() == _account)
                    return a;
            }
            return m_accounts[0]; // should never reach this line
        }

		private:
        std::vector<account> m_accounts;
        void refreshData()
        {
            //update data from account list
            std::string key = m_data.getKey();
            m_data.clear();
            m_data.setKey(key);
            for (auto const& accountObj : m_accounts)
                m_data.addSubObject(accountObj.getData());
        }
	};

    class expectState: public object
    {
        public:
        expectState(DataObject const& _state):
            object(_state)
        {
            for (auto const& accountObj : _state.getSubObjects())
                m_accounts.push_back(expectAccount(accountObj));
            refreshData();
        }
        std::vector<expectAccount> const& getAccounts() const {return m_accounts; }

        private:
        std::vector<expectAccount> m_accounts;
        void refreshData()
        {
            //update data from account list
            std::string key = m_data.getKey();
            m_data.clear();
            m_data.setKey(key);
            for (auto const& accountObj : m_accounts)
                m_data.addSubObject(accountObj.getData());
        }

    };

    /// Check expect section against Post state section
    CompareResult compareStates(expectState const& _stateExpect, state const& _statePost);

    class postState
    {
        public:
        typedef std::vector<postSectionElement> postSectionElements;
        postState(DataObject const& _post)
        {
            BOOST_CHECK_MESSAGE(_post.type() == DataType::Object, "State tests post section must be json object!");
            for(auto const& element: _post.getSubObjects())
            {
               test::checkAllowedNetwork(element.getKey());
               BOOST_CHECK_MESSAGE(element.type() == DataType::Array, "State tests post section value at fork results must be json array!");
               postSectionElements results;
               for (auto const& res: element.getSubObjects())
                   results.push_back(postSectionElement(res));

               m_elements[element.getKey()] = results;
            }
        }
        std::map<std::string, postSectionElements>const& getResults() const { return m_elements; }

        private:
        std::map<std::string, postSectionElements> m_elements;
    };

}

