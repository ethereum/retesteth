#pragma once
#include "../object.h"
#include "scheme_state.h"
#include "scheme_expectAccount.h"

namespace test {

    class scheme_expectState: public object
    {
        public:
        scheme_expectState(DataObject const& _state):
            object(_state)
        {
            for (auto const& accountObj : _state.getSubObjects())
                m_accounts.push_back(scheme_expectAccount(accountObj));
            refreshData();
        }
        std::vector<scheme_expectAccount> const& getAccounts() const {return m_accounts; }

        private:
        std::vector<scheme_expectAccount> m_accounts;
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
    CompareResult compareStates(scheme_expectState const& _stateExpect, scheme_state const& _statePost);
}

