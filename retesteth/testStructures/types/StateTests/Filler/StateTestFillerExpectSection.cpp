#include "StateTestFillerExpectSection.h"
#include <TestHelper.h>
#include <retesteth/ethObjects/object.h>

#include <retesteth/Options.h>
namespace test
{
namespace teststruct
{
StateTestFillerExpectSection::StateTestFillerExpectSection(DataObject const& _data)
{
    try
    {
        parseJsonIntValueIntoSet(_data.atKey("indexes").atKey("data"), m_dataInd);
        parseJsonIntValueIntoSet(_data.atKey("indexes").atKey("gas"), m_gasInd);
        parseJsonIntValueIntoSet(_data.atKey("indexes").atKey("value"), m_valInd);

        ETH_ERROR_REQUIRE_MESSAGE(m_dataInd.size() > 0, "Expect section `indexes::data` is empty!");
        ETH_ERROR_REQUIRE_MESSAGE(m_gasInd.size() > 0, "Expect section `indexes::gas` is empty!");
        ETH_ERROR_REQUIRE_MESSAGE(m_valInd.size() > 0, "Expect section `indexes::value` is empty!");

        // get allowed networks for this expect section
        std::set<string> forks;
        parseJsonStrValueIntoSet(_data.atKey("network"), forks);
        ETH_ERROR_REQUIRE_MESSAGE(forks.size() > 0, "Expect section `network` is empty!");

        // Parse >=Frontier into  Frontier, Homestead, ... Constantinople according to current config
        ClientConfig const& cfg = Options::get().getDynamicOptions().getCurrentConfig();
        m_forks = cfg.translateNetworks(forks);

        // Convert Expect Section IncompletePostState fields to hex, account keys add `0x` prefix
        DataObject tmpD = _data.atKey("result");
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

        m_result = GCP_SPointer<StateIncomplete>(new StateIncomplete(tmpD));
    }
    catch (std::exception const& _ex)
    {
        throw BaseEthException(string("StateTestFillerExpectSection parse error: ") + _ex.what());
    }
}

bool StateTestFillerExpectSection::checkIndexes(size_t _dInd, size_t _gInd, size_t _vInd) const
{
    if ((m_dataInd.count(_dInd) || m_dataInd.count(-1)) &&
        (m_gasInd.count(_gInd) || m_gasInd.count(-1)) &&
        (m_valInd.count(_vInd) || m_valInd.count(-1)))
        return true;
    return false;
}

void StateTestFillerExpectSection::correctMiningReward(FH20 const& _coinbase, VALUE const& _reward)
{
    m_result.getContent().correctMiningReward(_coinbase, _reward);
}

}  // namespace teststruct
}  // namespace test
