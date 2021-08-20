#include "State.h"
#include "Account.h"
#include <retesteth/EthChecks.h>

namespace test
{
namespace teststruct
{
State::State(std::vector<spAccount>& _accList)
{
    // Here spAccountBase will take control of spAccount content and increase its refCount
    // AccountBase will handle all the logic for Account, but with this constructor
    // We certain that account provided for the state is full and not incomplete
    m_raw = spDataObject(new DataObject());
    for (auto& el : _accList)
    {
        m_accounts[el->address()] = spAccountBase(&el.getContent());
        (*m_raw).atKeyPointer(el->address().asString()) = el.getContent().asDataObject();
    }
}

State::State(spDataObjectMove _data)
{
    try
    {
        m_raw = _data.getPointer();
        for (auto const& el : m_raw->getSubObjects())
        {
            FH20 key(el->getKey());
            m_accounts[key] = spAccountBase(new Account(el));
        }
        if (m_accounts.size() == 0)
            ETH_ERROR_MESSAGE("State must have at least one record!");
    }
    catch (std::exception const& _ex)
    {
        throw UpwardsException(string("State parse error: ") + _ex.what() + m_raw->asJson());
    }
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

spDataObject State::asDataObject() const
{
    // As long as we guarantee unmutability of parsed data in the structure
    // We can return the same data object as we got, not recalculating the whole thing
    return m_raw;
}

}  // namespace teststruct
}  // namespace test
