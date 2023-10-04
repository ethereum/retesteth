#include "Common.h"
#include "EthChecks.h"
#include <retesteth/helpers/TestHelper.h>
#include <retesteth/helpers/TestOutputHelper.h>
#include <retesteth/Options.h>
#include <libdevcrypto/Common.h>
#include <retesteth/Constants.h>

using namespace std;
using namespace test;
using namespace test::debug;
using namespace dev;
using namespace test::teststruct;
using namespace test::teststruct::constnames;
using namespace test::compiler;

namespace
{

string toCompactHexPrefixed(string const& _str, size_t _minSize)
{
    string prefix = string();
    const DigitsType t = stringIntegerType(_str);
    if (t == DigitsType::Hex || t == DigitsType::UnEvenHex)
        prefix = "0x";
    return dev::toCompactHexPrefixed(u256(prefix + _str), _minSize);
}

bool isHexDigitsType(DigitsType _dtype)
{
    return (_dtype == DigitsType::HexPrefixed || _dtype == DigitsType::UnEvenHexPrefixed);
}

void removeLeadingZeroesIfHex(string& _hexStr)
{
    if (_hexStr.size() <= 2 || _hexStr.at(0) != '0' || _hexStr.at(1) != 'x')
        return;

    auto const it = std::find_if_not(_hexStr.begin() + 2, _hexStr.end() - 1, [](char c) { return c == '0'; });
    _hexStr.erase(_hexStr.begin() + 2, it);
}

}  // namespace


