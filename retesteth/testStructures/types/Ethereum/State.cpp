#include "State.h"
#include <retesteth/EthChecks.h>

using namespace std;
namespace test::teststruct
{

State::State(std::map<FH20, spAccountBase>& _accList)
{
    // We certain that account provided for the state is full and not incomplete
    m_accounts = _accList;
    m_raw = spDataObject();
    for (auto const& el : _accList)
    {
        ETH_ERROR_REQUIRE_MESSAGE(el.second->type() == AccountType::FullAccount, "State::State(std::map) provided account type is not of a FullAccount type!");
        (*m_raw).atKeyPointer(el.first.asString()) = el.second->asDataObject();  // Recreate export data
    }
}

State::State(spDataObjectMove _data)
{
    try
    {
        m_raw = _data.getPointer();
        for (auto& el : (*m_raw).getSubObjectsUnsafe())
        {
            FH20 key(el->getKey());
            m_accounts[key] = spAccountBase(new Account(el));
        }
        if (m_raw->type() != DataType::Object)
            ETH_ERROR_MESSAGE("State must be initialized from json type `Object`!");
    }
    catch (std::exception const& _ex)
    {
        throw UpwardsException(string("State parse error: ") + _ex.what() + m_raw->asJson());
    }
}

State::Account const& State::getAccount(FH20 const& _address) const
{
    assert(m_accounts.count(_address));
    return dynamic_cast<State::Account const&>(m_accounts.at(_address).getCContent());
}

bool State::hasAccount(State::Account const& _accaunt) const
{
    return m_accounts.count(_accaunt.address());
}

bool State::hasAccount(FH20 const& _address) const
{
    return m_accounts.count(_address);
}

spDataObject const& State::asDataObject() const
{
    // As long as we guarantee unmutability of parsed data in the structure
    // We can return the same data object as we got, not recalculating the whole thing
    return m_raw;
}

}  // namespace teststruct
