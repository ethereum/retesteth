#pragma once
#include "../object.h"
#include "retesteth/Options.h"
#include "scheme_postSectionElement.h"
#include "scheme_state.h"
#include <set>
#include <string>

namespace test {
class scheme_postState
{
public:
    typedef std::vector<scheme_postSectionElement> postSectionElements;

    scheme_postState(DataObject const& _post)
    {
        ETH_ERROR_REQUIRE_MESSAGE(
            _post.type() == DataType::Object, "State tests post section must be json object!");
        for (auto const& element : _post.getSubObjects())
        {
            ClientConfig const& cfg = Options::get().getDynamicOptions().getCurrentConfig();
            test::checkAllowedNetwork(element.getKey(), cfg.getNetworks());
            ETH_ERROR_REQUIRE_MESSAGE(element.type() == DataType::Array,
                "State tests post section value at fork results must be json array!");
            postSectionElements results;
            for (auto const& res : element.getSubObjects())
            {
                scheme_postSectionElement postElement(res);
                for (auto const& regPostElement : results)
                {
                    if (postElement.compareIndexesTo(regPostElement))
                        ETH_ERROR_MESSAGE(
                            "State test post section has expect field with same indexes!" +
                            postElement.getData().asJson());
                }
                results.push_back(postElement);
            }
            m_elements[element.getKey()] = results;
        }
    }
    std::map<std::string, postSectionElements> const& getResults() const { return m_elements; }

private:
    std::map<std::string, postSectionElements> m_elements;
    };

}

