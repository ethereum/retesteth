#include "StateIncomplete.h"
#include "AccountIncomplete.h"
#include <retesteth/EthChecks.h>

using namespace std;
namespace test::teststruct
{
StateIncomplete::StateIncomplete(spDataObjectMove _data)
{
    spDataObject data = _data.getPointer();
    try
    {
        for (auto& el : (*data).getSubObjectsUnsafe())
        {
            FH20 const key(el->getKey());
            if (m_accounts.count(key))
            {
                ETH_ERROR_MESSAGE("StateIncomplete::StateIncomplete(DataObject): State has dublicate key: `" + key.asString() + "`");
            }
            m_accounts[key] = spAccountBase(new AccountIncomplete(el));
        }
    }
    catch (std::exception const& _ex)
    {
        throw UpwardsException(string("StateIncomplete parse error: ") + _ex.what() + data->asJson());
    }
}

spDataObject StateIncomplete::asDataObject() const
{
    spDataObject data;
    for (auto const& [accKey, acc] : m_accounts)
    {
        (*data).atKeyPointer(accKey.asString()) = acc->asDataObject();
    }
    return data;
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
        }
    }
}

}  // namespace teststruct
