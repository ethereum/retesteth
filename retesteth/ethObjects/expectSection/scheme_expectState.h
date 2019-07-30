#pragma once
#include "../object.h"
#include "../stateTest/scheme_state.h"
#include "scheme_expectAccount.h"

namespace test
{
/// State declared in expect section element
class scheme_expectState : public object
{
public:
    scheme_expectState(DataObject const& _state) : object(_state)
    {
        for (auto const& accountObj : _state.getSubObjects())
            m_accounts.push_back(scheme_expectAccount(accountObj));
        refreshData();  // needed?
    }
    bool isNull() const { return m_accounts.size() == 0; }
    std::vector<scheme_expectAccount> const& getAccounts() const { return m_accounts; }
    bool hasBalance(std::string const& _accountKey) const
    {
        for (auto const& acc : m_accounts)
        {
            if (acc.address() == _accountKey)
                return acc.hasBalance();
        }
        return false;
    }
    std::string getBalance(std::string const& _accountKey) const
    {
        for (auto const& acc : m_accounts)
        {
            if (acc.address() == _accountKey)
                return acc.getBalance();
        }
        return std::string();
    }
    void setBalance(std::string const& _accountKey, u256 const& _balance)
    {
        bool changed = false;
        for (auto& acc : m_accounts)
        {
            if (acc.address() == _accountKey)
            {
                acc.setBalance(_balance);
                changed = true;
            }
        }
        if (changed)
            refreshData();
    }

private:
    std::vector<scheme_expectAccount> m_accounts;
    void refreshData()
    {
        // update data from account list
        std::string key = m_data.getKey();
        m_data.clear();
        m_data.setKey(key);
        for (auto& accountObj : m_accounts)
            m_data.addSubObject(accountObj.getData());
    }
};

/// Check expect section against Post state section
CompareResult compareStates(scheme_expectState const& _stateExpect, scheme_state const& _statePost);
}
