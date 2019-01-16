#pragma once
#include <dataObject/DataObject.h>
#include <dataObject/DataObjectScheme.h>
#include <string>
#include <vector>

using namespace dataobject;
#define VerificationType DataObjectScheme::ValidationRule::verification
namespace dataobject
{
class DataObjectValidator
{
public:
    DataObjectValidator(DataObjectScheme const& _initScheme) : m_scheme(_initScheme) {}
    bool validatie(DataObject const& _object)
    {
        try
        {
            for (auto const& rule : m_scheme.getRules())
            {
                switch (rule.verificationType)
                {
                case VerificationType::CHECK_EXACT:
                    checkType(_object, rule);
                    checkType(rule.dataEnum, rule);
                    checkExactValue(_object, rule.dataEnum);
                    return true;
                    break;
                case VerificationType::CHECK_TYPE:
                    checkType(_object, rule);
                    return true;
                    break;
                default:
                    break;
                }
            }
            return false;
        }
        catch (DataObjectException const& _ex)
        {
            m_lastError = _ex.what();
            return false;
        }
    }
    std::string const& getError() { return m_lastError; }

private:
    void checkType(DataObject const& _object, DataObjectScheme::ValidationRule const& _rule)
    {
        if (_object.type() != _rule.dataTypeExpected)
            throw DataObjectException()
                << string("Validation scheme expected another data type! ") + " Expected: '" +
                       DataObject::dataTypeAsString(_rule.dataTypeExpected) + "'" + ", but got: '" +
                       DataObject::dataTypeAsString(_object.type()) + "'";
    }
    void checkExactValue(DataObject const& _object, DataObject const& _enum)
    {
        // assume _object and _enum has same datatype (checked above)
        switch (_object.type())
        {
        case DataType::String:
            if (_object.asString() != _enum.asString())
                throw ExpectedButGot(_enum.asString(), _object.asString());
            break;
        case DataType::Integer:
            if (_object.asInt() != _enum.asInt())
                throw ExpectedButGot(_enum.asInt(), _object.asInt());
            break;
        case DataType::Bool:
            if (_object.asBool() != _enum.asBool())
                throw ExpectedButGot(_enum.asBool(), _object.asBool());
            break;
        case DataType::Array:
            for (auto const& element : _enum.getSubObjects())
            {
                bool found = false;
                for (auto const& element2 : _object.getSubObjects())
                {
                    if (element == element2)
                        found = true;
                }
                if (!found)
                    throw ExpectedButGot("Array elelemnt '" + element.asJson() + "'",
                        " no elements like this in " + _object.asJson(), false);
            }
            break;
        default:
            throw DataObjectException()
                << "Unhandled datatype in checkExactValue in DataObjectValidator!";
            break;
        }
    }
    DataObjectScheme m_scheme;
    std::string m_lastError;
};
}
