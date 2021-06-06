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
    for (auto& el : _accList)
    {
        m_order.push_back(el->address());
        m_accounts[el->address()] = spAccountBase(&el.getContent());
    }
}

State::State(DataObject const& _data)
{
    try
    {
        for (auto const& el : _data.getSubObjects())
        {
            FH20 key(el.getKey());
            m_order.push_back(key);
            m_accounts[key] = spAccountBase(new Account(el));
        }
        if (m_accounts.size() == 0)
            ETH_ERROR_MESSAGE("State must have at least one record!");
    }
    catch (std::exception const& _ex)
    {
        throw UpwardsException(string("State parse error: ") + _ex.what() + _data.asJson());
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

const DataObject State::asDataObject(ExportOrder _order) const
{
    DataObject out;
    if (_order == ExportOrder::OldStyle)
    {
        for (auto const& el : m_order)
            out.addSubObject(m_accounts.at(el)->asDataObject(_order));
    }
    else
    {
        for (auto const& el : m_accounts)
            out.addSubObject(el.second->asDataObject());
    }
    return out;
}

}  // namespace teststruct
}  // namespace test
