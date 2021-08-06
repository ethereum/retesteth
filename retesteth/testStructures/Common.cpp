#include "Common.h"
#include "EthChecks.h"
#include "TestHelper.h"
#include "TestOutputHelper.h"
#include <retesteth/Options.h>
#include <retesteth/configs/ClientConfig.h>
#include <mutex>
#include <algorithm>

using namespace test;
using namespace dev;
using namespace test::teststruct;

namespace
{
string toCompactHexPrefixed(string const& _str, size_t _minSize)
{
    string prefix = string();
    DigitsType t = stringIntegerType(_str);
    if (t == DigitsType::Hex || t == DigitsType::UnEvenHex)
        prefix = "0x";
    return dev::toCompactHexPrefixed(u256(prefix + _str), _minSize);
}

bool isHexDigitsType(DigitsType _dtype)
{
    return (_dtype == DigitsType::HexPrefixed || _dtype == DigitsType::UnEvenHexPrefixed);
}

void removeLeadingZeroes(string& _hexStr)
{
    size_t i = 0;
    for (i = 2; i < _hexStr.length() - 1; i++)
    {
        if (_hexStr.at(i) == '0')
            continue;
        else
            break;
    }
    _hexStr = "0x" + _hexStr.substr(i);
}

}  // namespace


namespace test
{
namespace teststruct
{
// DataObject modifiers. Convert all json values to lower case
void mod_valuesToLowerCase(DataObject& _obj)
{
    if (_obj.type() == DataType::String)
    {
        string value = _obj.asString();
        std::transform(value.begin(), value.end(), value.begin(), [](unsigned char c) { return std::tolower(c); });
        _obj = value;
    }
}

// Remove keys starting with `//` from json
void mod_removeComments(DataObject& _obj)
{
    std::list<string> keysToRemove;
    for (auto& el : _obj.getSubObjectsUnsafe())
    {
        if (el->getKey()[0] == '/' && el->getKey()[1] == '/')
            keysToRemove.push_back(el->getKey());
    }
    for (auto const& key : keysToRemove)
        _obj.removeKey(key);
}

void mod_valueToCompactEvenHexPrefixed(DataObject& _obj)
{
    if (_obj.type() == DataType::String)
    {
        try
        {
            if (!(_obj.asString()[0] == '0' && _obj.asString()[1] == 'x'))
            {
                string src = _obj.asString();
                src.erase(std::remove(src.begin(), src.end(), '_'), src.end());
                _obj.setString(toCompactHexPrefixed(src, 1));
            }
        }
        catch (std::exception const& _ex)
        {
            throw UpwardsException(string(_ex.what()) + " Trying to convert to hex from `" + _obj.asString() + "`");
        }
    }
}

void mod_keyToCompactEvenHexPrefixed(DataObject& _obj)
{
    try
    {
        if (!(_obj.getKey()[0] == '0' && _obj.getKey()[1] == 'x'))
            _obj.setKey(toCompactHexPrefixed(_obj.getKey(), 1));
    }
    catch (std::exception const& _ex)
    {
        throw UpwardsException(
            string("keyToCompactEvenHexPrefixed error trying to convert string `" + _obj.getKey() + "` ") + _ex.what());
    }
}

// Remove leading zeros from hex values leaving 0x0004 - > 0x4
void mod_removeLeadingZerosFromHexValues(DataObject& _obj)
{
    if (_obj.type() == DataType::String)
    {
        string str = _obj.asString();
        removeLeadingZeroes(str);
        _obj.setString(str);
    }
}

// Remove leading zeros from hex values leaving 0x0004 - > 0x04
void mod_removeLeadingZerosFromHexValuesEVEN(DataObject& _obj)
{
    mod_removeLeadingZerosFromHexValues(_obj);
    if (_obj.type() == DataType::String)
    {
        DigitsType t = stringIntegerType(_obj.asString());
        if (t == DigitsType::UnEvenHexPrefixed)
            _obj.setString("0x0" + _obj.asString().substr(2));
    }
}

void mod_removeLeadingZerosFromHexKeysEVEN(DataObject& _obj)
{
    string str = _obj.getKey();
    removeLeadingZeroes(str);
    DigitsType t = stringIntegerType(str);
    if (t == DigitsType::UnEvenHexPrefixed)
        str = "0x0" + str.substr(2);
    _obj.setKey(str);
}

long long int hexOrDecStringToInt(string const& _str)
{
    long long int res;
    if (isHexDigitsType(stringIntegerType(_str)))
        res = (long long int)dev::u256(_str);
    else
    {
        res = atoll(_str.c_str());
        string const converted = fto_string(res);
        if (converted < _str)
            ETH_WARNING("Error converting `hexOrDecStringToInt`, decoded:  '" + converted + "' from '" + _str + "'");
    }
    return res;
}

void requireJsonFields(
    DataObject const& _o, std::string const& _section, std::map<std::string, possibleType> const& _validationMap, bool _fail)
{
    // check for unexpected fiedls
    for (auto const& field : _o.getSubObjects())
    {
        string message = "'" + field->getKey() + "' should not be declared in '" + _section + "' section!";
        if (_fail)
            ETH_FAIL_REQUIRE_MESSAGE(_validationMap.count(field->getKey()), message);
        else
            ETH_ERROR_REQUIRE_MESSAGE(_validationMap.count(field->getKey()), message);
    }

    // check field types with validation map
    for (auto const& vmap : _validationMap)
    {
        string message = vmap.first + " not found in " + _section + " section! " + TestOutputHelper::get().testName();
        if (_fail)
            ETH_FAIL_REQUIRE_MESSAGE(_o.count(vmap.first) > 0, message);
        else
            ETH_ERROR_REQUIRE_MESSAGE(_o.count(vmap.first) > 0, message);
        bool matched = false;
        std::string sTypes;
        for (auto const& type : vmap.second)
        {
            if (sTypes.size())
                sTypes += " or ";
            sTypes += DataObject::dataTypeAsString(type);
            if (_o.atKey(vmap.first).type() == type)
                matched = true;
        }
        if (matched == false)
        {
            std::string comment = _section + " '" + vmap.first + "' expected to be '" + sTypes + "', but set to: '" +
                                  DataObject::dataTypeAsString(_o.atKey(vmap.first).type()) + "' in " +
                                  TestOutputHelper::get().testName();
            if (_fail)
                ETH_FAIL_MESSAGE(comment + "\n" + _o.asJson());
            else
                ETH_ERROR_MESSAGE(comment + "\n" + _o.asJson());
        }
    }
}

void requireJsonFields(DataObject const& _o, std::string const& _config, std::map<std::string, jsonType> const& _validationMap)
{
    // check for unexpected fiedls
    for (auto const& field : _o.getSubObjects())
    {
        if (!_validationMap.count(field->getKey()))
        {
            std::string const comment = "Unexpected field '" + field->getKey() + "' in config: " + _config;
            ETH_ERROR_MESSAGE(comment + "\n" + _o.asJson());
        }
    }

    // check field types with validation map
    for (auto const& vmap : _validationMap)
    {
        // check that all required fields are in the object
        if (!_o.count(vmap.first))
        {
            if (vmap.second.second == jsonField::Required)
            {
                std::string const comment = "Expected field '" + vmap.first + "' not found in config: " + _config;
                ETH_ERROR_MESSAGE(comment + "\n" + _o.asJson());
            }
            else if (vmap.second.second == jsonField::Optional)
                continue;
        }

        // check that field type is one of allowed field types
        bool matched = false;
        for (auto const& type : vmap.second.first)
        {
            if (_o.atKey(vmap.first).type() == type)
                matched = true;
        }
        if (matched == false)
        {
            std::string sTypes;
            for (auto const& type : vmap.second.first)
            {
                if (sTypes.size())
                    sTypes += ", or ";
                sTypes += DataObject::dataTypeAsString(type);
            }
            std::string const comment = "Field '" + vmap.first + "' expected to be `" + sTypes + "`, but set to `" +
                                        DataObject::dataTypeAsString(_o.atKey(vmap.first).type()) + "` in " + _config;
            ETH_ERROR_MESSAGE(comment + "\n" + _o.asJson());
        }
    }
}

// Compile LLL in code
// Convert dec fields to hex, add 0x prefix to accounts and storage keys
spDataObject convertDecStateToHex(DataObject const& _data, solContracts const& _preSolidity)
{
    // -- Compile LLL in pre state into byte code if not already
    // -- Convert State::Storage keys/values into hex
    spDataObject tmpD(new DataObject());
    tmpD.getContent().copyFrom(_data); // TODO copy HERE time consuming!!!
    for (auto& acc2 : (*tmpD).getSubObjectsUnsafe())
    {
        DataObject& acc = acc2.getContent();
        if (acc.getKey()[1] != 'x')
            acc.setKey("0x" + acc.getKey());
        acc["code"].setString(test::compiler::replaceCode(acc.atKey("code").asString(), _preSolidity));
        acc["nonce"].performModifier(mod_valueToCompactEvenHexPrefixed);
        acc["balance"].performModifier(mod_valueToCompactEvenHexPrefixed);
        for (auto& rec : acc["storage"].getSubObjectsUnsafe())
        {
            rec.getContent().performModifier(mod_keyToCompactEvenHexPrefixed);
            rec.getContent().performModifier(mod_valueToCompactEvenHexPrefixed);
        }
    }
    return tmpD;
}

// Convert dec fields to hex, add 0x prefix to accounts and storage keys
spDataObject convertDecBlockheaderIncompleteToHex(DataObject const& _data)
{
    // Convert to HEX
    spDataObject tmpD(new DataObject());
    (*tmpD).copyFrom(_data);               // TODO copy time consuming!!!
    (*tmpD).removeKey("RelTimestamp");     // BlockchainTestFiller fields
    (*tmpD).removeKey("chainname");        // BlockchainTestFiller fields

    std::vector<string> hashKeys = {"parentHash", "coinbase", "bloom"};
    for (auto const& key : hashKeys)
        if (_data.count(key))
        {
            if (_data.atKey(key).asString().size() > 1 && _data.atKey(key).asString()[1] != 'x')
                (*tmpD)[key] = "0x" + _data.atKey(key).asString();
        }

    std::vector<string> valueKeys = {
        "difficulty",
        "gasLimit",
        "baseFeePerGas",
        "gasUsed",
        "nonce",
        "number",
        "timestamp"
    };
    for (auto const& key : valueKeys)
        if (_data.count(key))
            (*tmpD)[key].performModifier(mod_valueToCompactEvenHexPrefixed);
    return tmpD;
}

// Construct comapasion string
string compareBlockHeaders(DataObject const& _blockA, DataObject const& _blockB, string& _whatField)
{
    size_t k = 0;
    string message;
    bool errorInHashField = false;
    _whatField = string();
    ETH_ERROR_REQUIRE_MESSAGE(_blockA.getSubObjects().size() == _blockB.getSubObjects().size(),
        "compareBlockHeaders  _blockA.size() != _blockB.size()");
    for (auto const& el2 : _blockA.getSubObjects())
    {
        DataObject const& el = el2;
        string const testHeaderField = _blockB.getSubObjects().at(k++)->asString();
        message += cYellow + el.getKey() + cRed + " ";
        if (el.asString() != testHeaderField)
        {
            if (el.getKey() != "hash")
            {
                if (_whatField.empty())
                    _whatField = el.getKey();
            }
            else
                errorInHashField = true;
            message += el.asString() + " vs " + cYellow + testHeaderField + cRed + "\n";
        }
        else
            message += el.asString() + " vs " + testHeaderField + "\n";
    }
    if (_whatField.empty() && errorInHashField)
        _whatField = "hash";
    return message;
}

void readExpectExceptions(DataObject const& _data, std::map<FORK, string>& _out)
{
    for (auto const& rec : _data.getSubObjects())
    {
        // Parse ">=Frontier" : "EXCEPTION"
        ClientConfig const& cfg = Options::get().getDynamicOptions().getCurrentConfig();
        std::set<string> forksString = {rec->getKey()};
        std::vector<FORK> parsedForks = cfg.translateNetworks(forksString);
        for (auto const& el : parsedForks)
            _out.emplace(el, rec->asString());
    }
}

}  // namespace teststruct
}  // namespace test
