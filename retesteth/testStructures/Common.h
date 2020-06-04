#pragma once
#include "basetypes.h"
#include "configs/SealEngine.h"
#include "types/Ethereum/State.h"
#include "types/StateTests/Base/StateTestEnvBase.h"
#include <retesteth/dataObject/DataObject.h>

using namespace dataobject;

namespace test
{
namespace teststruct
{
// DataObject converters. Mostly Dec -> Hex, json data convertation
void mod_removeLeadingZerosFromHexValues(DataObject&);
void mod_removeLeadingZerosFromHexValuesEVEN(DataObject&);
void mod_removeComments(DataObject& _obj);
void mod_valuesToLowerCase(DataObject&);
void mod_valueToCompactEvenHexPrefixed(DataObject&);
void mod_keyToCompactEvenHexPrefixed(DataObject&);
long int hexOrDecStringToInt(string const& _str);


// Prepare chain params rpc request
DataObject prepareChainParams(FORK const&, SealEngine, State const&, StateTestEnvBase const&);

// Check the presents of fields in a DataObject with a validation map
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

// Check the json object with validation map that reuires certain field of certain type to be present in json
// _o a json object to check
// _configName a string with json object name. Will apper in error message.
// _validationMap a map with json objects that would be checked. "objName" -> {js::str_type, jsonField::Required}
void requireJsonFields(
    DataObject const& _o, std::string const& _configName, std::map<std::string, jsonType> const& _validationMap);

// Compile LLL in code
// Convert dec fields to hex, add 0x prefix to accounts and storage keys
DataObject convertDecStateToHex(DataObject const& _data);

// Convert dec fields to hex, add 0x prefix to accounts and storage keys
DataObject convertDecBlockheaderIncompleteToHex(DataObject const& _data);

}  // namespace teststruct
}  // namespace test
