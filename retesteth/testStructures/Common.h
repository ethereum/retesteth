#pragma once
#include <retesteth/compiler/Compiler.h>
#include <libdataobj/DataObject.h>
#include <retesteth/testStructures/types/Ethereum/Blocks/BlockHeaderReader.h>
#include <retesteth/testStructures/types/Ethereum/Transactions/TransactionReader.h>
#include <map>

namespace test::teststruct
{

// DataObject converters. Mostly Dec -> Hex, json data convertation
void mod_removeBigIntHint(DataObject&);
void mod_removeLeadingZerosFromHexValues(DataObject&);
void mod_removeLeadingZerosFromHexValueEVEN(DataObject&);
void mod_removeLeadingZerosFromHexKeyEVEN(DataObject&);
void mod_removeComments(DataObject& _obj);
void mod_valueToLowerCase(DataObject&);
void mod_keyToLowerCase(DataObject&);
void mod_valueToCompactEvenHexPrefixed(DataObject&);
void mod_keyToCompactEvenHexPrefixed(DataObject&);
void mod_valueInsertZeroXPrefix(DataObject&);
void mod_valueToFH32(DataObject&);
void mod_sortKeys(DataObject&);
void mod_changeValueAnyToBigint00(DataObject&);
bool src_findBigInt(DataObject const& el);

long long int hexOrDecStringToInt(std::string const& _str);

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

#define REQUIRE_JSONFIELDS(_data, _name, ...)  \
 static std::map<std::string, jsonType> requireJsonFieldsMap = __VA_ARGS__; \
 requireJsonFields(_data, _name, requireJsonFieldsMap)

// Compile LLL in code, solidity in code
// Convert dec fields to hex, add 0x prefix to accounts and storage keys
enum class StateToHex
{
    COMPILECODE,
    NOCOMPILECODE
};
void convertDecStateToHex(spDataObject& _data,
    test::compiler::solContracts const& _preSolidity = test::compiler::solContracts(),
    StateToHex _compileCode = StateToHex::COMPILECODE);

void convertDecTransactionToHex(spDataObject& _data);

// Convert dec fields to hex, add 0x prefix to accounts and storage keys
spDataObject convertDecBlockheaderIncompleteToHex(DataObject const& _data);

// Make a nice compare result string
std::string compareBlockHeaders(DataObject const& _blockA, DataObject const& _blockB, std::string& _whatField);

// Read expect Exceptions
void readExpectExceptions(DataObject const& _data, std::map<FORK, std::string>& _out);

// Marco move subpointer from key _key in _dataobject
#define MOVE(_dataobject, _key) move((*_dataobject).atKeyPointerUnsafe(_key))

// Convert Secret Key to Public eth key
spFH20 convertSecretToPublic(spFH32 const& _secret);
spFH20 convertSecretToPublic(VALUE const& _secret);

}  // namespace teststruct
