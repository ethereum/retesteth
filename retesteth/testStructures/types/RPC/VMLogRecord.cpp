#include <testStructures/Common.h>
#include <retesteth/EthChecks.h>
#include "VMLogRecord.h"
using namespace test;
using namespace test::teststruct;
using namespace dataobject;
using namespace std;

VMLogRecord::VMLogRecord(DataObject const& _obj)
{
    try
    {
        if (_obj.getSubObjects().size() == 2)
        {
            REQUIRE_JSONFIELDS(_obj, "VMLogRecord " + _obj.getKey(),
                {
                    {"output", {{DataType::String}, jsonField::Required}},
                    {"gasUsed", {{DataType::String}, jsonField::Required}}});
            isShort = true;
        }
        else if (_obj.getSubObjects().size() == 3)
        {
            REQUIRE_JSONFIELDS(_obj, "VMLogRecord " + _obj.getKey(),
                {
                    {"output", {{DataType::String}, jsonField::Required}},
                    {"gasUsed", {{DataType::String}, jsonField::Required}},
                    {"error", {{DataType::String}, jsonField::Required}}});
            isShort = true;
        }
        else if (_obj.getSubObjects().size() == 4)
        {
            REQUIRE_JSONFIELDS(_obj, "VMLogRecord " + _obj.getKey(),
                {
                    {"output", {{DataType::String}, jsonField::Required}},
                    {"gasUsed", {{DataType::String}, jsonField::Required}},
                    {"time", {{DataType::Integer}, jsonField::Required}},
                    {"error", {{DataType::String}, jsonField::Required}}});
            isShort = true;
        }
        else
        {
            REQUIRE_JSONFIELDS(_obj, "VMLogRecord " + _obj.getKey(),
                {{"pc", {{DataType::Integer}, jsonField::Required}},
                    {"op", {{DataType::Integer}, jsonField::Required}},
                    {"gas", {{DataType::String}, jsonField::Required}},
                    {"gasCost", {{DataType::String}, jsonField::Required}},
                    {"memory", {{DataType::String, DataType::Array}, jsonField::Optional}},
                    {"memSize", {{DataType::Integer}, jsonField::Required}},
                    {"stack", {{DataType::Array}, jsonField::Required}},
                    {"depth", {{DataType::Integer}, jsonField::Required}},
                    {"refund", {{DataType::Integer}, jsonField::Required}},
                    {"opName", {{DataType::String}, jsonField::Required}},
                    {"returnData", {{DataType::String}, jsonField::Optional}},
                    {"error", {{DataType::String}, jsonField::Optional}}});
        }

        if (_obj.count("error"))
            error = _obj.atKey("error").asString();

        if (_obj.getSubObjects().size() > 4)
        {
            pc = _obj.atKey("pc").asInt();
            op = _obj.atKey("op").asInt();
            gas = spVALUE(new VALUE(_obj.atKey("gas")));
            gasCost = spVALUE(new VALUE(_obj.atKey("gasCost")));
            memory = spBYTES(new BYTES(DataObject("0x")));
            if (_obj.count("memory"))
            {
                if (_obj.atKey("memory").type() == DataType::String)
                    memory = spBYTES(new BYTES(_obj.atKey("memory")));
                if (_obj.atKey("memory").type() == DataType::Array)
                {
                    for (auto const& el : _obj.atKey("memory").getSubObjects())
                        memory.getContent().asStringUnsafe() += el->asString();
                }
            }

            memSize = _obj.atKey("memSize").asInt();
            for (auto const& el : _obj.atKey("stack").getSubObjects())
                stack.emplace_back(el->asString());
            if (_obj.count("returnData") && !_obj.atKey("returnData").asString().empty())
                returnData = spBYTES(new BYTES(_obj.atKey("returnData")));
            else
                returnData = spBYTES(new BYTES(DataObject("0x")));
            depth = _obj.atKey("depth").asInt();
            refund = _obj.atKey("refund").asInt();
            opName = _obj.atKey("opName").asString();
            error = _obj.count("error") ? _obj.atKey("error").asString() : "";
        }

    }
    catch (std::exception const& _ex)
    {
        throw UpwardsException(string("VMLogRecord parse error: ") + _ex.what());
    }
}