namespace test::teststruct
{

spFH20 convertSecretToPublic(VALUE const& _secret)
{
    spFH32 secret = sFH32(_secret.asString());
    return convertSecretToPublic(secret);
}

spFH20 convertSecretToPublic(spFH32 const& _secret)
{
    const dev::Secret secret(_secret->asString());
    return sFH20("0x" + dev::toAddress(dev::toPublic(secret)).hex());
}

// DataObject modifiers
void mod_valueToLowerCase(DataObject& _obj)
{
    if (_obj.type() == DataType::String)
    {
        string& value = _obj.asStringUnsafe();
        std::transform(value.begin(), value.end(), value.begin(), [](unsigned char c) { return std::tolower(c); });
    }
}

void mod_keyToLowerCase(DataObject& _obj)
{
    if (!_obj.getKey().empty())
    {
        string& value = _obj.getKeyUnsafe();
        std::transform(value.begin(), value.end(), value.begin(), [](unsigned char c) { return std::tolower(c); });
    }
}

// Remove keys starting with `//` from json
void mod_removeComments(DataObject& _obj)
{
    std::list<string> keysToRemove;
    for (auto& el : _obj.getSubObjectsUnsafe())
    {
        if (el->getKey()[0] == '/' && el->getKey()[1] == '/')
            keysToRemove.emplace_back(el->getKey());
    }
    for (auto const& key : keysToRemove)
        _obj.removeKey(key);
}

void mod_valueToFH32(DataObject& _obj)
{
    if (_obj.type() == DataType::String)
    {
        const string& str = _obj.asString();
        if (str.substr(0, 2) == "0x")
        {
            const size_t paddingCount = 66 - str.size();
            _obj.asStringUnsafe().insert(2, string(paddingCount, '0'));
        }
    }
}

void mod_valueToCompactEvenHexPrefixed(DataObject& _obj)
{
    if (_obj.type() == DataType::String)
    {
        try
        {
            string& str = _obj.asStringUnsafe();
            if (str.size() <= 2 || !(str.at(0) == '0' && str.at(1) == 'x'))
            {
                str.erase(std::remove(str.begin(), str.end(), '_'), str.end());
                _obj.setString(toCompactHexPrefixed(str, 1));
            }
            else
                _obj.performModifier(mod_removeLeadingZerosFromHexValueEVEN);
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
        string const& key = _obj.getKey();
        if (key.size() <= 2 || !(key.at(0) == '0' && key.at(1) == 'x'))
            _obj.setKey(toCompactHexPrefixed(key, 1));
        else
            _obj.performModifier(mod_removeLeadingZerosFromHexKeyEVEN);
    }
    catch (std::exception const& _ex)
    {
        throw UpwardsException(
            string("keyToCompactEvenHexPrefixed error trying to convert string `" + _obj.getKey() + "` ") + _ex.what());
    }
}

// Remove leading zeros from hex values leaving 0x0004 - > 0x4
void mod_removeBigIntHint(DataObject& _obj)
{
    if (_obj.type() == DataType::String)
    {
        static string const bigintSer = "0x:bigint ";
        string& value = _obj.asStringUnsafe();
        size_t isBigInt = value.find(bigintSer);
        if (isBigInt != string::npos)
            value.erase(isBigInt, bigintSer.size());
    }
}

// Remove leading zeros from hex values leaving 0x0004 - > 0x4
void mod_removeLeadingZerosFromHexValues(DataObject& _obj)
{
    if (_obj.type() == DataType::String)
    {
        string& str = _obj.asStringUnsafe();
        removeLeadingZeroesIfHex(str);
    }
}

// Remove leading zeros from hex values leaving 0x0004 - > 0x04
void mod_removeLeadingZerosFromHexValueEVEN(DataObject& _obj)
{
    mod_removeLeadingZerosFromHexValues(_obj);
    if (_obj.type() == DataType::String)
    {
        const DigitsType t = stringIntegerType(_obj.asString());
        if (t == DigitsType::UnEvenHexPrefixed)
            _obj.asStringUnsafe().replace(0, 2, "0x0", 3);
    }
}

void mod_removeLeadingZerosFromHexKeyEVEN(DataObject& _obj)
{
    string& str = _obj.getKeyUnsafe();
    removeLeadingZeroesIfHex(str);
    const DigitsType t = stringIntegerType(str);
    if (t == DigitsType::UnEvenHexPrefixed)
        str.replace(0, 2, "0x0", 3);
}

void mod_valueInsertZeroXPrefix(DataObject& _obj)
{
    if (_obj.asString().size() > 1 && _obj.asString().at(1) != 'x')
        _obj.asStringUnsafe().insert(0, "0x");
}

void mod_sortKeys(DataObject& _obj)
{
    if (_obj.isArray())
    {
        std::map<string, spDataObject> const map = _obj.getSubObjectKeys();
        if (map.size() > 1)
        {
            _obj.clearSubobjects();
            _obj.setAutosort(true);
            for (auto const& el : map)
                _obj.atKeyPointer(el.second->getKey()) = el.second;
        }
    }
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

void requireJsonFields(DataObject const& _o, std::string const& _config, std::map<std::string, jsonType> const& _validationMap)
{
    // check for unexpected fiedls
    for (auto const& field : _o.getSubObjects())
    {
        if (!_validationMap.count(field->getKey()))
        {
            std::string const comment = "Unexpected field '" + field->getKey() + "' in config: " + _config;
            throw test::UpwardsException(comment + "\n" + _o.asJson());
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
                throw test::UpwardsException(comment + "\n" + _o.asJson());
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
            throw test::UpwardsException(comment + "\n" + _o.asJson());
        }
    }
}

// Compile LLL in code
// Convert dec fields to hex, add 0x prefix to accounts and storage keys
void convertDecStateToHex(spDataObject& _data, solContracts const& _preSolidity, StateToHex _compileCode)
{
    // -- Compile LLL in pre state into byte code if not already
    // -- Convert State::Storage keys/values into hex
    for (auto& acc2 : (*_data).getSubObjectsUnsafe())
    {
        DataObject& acc = acc2.getContent();
        if (acc.getKey()[1] != 'x')
            acc.getKeyUnsafe().insert(0, "0x");
        acc.performModifier(mod_keyToLowerCase);

        if (acc.count(c_code))
        {
            auto const& code = acc.atKey(c_code).asString();
            if (_compileCode == StateToHex::COMPILECODE)
                acc[c_code].setString(test::compiler::replaceCode(code, _preSolidity));
            if (code.empty())
                acc[c_code].asStringUnsafe().insert(0, "0x");
        }

        if (acc.count(c_nonce))
            acc[c_nonce].performModifier(mod_valueToCompactEvenHexPrefixed);
        if (acc.count(c_balance))
            acc[c_balance].performModifier(mod_valueToCompactEvenHexPrefixed);
        if (acc.count(c_storage))
        {
            for (auto& rec : acc[c_storage].getSubObjectsUnsafe())
            {
                rec.getContent().performModifier(mod_keyToCompactEvenHexPrefixed);
                rec.getContent().performModifier(mod_valueToCompactEvenHexPrefixed);
                acc.performModifier(mod_keyToLowerCase);
            }
        }
        acc.performModifier(mod_valueToLowerCase);
    }
}

// Convert dec fields to hex, add 0x prefix to accounts and storage keys
spDataObject convertDecBlockheaderIncompleteToHex(DataObject const& _data)
{
    // Convert to HEX
    spDataObject tmpD;
    (*tmpD).copyFrom(_data);               // TODO copy time consuming!!!
    (*tmpD).removeKey("RelTimestamp");     // BlockchainTestFiller fields
    (*tmpD).removeKey("chainname");        // BlockchainTestFiller fields

    static const std::vector<string> hashKeys = {c_parentHash, c_coinbase, c_bloom};
    for (auto const& key : hashKeys)
        if (_data.count(key))
        {
            auto const& data = _data.atKey(key).asString();
            if (data.size() > 1 && data.at(1) != 'x')
                (*tmpD)[key] = "0x" + data;
        }

    static const std::vector<string> valueKeys = {
        c_difficulty,
        c_gasLimit,
        c_baseFeePerGas,
        c_gasUsed,
        c_nonce,
        c_number,
        c_timestamp
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
        if (Options::get().nologcolor)
            message += el.getKey() + " ";
        else
            message += cYellow + el.getKey() + cRed + " ";
        if (el.asString() != testHeaderField)
        {
            if (el.getKey() != c_hash)
            {
                if (_whatField.empty())
                    _whatField = el.getKey();
            }
            else
                errorInHashField = true;
            if (Options::get().nologcolor)
                message += el.asString() + " vs " + testHeaderField + "\n";
            else
                message += el.asString() + " vs " + cYellow + testHeaderField + cRed + "\n";
        }
        else
            message += el.asString() + " vs " + testHeaderField + "\n";
    }
    if (_whatField.empty() && errorInHashField)
        _whatField = c_hash;
    return message;
}

void readExpectExceptions(DataObject const& _data, std::map<FORK, string>& _out)
{
    for (auto const& rec : _data.getSubObjects())
    {
        // Parse ">=Frontier" : "EXCEPTION"
        ClientConfig const& cfg = Options::get().getDynamicOptions().getCurrentConfig();
        const std::set<string> forksString = {rec->getKey()};
        if (rec->getKey().size() < 3)
            ETH_ERROR_MESSAGE("readExpectExceptions:: Dataobject key as fork is too short around: " + _data.asJson());
        const std::vector<FORK> parsedForks = cfg.translateNetworks(forksString);
        for (auto const& el : parsedForks)
            _out.emplace(el, rec->asString());
    }
}

void convertDecTransactionToHex(spDataObject& _data)
{
    DataObject& data = _data.getContent();
    data.performModifier(mod_valueToCompactEvenHexPrefixed, DataObject::ModifierOption::RECURSIVE, {c_data, c_to});

    // fix 0x prefix on 'to' key
    string& to = data.atKeyUnsafe(c_to).asStringUnsafe();
    if (to.size() > 1 && to.at(1) != 'x')
        to.insert(0, "0x");

    // Compile LLL in transaction data into byte code if not already
    data[c_data] = test::compiler::replaceCode(data.atKey(c_data).asString());

    data[c_data].performModifier(mod_valueToLowerCase);
    data[c_to].performModifier(mod_valueToLowerCase);
}

bool src_findBigInt(DataObject const& el)
{
    if (el.type() == DataType::String && el.asString().find(C_BIGINT_PREFIX) != string::npos)
        return true;
    return false;
}

void mod_changeValueAnyToBigint00(DataObject& _el)
{
    if (_el.type() == DataType::String && _el.asString() == "ANY")
        _el.setString("0x:bigint 0x00");
}

}  // namespace teststruct
