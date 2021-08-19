#pragma once
#include <retesteth/compiler/Compiler.h>
#include <retesteth/dataObject/DataObject.h>
#include <retesteth/testStructures/types/Ethereum/BlockHeaderReader.h>
#include <retesteth/testStructures/types/Ethereum/TransactionReader.h>
#include <map>
using namespace dataobject;
using namespace test::compiler;

namespace test
{
namespace teststruct
{
// DataObject converters. Mostly Dec -> Hex, json data convertation
void mod_removeLeadingZerosFromHexValues(DataObject&);
void mod_removeLeadingZerosFromHexValuesEVEN(DataObject&);
void mod_removeLeadingZerosFromHexKeysEVEN(DataObject&);
void mod_removeComments(DataObject& _obj);
void mod_valuesToLowerCase(DataObject&);
void mod_valueToCompactEvenHexPrefixed(DataObject&);
void mod_keyToCompactEvenHexPrefixed(DataObject&);
long long int hexOrDecStringToInt(string const& _str);

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

static VALUE c_maxNonce(DataObject("0xffffffffffffffff"));

// Check the json object with validation map that reuires certain field of certain type to be present in json
// _o a json object to check
// _configName a string with json object name. Will apper in error message.
// _validationMap a map with json objects that would be checked. "objName" -> {js::str_type, jsonField::Required}
void requireJsonFields(
    DataObject const& _o, std::string const& _configName, std::map<std::string, jsonType> const& _validationMap);

// Compile LLL in code, solidity in code
// Convert dec fields to hex, add 0x prefix to accounts and storage keys
void convertDecStateToHex(spDataObject& _data, solContracts const& _preSolidity = solContracts());

// Convert dec fields to hex, add 0x prefix to accounts and storage keys
spDataObject convertDecBlockheaderIncompleteToHex(DataObject const& _data);

// Make a nice compare result string
string compareBlockHeaders(DataObject const& _blockA, DataObject const& _blockB, string& _whatField);

// Read expect Exceptions
void readExpectExceptions(DataObject const& _data, std::map<FORK, string>& _out);

// Marco move subpointer from key _key in _dataobject
#define MOVE(_dataobject, _key) dataobject::move((*_dataobject).atKeyPointerUnsafe(_key))

}  // namespace teststruct
}  // namespace test
