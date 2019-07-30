#pragma once
#include "../object.h"
using namespace dev;

namespace test
{
// This structure verifies remote state info constructed from multiple requests
class scheme_remoteState : public object
{
public:
    scheme_remoteState() : object(DataObject()) {}
    scheme_remoteState(DataObject const& _remoteState) : object(_remoteState)
    {
        requireJsonFields(_remoteState, "remoteStateConstruct",
            {{"postHash", {{DataType::String}, jsonField::Required}},
                {"postState", {{DataType::String, DataType::Object}, jsonField::Optional}},
                {"rawBlockData", {{DataType::Object}, jsonField::Required}}});
    }

    DataObject getRawBlockData() const { return m_data.atKey("rawBlockData"); }
    string getPostHash() const { return m_data.atKey("postHash").asString(); }
    bool hasPostState() const
    {
        return m_data.count("postState") && m_data.atKey("postState").getSubObjects().size() > 0;
    }
    DataObject getPostState() const
    {
        if (m_data.count("postState"))
            return m_data.atKey("postState");
        else
            return DataObject(DataType::Object);
    }
};
}
