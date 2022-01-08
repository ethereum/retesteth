#include "Info.h"
#include <retesteth/testStructures/Common.h>

namespace test
{
namespace teststruct
{
Info::Info(DataObject const& _data)
{
    if (_data.count("filledwith"))
    {
        REQUIRE_JSONFIELDS(_data, "InfoLegacy " + _data.getKey(),
            {{"comment", {{DataType::String}, jsonField::Required}},
                {"filledwith", {{DataType::String}, jsonField::Required}},
                {"lllcversion", {{DataType::String}, jsonField::Required}},
                {"source", {{DataType::String}, jsonField::Required}},
                {"sourceHash", {{DataType::String}, jsonField::Optional}},
                {"source", {{DataType::String}, jsonField::Required}},
                {"sourceHash", {{DataType::String}, jsonField::Required}}});
    }
    else
    {
        REQUIRE_JSONFIELDS(_data, "Info " + _data.getKey(),
            {{"comment", {{DataType::String}, jsonField::Required}},
            {"filling-rpc-server", {{DataType::String}, jsonField::Required}},
            {"filling-tool-version", {{DataType::String}, jsonField::Required}},
            {"lllcversion", {{DataType::String}, jsonField::Required}},
            {"solidity", {{DataType::String}, jsonField::Required}},
            {"generatedTestHash", {{DataType::String}, jsonField::Required}},
            {"source", {{DataType::String}, jsonField::Required}},
            {"sourceHash", {{DataType::String}, jsonField::Required}},
            {"labels", {{DataType::Object}, jsonField::Optional}}});
    }

    if (_data.count("labels"))
    {
        for (auto const& el : _data.atKey("labels").getSubObjects())
            m_labels.emplace(el->getKey(), el->asString());
    }
}

}  // namespace teststruct
}  // namespace test
