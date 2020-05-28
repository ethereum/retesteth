#include "State.h"
#include "Account.h"

namespace test
{
namespace teststruct
{
State::State(std::map<FH20, spAccount>& _accList)
{
    for (auto& el : _accList)
        m_accounts[el.first] = spAccountBase(&el.second.getContent());
}

State::State(DataObject const& _data)
{
    for (auto const& el : _data.getSubObjects())
        m_accounts[FH20(el.getKey())] = spAccountBase(new Account(el));
}

Account const& State::getAccount(FH20 const& _address) const
{
    assert(m_accounts.count(_address));
    return dynamic_cast<Account const&>(m_accounts.at(_address).getCContent());
}

bool State::hasAccount(Account const& _accaunt) const
{
    return m_accounts.count(_accaunt.address());
}

bool State::hasAccount(FH20 const& _address) const
{
    return m_accounts.count(_address);
}

const DataObject State::asDataObject() const
{
    DataObject out;
    for (auto const& el : m_accounts)
        out.addSubObject(el.second.getCContent().asDataObject());
    return out;
}

}  // namespace teststruct
}  // namespace test
