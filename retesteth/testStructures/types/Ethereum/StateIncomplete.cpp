#include "StateIncomplete.h"
#include "AccountIncomplete.h"
#include <retesteth/EthChecks.h>
#include <retesteth/testStructures/Common.h>

namespace test
{
namespace teststruct
{
StateIncomplete::StateIncomplete(DataObject const& _data, DataRequier _req)
{
    try
    {
        // The difference to Common.h::ConvertDecStateToHex is that here we don't compile code
        if (_req == DataRequier::ALLOWDEC)
        {
            // Convertion is here so not to repeat convertion in State and Blockchain tests
            // Convert Expect Section IncompletePostState fields to hex, account keys add `0x` prefix
            // Code field add `0x` prefix, storage key:value add `0x` prefix, coverted to hex
            DataObject tmpD = _data;
            for (auto& acc : tmpD.getSubObjectsUnsafe())
            {
                string const& key = acc.getKey();
                if ((key.size() > 2 && (key[0] != '0' || key[1] != 'x')))
                    acc.setKey("0x" + acc.getKey());
                if (acc.count("balance"))
                    acc["balance"].performModifier(mod_valueToCompactEvenHexPrefixed);
                if (acc.count("nonce"))
                    acc["nonce"].performModifier(mod_valueToCompactEvenHexPrefixed);
                if (acc.count("code") && acc.atKey("code").asString().empty())
                    acc["code"] = "0x" + acc.atKey("code").asString();
                if (acc.count("storage"))
                    for (auto& rec : acc["storage"].getSubObjectsUnsafe())
                    {
                        rec.performModifier(mod_keyToCompactEvenHexPrefixed);
                        rec.performModifier(mod_valueToCompactEvenHexPrefixed);
                    }
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
    catch (std::exception const& _ex)
    {
        throw UpwardsException(string("StateIncomplete parse error: ") + _ex.what() + _data.asJson());
    }
}

const DataObject StateIncomplete::asDataObject(ExportOrder) const
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
