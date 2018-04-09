#pragma once

#include <set>
#include <string>

#include <retesteth/DataObject.h>
#include <retesteth/TestHelper.h>
#include <retesteth/ethObjects/state.h>
#include <retesteth/ethObjects/object.h>

#include <retesteth/TestHelper.h>

namespace test {
    class expectSectionElement: public object
	{
        public:
        expectSectionElement(DataObject const& _expect):
            object(_expect)
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

            parseJsonIntValueIntoSet(_expect.at("indexes").at("data"), m_dataIndexes);
            parseJsonIntValueIntoSet(_expect.at("indexes").at("gas"), m_gasIndexes);
            parseJsonIntValueIntoSet(_expect.at("indexes").at("value"), m_valueIndexes);

            // get allowed networks for this expect section
            parseJsonStrValueIntoSet(_expect.at("network"), m_networks);
            m_networks = translateNetworks(m_networks);
        }

        std::set<std::string> const& getNetworks() const { return m_networks; }
        bool checkIndexes(int _d, int _g, int _v) const
        {
            if ((m_dataIndexes.count(_d) || m_dataIndexes.count(-1)) &&
                (m_gasIndexes.count(_g) || m_gasIndexes.count(-1)) &&
                (m_valueIndexes.count(_v) || m_valueIndexes.count(-1)))
                return true;
            return false;
        }

        private:
        std::set<int> m_dataIndexes;
        std::set<int> m_gasIndexes;
        std::set<int> m_valueIndexes;
        std::set<std::string> m_networks;
	};
}

