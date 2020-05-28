#include "StateIncomplete.h"
#include "AccountIncomplete.h"

namespace test
{
namespace teststruct
{
StateIncomplete::StateIncomplete(DataObject const& _data)
{
    for (auto const& el : _data.getSubObjects())
        m_accounts[FH20(el.getKey())] = spAccountBase(new AccountIncomplete(el));
}

const DataObject StateIncomplete::asDataObject() const
{
    DataObject out;
    for (auto const& el : m_accounts)
        out.addSubObject(el.second.getCContent().asDataObject());
    return out;
}

void StateIncomplete::correctMiningReward(FH20 const& _coinbase, VALUE const& _reward)
{
    for (auto& el : m_accounts)
    {
        AccountIncomplete& acc = dynamic_cast<AccountIncomplete&>(el.second.getContent());
        if (el.first == _coinbase && acc.hasBalance())
            acc.setBalance(acc.balance() + _reward);
    }
}

}  // namespace teststruct
}  // namespace test
