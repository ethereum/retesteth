#include "StateIncomplete.h"
#include "AccountIncomplete.h"
#include <retesteth/EthChecks.h>
#include <retesteth/testStructures/Common.h>

namespace test
{
namespace teststruct
{
StateIncomplete::StateIncomplete(spDataObjectMove _data)
{
    m_rawData = _data.getPointer();
    try
    {
        for (auto const& el : m_rawData->getSubObjects())
            m_accounts[FH20(el->getKey())] = spAccountBase(new AccountIncomplete(el));
    }
    catch (std::exception const& _ex)
    {
        throw UpwardsException(string("StateIncomplete parse error: ") + _ex.what() + m_rawData->asJson());
    }
}

spDataObject StateIncomplete::asDataObject(ExportOrder) const
{
    spDataObject out(new DataObject());
    for (auto const& el : m_accounts)
        (*out).addSubObject(el.second->asDataObject());
    return out;
}

void StateIncomplete::correctMiningReward(FH20 const& _coinbase, VALUE const& _reward)
{
    for (auto& el : m_accounts)
    {
        // We always assume that StateIncomplete is made of AccountIncomplete, but still
        AccountIncomplete& acc = dynamic_cast<AccountIncomplete&>(el.second.getContent());
        if (el.first == _coinbase && acc.hasBalance())
            acc.setBalance(acc.balance() + _reward);
    }
}

}  // namespace teststruct
}  // namespace test
