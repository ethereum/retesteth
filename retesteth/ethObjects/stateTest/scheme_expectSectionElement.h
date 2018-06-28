#pragma once
#include "../object.h"
#include "scheme_expectState.h"
#include <retesteth/TestHelper.h>

namespace  test {
    class scheme_expectSectionElement: public object
    {
        public:
        scheme_expectSectionElement(DataObject const& _expect):
            object(_expect),
            m_filedChecker(_expect),
            m_expectState(_expect.at("result"))
        {
            parseJsonIntValueIntoSet(_expect.at("indexes").at("data"), m_dataIndexes);
            parseJsonIntValueIntoSet(_expect.at("indexes").at("gas"), m_gasIndexes);
            parseJsonIntValueIntoSet(_expect.at("indexes").at("value"), m_valueIndexes);

            // get allowed networks for this expect section
            parseJsonStrValueIntoSet(_expect.at("network"), m_networks);
            m_networks = translateNetworks(m_networks);
        }
        std::set<std::string> const& getNetworks() const { return m_networks; }
        scheme_expectState const& getExpectState() const { return m_expectState; }
        scheme_expectState& getExpectStateUnsafe() { return m_expectState; }
        void correctMiningReward(std::string const& _net, std::string const& _coinbaseAddress)
        {
            u256 balance = 5000000000000000000;
            if (_net == "Byzantium" || _net == "Constantinople")
                balance = 3000000000000000000;
            if (getExpectState().hasAccount(_coinbaseAddress))
            {
                u256 origBalance = u256(getExpectState().getBalance(_coinbaseAddress));
                getExpectStateUnsafe().setBalance(_coinbaseAddress, origBalance + balance);
            }
        }
        bool checkIndexes(int _d, int _g, int _v) const
        {
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
                requireJsonFields(_expect, "expect", {
                    {"indexes", {DataType::Object} },
                    {"network", {DataType::Array, DataType::String} },
                    {"result", {DataType::Object} }
                });
                requireJsonFields(_expect.at("indexes"), "indexes", {
                    {"data", {DataType::Array, DataType::Integer} },
                    {"gas", {DataType::Array, DataType::Integer} },
                    {"value", {DataType::Array, DataType::Integer} }
                });
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
