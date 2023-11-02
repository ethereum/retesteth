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
            REQUIRE_JSONFIELDS(_data, "Info " + _data.getKey(),
                {{"comment", {{DataType::String}, jsonField::Required}},
                    {"filling-transition-tool", {{DataType::String}, jsonField::Optional}},
                    {"reference-spec", {{DataType::String}, jsonField::Optional}},
                    {"reference-spec-version", {{DataType::String}, jsonField::Optional}},
                    {"filling-rpc-server", {{DataType::String}, jsonField::Required}},
                    {"filling-tool-version", {{DataType::String}, jsonField::Required}},
                    {"lllcversion", {{DataType::String}, jsonField::Required}},
                    {"solidity", {{DataType::String}, jsonField::Required}},
                    {"generatedTestHash", {{DataType::String}, jsonField::Required}},
                    {"source", {{DataType::String}, jsonField::Required}},
                    {"sourceHash", {{DataType::String}, jsonField::Required}},
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
        ETH_WARNING(string() + "Info section verification failed: " + _ex.what());
    }

    if (_data.count("labels"))
    {
        for (auto const& el : _data.atKey("labels").getSubObjects())
            m_labels.emplace(el->getKey(), el->asString());
    }
}

}  // namespace teststruct
