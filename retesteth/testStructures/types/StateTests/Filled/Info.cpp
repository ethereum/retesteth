#include "Info.h"
#include <retesteth/EthChecks.h>
#include <retesteth/testStructures/Common.h>
using namespace std;
using namespace test::teststruct;

namespace
{
    void requireInfoFields(DataObject const& _data)
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
            bool isPyspecs = _data.count("filling-transition-tool");
            REQUIRE_JSONFIELDS(_data, "Info " + _data.getKey(),
                {{"comment", {{DataType::String}, jsonField::Required}},
                    {"filling-transition-tool", {{DataType::String}, isPyspecs ? jsonField::Required : jsonField::Optional}},
                    {"reference-spec", {{DataType::String}, jsonField::Optional}},
                    {"reference-spec-version", {{DataType::String}, jsonField::Optional}},
                    {"fixture-format", {{DataType::String}, jsonField::Optional}},
                    {"filling-rpc-server", {{DataType::String}, isPyspecs ? jsonField::Optional : jsonField::Required}},
                    {"filling-tool-version", {{DataType::String}, isPyspecs ? jsonField::Optional : jsonField::Required}},
                    {"hash", {{DataType::String}, isPyspecs ? jsonField::Required : jsonField::Optional}},
                    {"lllcversion", {{DataType::String}, isPyspecs ? jsonField::Optional : jsonField::Required}},
                    {"solidity", {{DataType::String}, isPyspecs ? jsonField::Optional : jsonField::Required}},
                    {"generatedTestHash", {{DataType::String}, isPyspecs ? jsonField::Optional : jsonField::Required}},
                    {"source", {{DataType::String}, isPyspecs ? jsonField::Optional : jsonField::Required}},
                    {"sourceHash", {{DataType::String}, isPyspecs ? jsonField::Optional : jsonField::Required}},
                    {"labels", {{DataType::Object}, jsonField::Optional}}});
        }
    }
}

namespace test::teststruct
{

Info::Info(DataObject const& _data)
{
    try
    {
        requireInfoFields(_data);
    }
    catch (std::exception const& _ex)
    {
        ETH_DC_MESSAGE(test::debug::DC::STATS2, string() + "Info section verification failed: " + _ex.what());
    }

    if (_data.count("labels"))
    {
        for (auto const& el : _data.atKey("labels").getSubObjects())
            m_labels.emplace(el->getKey(), el->asString());
    }
}

}  // namespace teststruct
