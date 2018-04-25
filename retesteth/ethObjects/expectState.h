#pragma once
#include "state.h"
#include "expectAccount.h"
#include <retesteth/DataObject.h>

namespace test {

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
}

