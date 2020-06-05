#include "Common.h"
#include "EthChecks.h"
#include "TestHelper.h"
#include "TestOutputHelper.h"
#include <retesteth/Options.h>
#include <retesteth/configs/ClientConfig.h>
#include <mutex>

using namespace test;
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
        if (el.getKey()[0] == '/' && el.getKey()[1] == '/')
            keysToRemove.push_back(el.getKey());
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
            _obj.setString(toCompactHexPrefixed(_obj.asString(), 1));
        }
        catch (std::exception const& _ex)
        {
            throw BaseEthException(string(_ex.what()) + " Trying to convert to hex from `" + _obj.asString() + "`");
        }
    }
}

void mod_keyToCompactEvenHexPrefixed(DataObject& _obj)
{
    try
    {
        _obj.setKey(toCompactHexPrefixed(_obj.getKey(), 1));
    }
    catch (std::exception const& _ex)
    {
        throw BaseEthException(
            string("keyToCompactEvenHexPrefixed error trying to convert string `" + _obj.getKey() + "` ") + _ex.what());
    }
}

// Remove leading zeros from hex values leaving 0x0004 - > 0x4
void mod_removeLeadingZerosFromHexValues(DataObject& _obj)
{
    if (_obj.type() == DataType::String)
    {
        string const& origVal = _obj.asString();
        bool replacePossible = true;
        while (replacePossible)
        {
            if (origVal[0] == '0' && origVal[1] == 'x' && origVal[2] == '0' && origVal.size() >= 4)
            {
                _obj.setString("0x" + origVal.substr(3));
                continue;
            }
            replacePossible = false;
        }
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

long int hexOrDecStringToInt(string const& _str)
{
    if (isHexDigitsType(stringIntegerType(_str)))
        return (long int)dev::u256(_str);
    else
        return atoi(_str.c_str());
}


DataObject prepareChainParams(FORK const& _net, SealEngine _engine, State const& _state, StateTestEnvBase const& _env)
{
    ClientConfig const& cfg = Options::get().getDynamicOptions().getCurrentConfig();
    cfg.validateForkAllowed(_net);

    DataObject genesis;
    genesis = cfg.getGenesisTemplate(_net);
    genesis["sealEngine"] = sealEngineToStr(_engine);

    genesis["genesis"]["author"] = _env.currentCoinbase().asString();
    genesis["genesis"]["difficulty"] = _env.currentDifficulty().asString();
    genesis["genesis"]["gasLimit"] = _env.currentGasLimit().asString();
    genesis["genesis"]["extraData"] = _env.currentExtraData().asString();
    genesis["genesis"]["timestamp"] = _env.currentTimestamp().asString();
    genesis["genesis"]["nonce"] = _env.currentNonce().asString();
    genesis["genesis"]["mixHash"] = _env.currentMixHash().asString();

    // Because of template might contain preset accounts
    for (auto const& el : _state.accounts())
        genesis["accounts"].addSubObject(el.second.getCContent().asDataObject());
    return genesis;

    // CHAIN PARAMS AS A STRUCTURE ???
}


void requireJsonFields(
    DataObject const& _o, std::string const& _section, std::map<std::string, possibleType> const& _validationMap, bool _fail)
{
    // check for unexpected fiedls
    for (auto const field : _o.getSubObjects())
    {
        string message = "'" + field.getKey() + "' should not be declared in '" + _section + "' section!";
        if (_fail)
            ETH_FAIL_REQUIRE_MESSAGE(_validationMap.count(field.getKey()), message);
        else
            ETH_ERROR_REQUIRE_MESSAGE(_validationMap.count(field.getKey()), message);
    }

    // check field types with validation map
    for (auto const vmap : _validationMap)
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
        if (!_validationMap.count(field.getKey()))
        {
            std::string const comment = "Unexpected field '" + field.getKey() + "' in config: " + _config;
            ETH_ERROR_MESSAGE(comment + "\n" + _o.asJson());
        }
    }

    // check field types with validation map
    for (auto const vmap : _validationMap)
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
            std::string const comment = "Field '" + vmap.first + "' expected to be " + sTypes + ", but set to " +
                                        DataObject::dataTypeAsString(_o.atKey(vmap.first).type()) + " in " + _config;
            ETH_ERROR_MESSAGE(comment + "\n" + _o.asJson());
        }
    }
}

