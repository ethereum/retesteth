#pragma once
#include "../object.h"
#include "scheme_expectSectionElement.h"
#include <retesteth/TestHelper.h>

namespace test
{
class scheme_expectSection : public object
{
public:
    scheme_expectSection(DataObject const& _expect) : object(_expect)
    {
        ETH_ERROR_REQUIRE_MESSAGE(
            _expect.type() == DataType::Array, "Expect section object must be an array!");
        for (auto const& expect : _expect.getSubObjects())
        {
            scheme_expectSectionElement expElement(expect);
            m_expectSections.push_back(expElement);
            for (auto const& net : expElement.getNetworks())
                m_allNetworksDeclaredInExpectSection.emplace(net);
        }

        if (!Options::get().singleTestNet.empty())
        {
            if (m_allNetworksDeclaredInExpectSection.count(Options::get().singleTestNet))
            {
                m_allNetworksDeclaredInExpectSection.clear();
                m_allNetworksDeclaredInExpectSection.emplace(Options::get().singleTestNet);
            }
        }
    }
    std::set<std::string> const& getAllNetworksFromExpectSection() const
    {
        return m_allNetworksDeclaredInExpectSection;
    }
    std::vector<scheme_expectSectionElement> const& getExpectSections() const
    {
        return m_expectSections;
    }

private:
    std::set<std::string> m_allNetworksDeclaredInExpectSection;
    std::vector<scheme_expectSectionElement> m_expectSections;
};
}
