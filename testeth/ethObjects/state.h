#pragma once
#include <testeth/DataObject.h>
#include <testeth/ethObjects/account.h>
#include <testeth/ethObjects/object.h>

namespace test {
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
            // refresh data with information from m_accounts
            std::string key = m_data.getKey();
            m_data.clear();
            for (auto const& accountObj : m_accounts)
                m_data.addSubObject(accountObj.getData());
            m_data.setKey(key);
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
            // refresh data with information from m_accounts
            std::string key = m_data.getKey();
            m_data.clear();
            for (auto const& accountObj : m_accounts)
                m_data.addSubObject(accountObj.getData());
            m_data.setKey(key);
        }
    };

    /// Check expect section against Post state section
    bool compareStates(expectState const& _stateExpect, state const& _statePost);

}

