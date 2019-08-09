#pragma once
#include "../object.h"
#include "scheme_expectState.h"
#include <retesteth/Options.h>
#include <retesteth/TestHelper.h>

namespace test
{
/// Element of expect section array
class scheme_expectSectionElement : public object
{
public:
    scheme_expectSectionElement(DataObject const& _expect)
      : object(_expect), m_filedChecker(_expect), m_expectState(_expect.atKey("result"))
    {
        if (_expect.count("indexes"))
        {
            parseJsonIntValueIntoSet(_expect.atKey("indexes").atKey("data"), m_dataIndexes);
            parseJsonIntValueIntoSet(_expect.atKey("indexes").atKey("gas"), m_gasIndexes);
            parseJsonIntValueIntoSet(_expect.atKey("indexes").atKey("value"), m_valueIndexes);
        }

        // get allowed networks for this expect section
        parseJsonStrValueIntoSet(_expect.atKey("network"), m_networks);
        ClientConfig const& cfg = Options::get().getDynamicOptions().getCurrentConfig();
        m_networks = translateNetworks(m_networks, cfg.getNetworks());
    }
    std::set<std::string> const& getNetworks() const { return m_networks; }
    scheme_expectState const& getExpectState() const { return m_expectState; }
    scheme_expectState& getExpectStateUnsafe() { return m_expectState; }

    /// Correct expect section mining reward when filling the blockchain tests out of state
    /// tests State tests does not count mining rewards, but it is possible to generate a
    /// blockchain test out of it
    void correctMiningReward(std::string const& _net, std::string const& _coinbaseAddress)
    {
        ClientConfig const& cfg = Options::getDynamicOptions().getCurrentConfig();
        ETH_ERROR_REQUIRE_MESSAGE(cfg.getMiningRewardInfo().count(_net), "Network '" + _net
            + "' not found in correct mining info config! Client: " + cfg.getName());
        u256 balance (cfg.getMiningRewardInfo().atKey(_net).asString());
        if (getExpectState().hasBalance(_coinbaseAddress))
        {
            u256 origBalance = u256(getExpectState().getBalance(_coinbaseAddress));
            getExpectStateUnsafe().setBalance(_coinbaseAddress, origBalance + balance);
        }
    }

    bool checkIndexes(int _d, int _g, int _v) const
    {
        ETH_ERROR_REQUIRE_MESSAGE(m_dataIndexes.size() != 0,
            "checkIndexes() must not be called for blockchain expect section!");
        if ((m_dataIndexes.count(_d) || m_dataIndexes.count(-1)) &&
            (m_gasIndexes.count(_g) || m_gasIndexes.count(-1)) &&
            (m_valueIndexes.count(_v) || m_valueIndexes.count(-1)))
            return true;
        return false;
    }

private:
    class fieldChecker
    {
    public:
        fieldChecker(DataObject const& _expect)
        {
            requireJsonFields(_expect, "expect",
                {{"indexes", {{DataType::Object}, jsonField::Optional}},
                    {"network", {{DataType::Array, DataType::String}, jsonField::Required}},
                    {"result", {{DataType::Object}, jsonField::Required}}});

            if (_expect.count("indexes"))
            {
                requireJsonFields(_expect.atKey("indexes"), "indexes",
                    {{"data", {DataType::Array, DataType::Integer}},
                        {"gas", {DataType::Array, DataType::Integer}},
                        {"value", {DataType::Array, DataType::Integer}}});
            }
        }
    };
    std::set<int> m_dataIndexes;
    std::set<int> m_gasIndexes;
    std::set<int> m_valueIndexes;
    std::set<std::string> m_networks;
    fieldChecker m_filedChecker;
    scheme_expectState m_expectState;
};
}
