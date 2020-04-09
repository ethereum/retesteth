#pragma once
#include <dataObject/DataObject.h>
#include <retesteth/EthChecks.h>
#include <retesteth/TestOutputHelper.h>
#include <boost/test/unit_test.hpp>

using namespace dataobject;
namespace test {
class object
{
public:
    object(DataObject const& _json) : m_data(_json) {}
    DataObject const& getData() const { return m_data; }

    enum DigitsType
    {
        Decimal,
        Hex,
        UnEvenHex,
        HexPrefixed,
        UnEvenHexPrefixed,
        String
    };

    static string DigitsTypeToString(DigitsType _t)
    {
        switch (_t)
        {
        case DigitsType::Decimal:
            return "Decimal";
        case DigitsType::Hex:
            return "Hex";
        case DigitsType::UnEvenHex:
            return "UnEvenHex";
        case DigitsType::HexPrefixed:
            return "HexPrefixed";
        case DigitsType::UnEvenHexPrefixed:
            return "UnEvenHexPrefixed";
        case DigitsType::String:
            return "String";
        default:
            return "Unrecognized type";
        }
    }

    static std::string emptyString;
    static DigitsType stringIntegerType(std::string const& _string);
    static std::string makeHexAddress(std::string const& _address);
    static DataObject prepareGenesisParams(std::string const& _network);

protected:
    static void makeKeyHex(DataObject& _key);
    static void makeAllFieldsHex(DataObject& _data);
    DataObject m_data;

private:
    static std::string convertStringToHexPrefixed(string const& _input, short _minimumBytes);
};

/// check the presents of fields in a DataObject with a validation map
typedef std::set<DataType> possibleType;
void requireJsonFields(DataObject const& _o, std::string const& _section,
    std::map<std::string, possibleType> const& _validationMap, bool _fail = false);

enum jsonField
{
    Required,
    Optional
};
using jsonTypeSet = std::set<DataType>;
using jsonType = std::pair<jsonTypeSet, jsonField>;
//! Check the json object with validation map that reuires certain field of certain type to be
//! present in json
/*!
  \param _o a json object to check
  \param _configName a string with json object name. Will apper in error message.
  \param _validationMap a map with json objects that would be checked. "objName" ->
  {js::str_type, jsonField::Required}
*/
void requireJsonFields(DataObject const& _o, std::string const& _configName,
    std::map<std::string, jsonType> const& _validationMap);

/// Convert hex/dec string to int
bool isHexDigitsType(test::object::DigitsType _dtype);
int hexOrDecStringToInt(string const& _str);
}

