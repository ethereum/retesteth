#pragma once
#include "scheme_account.h"
#include <dataObject/DataObject.h>

namespace test {
class scheme_state : public object
{
public:
    scheme_state(DataObject const& _state) : object(_state)
    {
        if (_state.type() == DataType::String)
            m_hash = _state.asString();
        else
        {
            for (auto const& accountObj : _state.getSubObjects())
                m_accounts.push_back(scheme_account(accountObj));
            refreshData();
        }
    }

    string const& isHash() const { return m_hash; }
    std::vector<scheme_account> const& getAccounts() const { return m_accounts; }
    bool hasAccount(std::string const& _address) const
    {
        for (auto const& a : m_accounts)
        {
            if (a.getData().getKey() == _address)
                return true;
        }
        return false;
    }

    scheme_account const& getAccount(std::string const& _account) const
    {
        assert(hasAccount(_account));
        for (auto const& a : m_accounts)
        {
            if (a.getData().getKey() == _account)
                return a;
        }
        return m_accounts[0];  // should never reach this line
    }

    // Insert precompiled account info into genesis
    DataObject getDataForRPC(std::string const& _network) const;

private:
    std::vector<scheme_account> m_accounts;
    string m_hash;
    void refreshData()
    {
        // update data from account list
        std::string key = m_data.getKey();
        m_data.clear();
        m_data.setKey(key);
        for (auto const& accountObj : m_accounts)
            m_data.addSubObject(accountObj.getData());
    }
};

}

