#pragma once
#include <set>
#include <string>
#include "../object.h"
#include "scheme_state.h"
#include "scheme_postSectionElement.h"

namespace test {

    class scheme_postState
    {
        public:

        typedef std::vector<scheme_postSectionElement> postSectionElements;

        scheme_postState(DataObject const& _post)
        {
            ETH_ERROR_REQUIRE_MESSAGE(
                _post.type() == DataType::Object, "State tests post section must be json object!");
            for(auto const& element: _post.getSubObjects())
            {
               test::checkAllowedNetwork(element.getKey());
               ETH_ERROR_REQUIRE_MESSAGE(element.type() == DataType::Array,
                   "State tests post section value at fork results must be json array!");
               postSectionElements results;
               for (auto const& res: element.getSubObjects())
                   results.push_back(scheme_postSectionElement(res));

               m_elements[element.getKey()] = results;
            }
        }
        std::map<std::string, postSectionElements>const& getResults() const { return m_elements; }

        private:
        std::map<std::string, postSectionElements> m_elements;
    };

}

