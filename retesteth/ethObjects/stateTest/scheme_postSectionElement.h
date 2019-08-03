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
            requireJsonFields(_expect.atKey("indexes"), "indexes",
                {{"data", {DataType::Array, DataType::Integer}},
                    {"gas", {DataType::Array, DataType::Integer}},
                    {"value", {DataType::Array, DataType::Integer}}});

            parseJsonIntValueIntoSet(_expect.atKey("indexes").atKey("data"), m_dataIndexes);
            parseJsonIntValueIntoSet(_expect.atKey("indexes").atKey("gas"), m_gasIndexes);
            parseJsonIntValueIntoSet(_expect.atKey("indexes").atKey("value"), m_valueIndexes);

            // currently post section support only single index check
            ETH_ERROR_REQUIRE_MESSAGE(
                m_dataIndexes.size() == 1, "Post section support only single data index!");
            ETH_ERROR_REQUIRE_MESSAGE(
                m_gasIndexes.size() == 1, "Post section support only single gas index!");
            ETH_ERROR_REQUIRE_MESSAGE(
                m_valueIndexes.size() == 1, "Post section support only single value index!");
        }

        bool compareIndexesTo(scheme_postSectionElement const& _anotheElement)
        {
            int ourData = *m_dataIndexes.begin();
            int ourGas = *m_gasIndexes.begin();
            int ourVal = *m_valueIndexes.begin();
            return _anotheElement.checkIndexes(ourData, ourGas, ourVal);
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
