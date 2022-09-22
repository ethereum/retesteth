#pragma once
#include <libdataobj/DataObject.h>
#include <libdataobj/Exception.h>
#include <string>
#include <vector>

namespace dataobject
{
/*
 * {
  "type": "string",
  "enum": ["0x0"]
}
{
   "type": "string"
}
*/


/// Validation scheme datastructure
class DataObjectScheme
{
public:
    // Initialize scheme from description
    DataObjectScheme(DataObject const& _init) { readRules(_init); }

    class ValidationRule
    {
    public:
        enum class verification
        {
            CHECK_EXACT,
            CHECK_TYPE
        };
        DataType dataTypeExpected;
        verification verificationType;
        spDataObject dataEnum;
    };

    std::vector<ValidationRule> const& getRules() { return rulesVector; }

private:
    void readRules(DataObject const& _obj)
    {
        if (_obj.type() != DataType::Object)
            throw DataObjectException()
                << "Validation scheme object is expected to be of object type(1)!";
        if (!_obj.count("type"))
            throw DataObjectException() << "Required field 'type' not found in validation scheme!";
        if (_obj.atKey("type").type() != DataType::String)
            throw DataObjectException()
                << "Validation scheme 'type' field must be a string object!";

        ValidationRule rule;
        // Read string field (data type) into DataObject type
        rule.verificationType = ValidationRule::verification::CHECK_TYPE;
        if (_obj.atKey("type").asString() == "string")
            rule.dataTypeExpected = DataType::String;
        else if (_obj.atKey("type").asString() == "integer")
            rule.dataTypeExpected = DataType::Integer;
        else if (_obj.atKey("type").asString() == "bool")
            rule.dataTypeExpected = DataType::Bool;
        else if (_obj.atKey("type").asString() == "object")
            rule.dataTypeExpected = DataType::Object;
        else if (_obj.atKey("type").asString() == "array")
            rule.dataTypeExpected = DataType::Array;

        // validation scheme has enum field
        if (_obj.count("enum"))
        {
            if (_obj.atKey("enum").type() != DataType::Array)
                throw DataObjectException() << "Validation scheme 'enum' field must be an array!";
            rule.verificationType = ValidationRule::verification::CHECK_EXACT;
            rule.dataEnum = _obj.atKey("enum").getSubObjects().at(0);  // enum with only 1 element!!!
        }

        rulesVector.push_back(rule);
    }

private:
    std::vector<ValidationRule> rulesVector;
};
}
