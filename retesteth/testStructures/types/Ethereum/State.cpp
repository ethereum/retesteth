#include "State.h"
#include <retesteth/EthChecks.h>

using namespace std;
namespace test::teststruct
{

State::State(std::map<FH20, spAccountBase>& _accList)
{
    // We certain that account provided for the state is full and not incomplete
    m_accounts = _accList;
    for (auto const& el : _accList)
    {
        ETH_ERROR_REQUIRE_MESSAGE(el.second->type() == AccountType::FullAccount, "State::State(std::map) provided account type is not of a FullAccount type!");
    }
}

State::State(spDataObjectMove _data)
{
    spDataObject data = _data.getPointer();
    try
    {
        for (auto& el : (*data).getSubObjectsUnsafe())
        {
            FH20 key(el->getKey());
            if (m_accounts.count(key))
            {
                ETH_ERROR_MESSAGE("State::State(DataObject): State has dublicate key: `" + key.asString() + "`");
            }
            m_accounts[key] = spAccountBase(new Account(el));
        }
        if (data->type() != DataType::Object)
            ETH_ERROR_MESSAGE("State must be initialized from json type `Object`!");
    }
    catch (std::exception const& _ex)
    {
        throw UpwardsException(string("State parse error: ") + _ex.what() + data->asJson());
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

spDataObject State::asDataObject() const
{
    spDataObject data = sDataObject(DataType::Object);
    for (auto const& [accKey, acc] : m_accounts)
    {
        (*data).atKeyPointer(accKey.asString()) = acc->asDataObject();
    }
    return data;
}

void State::addAccount(spAccountBase _acc)
{
    if (m_accounts.contains(_acc->address()))
    {
        ETH_ERROR_MESSAGE("State::addAccount: State has dublicate key: `" + _acc->address().asString() + "`");
    }
    m_accounts.emplace(_acc->address(), _acc);
}

}  // namespace teststruct
