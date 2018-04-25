#pragma once
#include "postSectionElement.h"
#include <retesteth/DataObject.h>

namespace test {

    class postState
    {
        public:
        typedef std::vector<postSectionElement> postSectionElements;
        postState(DataObject const& _post)
        {
            ETH_CHECK_MESSAGE(_post.type() == DataType::Object, "State tests post section must be json object!");
            for(auto const& element: _post.getSubObjects())
            {
               test::checkAllowedNetwork(element.getKey());
               ETH_CHECK_MESSAGE(element.type() == DataType::Array, "State tests post section value at fork results must be json array!");
               postSectionElements results;
               for (auto const& res: element.getSubObjects())
                   results.push_back(postSectionElement(res));

               m_elements[element.getKey()] = results;
            }
        }
        std::map<std::string, postSectionElements>const& getResults() const { return m_elements; }

        private:
        std::map<std::string, postSectionElements> m_elements;
    };

}

