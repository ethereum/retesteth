#pragma once
#include "../object.h"
#include <retesteth/TestHelper.h>

namespace  test {
    class scheme_postSectionElement: public object
    {
        public:
        scheme_postSectionElement(DataObject const& _expect):
            object(_expect)
        {
            requireJsonFields(_expect, "post", {
                {"hash", {DataType::String} },
                {"logs", {DataType::String} },
                {"indexes", {DataType::Object} }
            });
            requireJsonFields(_expect.at("indexes"), "indexes", {
                {"data", {DataType::Array, DataType::Integer} },
                {"gas", {DataType::Array, DataType::Integer} },
                {"value", {DataType::Array, DataType::Integer} }
            });

            parseJsonIntValueIntoSet(_expect.at("indexes").at("data"), m_dataIndexes);
            parseJsonIntValueIntoSet(_expect.at("indexes").at("gas"), m_gasIndexes);
            parseJsonIntValueIntoSet(_expect.at("indexes").at("value"), m_valueIndexes);

            // currently post section support only single index check
            ETH_FAIL_REQUIRE(m_dataIndexes.size() == 1);
            ETH_FAIL_REQUIRE(m_gasIndexes.size() == 1);
            ETH_FAIL_REQUIRE(m_valueIndexes.size() == 1);
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
        std::set<int> m_dataIndexes;
        std::set<int> m_gasIndexes;
        std::set<int> m_valueIndexes;
    };
}