// Compile LLL in code
// Convert dec fields to hex, add 0x prefix to accounts and storage keys
DataObject convertDecStateToHex(DataObject const& _data)
{
    // -- Compile LLL in pre state into byte code if not already
    // -- Convert State::Storage keys/values into hex
    DataObject tmpD = _data;
    for (auto& acc : tmpD.getSubObjectsUnsafe())
    {
        if (acc.getKey()[1] != 'x')
            acc.setKey("0x" + acc.getKey());
        acc["code"].setString(test::replaceCode(acc.atKey("code").asString()));
        acc["nonce"].performModifier(mod_valueToCompactEvenHexPrefixed);
        acc["balance"].performModifier(mod_valueToCompactEvenHexPrefixed);
        for (auto& rec : acc["storage"].getSubObjectsUnsafe())
        {
            rec.performModifier(mod_keyToCompactEvenHexPrefixed);
            rec.performModifier(mod_valueToCompactEvenHexPrefixed);
        }
    }
    return tmpD;
}

// Convert dec fields to hex, add 0x prefix to accounts and storage keys
DataObject convertDecBlockheaderIncompleteToHex(DataObject const& _data)
{
    // Convert to HEX
    DataObject tmpD = _data;
    tmpD.removeKey("updatePoW");            // BlockchainTestFiller fields
    tmpD.removeKey("RelTimestamp");         // BlockchainTestFiller fields
    tmpD.removeKey("expectException");      // BlockchainTestFiller fields
    tmpD.removeKey("overwriteAndRedoPoW");  // BlockchainTestFiller fields
    tmpD.removeKey("populateFromBlock");    // BlockchainTestFiller fields
    tmpD.removeKey("chainname");            // BlockchainTestFiller fields
    tmpD.removeKey("RelTimestampFromPopulateBlock");

    std::vector<string> hashKeys = {"parentHash", "coinbase"};
    for (auto const& key : hashKeys)
        if (_data.count(key))
        {
            if (_data.atKey(key).asString().size() > 1 && _data.atKey(key).asString()[1] != 'x')
                tmpD[key] = "0x" + _data.atKey(key).asString();
            tmpD[key].performModifier(mod_valueToCompactEvenHexPrefixed);
        }

    std::vector<string> valueKeys = {"difficulty", "gasLimit", "gasUsed", "nonce", "number", "timestamp"};
    for (auto const& key : valueKeys)
        if (_data.count(key))
            tmpD[key].performModifier(mod_valueToCompactEvenHexPrefixed);
    return tmpD;
}

std::mutex g_strFindMutex;
DigitsType stringIntegerType(std::string const& _string)
{
    if (_string[0] == '0' && _string[1] == 'x')
    {
        string substring = _string.substr(2);
        DigitsType substringType = stringIntegerType(substring);
        if (substringType == DigitsType::Hex)
            return DigitsType::HexPrefixed;

        if (substringType == DigitsType::Decimal)
        {
            if (substring.size() % 2 == 0)
                return DigitsType::HexPrefixed;
            else
                return DigitsType::UnEvenHexPrefixed;
        }

        if (substringType == DigitsType::UnEvenHex)
            return DigitsType::UnEvenHexPrefixed;
    }

    bool isDecimalOnly = true;
    std::lock_guard<std::mutex> lock(g_strFindMutex);  // string.find is not thread safe + static
    static std::string hexAlphabet = "0123456789abcdefABCDEF";
    static std::string decimalAlphabet = "0123456789";
    for (size_t i = 0; i < _string.length(); i++)
    {
        if (hexAlphabet.find(_string[i]) == std::string::npos)
            return DigitsType::String;

        if (decimalAlphabet.find(_string[i]) == std::string::npos)
            isDecimalOnly = false;
    }

    if (isDecimalOnly)
        return DigitsType::Decimal;

    if (_string.size() % 2 == 0)
        return DigitsType::Hex;

    return DigitsType::UnEvenHex;
}

}  // namespace teststruct
}  // namespace test
