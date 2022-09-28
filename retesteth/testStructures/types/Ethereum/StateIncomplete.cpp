#include "StateIncomplete.h"
#include "AccountIncomplete.h"
#include <retesteth/EthChecks.h>

using namespace std;
namespace test::teststruct
{
StateIncomplete::StateIncomplete(spDataObjectMove _data)
{
    m_rawData = _data.getPointer();
    try
    {
        for (auto& el : (*m_rawData).getSubObjectsUnsafe())
            m_accounts[FH20(el->getKey())] = spAccountBase(new AccountIncomplete(el));
    }
    catch (std::exception const& _ex)
    {
        throw UpwardsException(string("StateIncomplete parse error: ") + _ex.what() + m_rawData->asJson());
    }
}

spDataObject const& StateIncomplete::asDataObject() const
{
    return m_rawData;
}

// Since we do not export StateIncomplete after correctMiningReward
// No need to correct m_rawData, but keep in mind that it won't match
void StateIncomplete::correctMiningReward(FH20 const& _coinbase, VALUE const& _reward)
{
    if (m_accounts.count(_coinbase))
    {
        auto rec = m_accounts.at(_coinbase);
        if (rec.getCContent().hasBalance())
        {
            // We always assume that StateIncomplete is made of AccountIncomplete, but still
            AccountIncomplete& acc = dynamic_cast<AccountIncomplete&>(rec.getContent());
            acc.setBalance(acc.balance() + _reward);
            (*m_rawData).atKeyUnsafe(_coinbase.asString()).atKeyUnsafe("balance").setString(string(acc.balance().asString()));
        }
    }
}

}  // namespace teststruct
