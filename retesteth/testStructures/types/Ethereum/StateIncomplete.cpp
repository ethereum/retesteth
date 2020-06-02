#include "StateIncomplete.h"
#include "AccountIncomplete.h"
#include <retesteth/ethObjects/object.h>

namespace test
{
namespace teststruct
{
StateIncomplete::StateIncomplete(DataObject const& _data, DataRequier _req)
{
    if (_req == DataRequier::ALLOWDEC)
    {
        // Convert Expect Section IncompletePostState fields to hex, account keys add `0x` prefix
        DataObject tmpD = _data;
        for (auto& acc : tmpD.getSubObjectsUnsafe())
        {
            string const& key = acc.getKey();
            if (key.size() > 2 && (key[0] != '0' || key[1] != 'x'))
                acc.setKey("0x" + acc.getKey());
            if (acc.count("balance"))
                acc["balance"].performModifier(mod_valueToCompactEvenHexPrefixed);
            if (acc.count("nonce"))
                acc["nonce"].performModifier(mod_valueToCompactEvenHexPrefixed);
        }
        for (auto const& el : tmpD.getSubObjects())
            m_accounts[FH20(el.getKey())] = spAccountBase(new AccountIncomplete(el));
    }
    else
    {
        for (auto const& el : _data.getSubObjects())
            m_accounts[FH20(el.getKey())] = spAccountBase(new AccountIncomplete(el));
    }
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
        // We always assume that StateIncomplete is made of AccountIncomplete, but still
        AccountIncomplete& acc = dynamic_cast<AccountIncomplete&>(el.second.getContent());
        if (el.first == _coinbase && acc.hasBalance())
            acc.setBalance(acc.balance() + _reward);
    }
}

}  // namespace teststruct
}  // namespace test
