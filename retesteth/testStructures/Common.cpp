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
            {
                if (Options::get().convertpy)
                    acc["code_raw"] = code;

                acc[c_code].setString(test::compiler::replaceCode(code, _preSolidity));

                if (Options::get().convertpy)
                {
                    if (acc["code_raw"].asString() == acc[c_code].asString())
                        acc.removeKey("code_raw");
                }
            }
            if (code.empty())
                acc[c_code].asStringUnsafe().insert(0, "0x");
        }

        if (acc.count(c_nonce))
            acc[c_nonce].performModifier(mod_valueToCompactEvenHexPrefixed);
        if (acc.count(c_balance))
            acc[c_balance].performModifier(mod_valueToCompactEvenHexPrefixed);
        if (acc.count(c_storage))
        {
            std::vector<string> emptyKeys;
            for (auto& rec : acc[c_storage].getSubObjectsUnsafe())
            {
                rec.getContent().performModifier(mod_valueToCompactEvenHexPrefixed);
                if (rec->asString() == "0x00")
                    emptyKeys.emplace_back(rec->getKey());
                else
                {
                    rec.getContent().performModifier(mod_keyToCompactEvenHexPrefixed);
                    acc.performModifier(mod_keyToLowerCase);
                }
            }
            for (auto const& key : emptyKeys)
                acc[c_storage].removeKey(key);
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

bool checkEmptyAccounts(spState _state)
{
    for (auto const& [address, acc] : _state->accounts())
    {
        if (acc->nonce() == 0 && acc->balance() == 0 && acc->code().asString() == "0x")
            return true;
    }
    return false;
}

void checkEmptyStorages(spState _state)
{
    for (auto const& [address, acc] : _state->accounts())
    {
        for (auto const& [str, record] : acc->storage().getKeys())
        {
            if (std::get<1>(record)->asBigInt() == 0)
            {
                ETH_ERROR_MESSAGE("Pre state has empty storage record in account: " + address.asString() + TestOutputHelper::get().testInfo().errorDebug());
                return;
            }
        }
    }
}

spAccountBase makeBeaconAccount()
{
    spDataObject accountData;
    (*accountData).setKey(teststruct::C_FH20_BEACON.asString());
    (*accountData)["balance"] = "0x00";
    (*accountData)["code"] = "0x3373fffffffffffffffffffffffffffffffffffffffe14604d57602036146024575f5ffd5b5f35801560495762001fff810690815414603c575f5ffd5b62001fff01545f5260205ff35b5f5ffd5b62001fff42064281555f359062001fff015500";
    (*accountData)["nonce"] = "0x01";
    (*accountData).atKeyPointer("storage") = sDataObject(DataType::Object);
    spAccountBase acc(new State::Account(accountData));
    return  acc;
}

spAccountBase makeHistoryAccount()
{
    spDataObject accountData;
    (*accountData).setKey(teststruct::C_FH20_HISTORY_2935.asString());
    (*accountData)["balance"] = "0x00";
    (*accountData)["code"] = "0x3373fffffffffffffffffffffffffffffffffffffffe14604657602036036042575f35600143038111604257611fff81430311604257611fff9006545f5260205ff35b5f5ffd5b5f35611fff60014303065500";
    (*accountData)["nonce"] = "0x01";
    (*accountData).atKeyPointer("storage") = sDataObject(DataType::Object);
    spAccountBase acc(new State::Account(accountData));
    return  acc;
}

spAccountBase makeDepositsAccount()
{
    spDataObject accountData;
    (*accountData).setKey(teststruct::C_FH20_DEPOSITS_6110.asString());
    (*accountData)["balance"] = "0x00";
    (*accountData)["code"] = "0x60806040526004361061003f5760003560e01c806301ffc9a71461004457806322895118146100a4578063621fd130146101ba578063c5f2892f14610244575b600080fd5b34801561005057600080fd5b506100906004803603602081101561006757600080fd5b50357fffffffff000000000000000000000000000000000000000000000000000000001661026b565b604080519115158252519081900360200190f35b6101b8600480360360808110156100ba57600080fd5b8101906020810181356401000000008111156100d557600080fd5b8201836020820111156100e757600080fd5b8035906020019184600183028401116401000000008311171561010957600080fd5b91939092909160208101903564010000000081111561012757600080fd5b82018360208201111561013957600080fd5b8035906020019184600183028401116401000000008311171561015b57600080fd5b91939092909160208101903564010000000081111561017957600080fd5b82018360208201111561018b57600080fd5b803590602001918460018302840111640100000000831117156101ad57600080fd5b919350915035610304565b005b3480156101c657600080fd5b506101cf6110b5565b6040805160208082528351818301528351919283929083019185019080838360005b838110156102095781810151838201526020016101f1565b50505050905090810190601f1680156102365780820380516001836020036101000a031916815260200191505b509250505060405180910390f35b34801561025057600080fd5b506102596110c7565b60408051918252519081900360200190f35b60007fffffffff0000000000000000000000000000000000000000000000000000000082167f01ffc9a70000000000000000000000000000000000000000000000000000000014806102fe57507fffffffff0000000000000000000000000000000000000000000000000000000082167f8564090700000000000000000000000000000000000000000000000000000000145b92915050565b6030861461035d576040517f08c379a00000000000000000000000000000000000000000000000000000000081526004018080602001828103825260268152602001806118056026913960400191505060405180910390fd5b602084146103b6576040517f08c379a000000000000000000000000000000000000000000000000000000000815260040180806020018281038252603681526020018061179c6036913960400191505060405180910390fd5b6060821461040f576040517f08c379a00000000000000000000000000000000000000000000000000000000081526004018080602001828103825260298152602001806118786029913960400191505060405180910390fd5b670de0b6b3a7640000341015610470576040517f08c379a00000000000000000000000000000000000000000000000000000000081526004018080602001828103825260268152602001806118526026913960400191505060405180910390fd5b633b9aca003406156104cd576040517f08c379a00000000000000000000000000000000000000000000000000000000081526004018080602001828103825260338152602001806117d26033913960400191505060405180910390fd5b633b9aca00340467ffffffffffffffff811115610535576040517f08c379a000000000000000000000000000000000000000000000000000000000815260040180806020018281038252602781526020018061182b6027913960400191505060405180910390fd5b6060610540826114ba565b90507f649bbc62d0e31342afea4e5cd82d4049e7e1ee912fc0889aa790803be39038c589898989858a8a6105756020546114ba565b6040805160a0808252810189905290819060208201908201606083016080840160c085018e8e80828437600083820152601f017fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffe01690910187810386528c815260200190508c8c808284376000838201819052601f9091017fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffe01690920188810386528c5181528c51602091820193918e019250908190849084905b83811015610648578181015183820152602001610630565b50505050905090810190601f1680156106755780820380516001836020036101000a031916815260200191505b5086810383528881526020018989808284376000838201819052601f9091017fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffe0169092018881038452895181528951602091820193918b019250908190849084905b838110156106ef5781810151838201526020016106d7565b50505050905090810190601f16801561071c5780820380516001836020036101000a031916815260200191505b509d505050505050505050505050505060405180910390a1600060028a8a600060801b604051602001808484808284377fffffffffffffffffffffffffffffffff0000000000000000000000000000000090941691909301908152604080517ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff0818403018152601090920190819052815191955093508392506020850191508083835b602083106107fc57805182527fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffe090920191602091820191016107bf565b51815160209384036101000a7fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff01801990921691161790526040519190930194509192505080830381855afa158015610859573d6000803e3d6000fd5b5050506040513d602081101561086e57600080fd5b5051905060006002806108846040848a8c6116fe565b6040516020018083838082843780830192505050925050506040516020818303038152906040526040518082805190602001908083835b602083106108f857805182527fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffe090920191602091820191016108bb565b51815160209384036101000a7fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff01801990921691161790526040519190930194509192505080830381855afa158015610955573d6000803e3d6000fd5b5050506040513d602081101561096a57600080fd5b5051600261097b896040818d6116fe565b60405160009060200180848480828437919091019283525050604080518083038152602092830191829052805190945090925082918401908083835b602083106109f457805182527fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffe090920191602091820191016109b7565b51815160209384036101000a7fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff01801990921691161790526040519190930194509192505080830381855afa158015610a51573d6000803e3d6000fd5b5050506040513d6020811015610a6657600080fd5b5051604080516020818101949094528082019290925280518083038201815260609092019081905281519192909182918401908083835b60208310610ada57805182527fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffe09092019160209182019101610a9d565b51815160209384036101000a7fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff01801990921691161790526040519190930194509192505080830381855afa158015610b37573d6000803e3d6000fd5b5050506040513d6020811015610b4c57600080fd5b50516040805160208101858152929350600092600292839287928f928f92018383808284378083019250505093505050506040516020818303038152906040526040518082805190602001908083835b60208310610bd957805182527fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffe09092019160209182019101610b9c565b51815160209384036101000a7fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff01801990921691161790526040519190930194509192505080830381855afa158015610c36573d6000803e3d6000fd5b5050506040513d6020811015610c4b57600080fd5b50516040518651600291889160009188916020918201918291908601908083835b60208310610ca957805182527fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffe09092019160209182019101610c6c565b6001836020036101000a0380198251168184511680821785525050505050509050018367ffffffffffffffff191667ffffffffffffffff1916815260180182815260200193505050506040516020818303038152906040526040518082805190602001908083835b60208310610d4e57805182527fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffe09092019160209182019101610d11565b51815160209384036101000a7fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff01801990921691161790526040519190930194509192505080830381855afa158015610dab573d6000803e3d6000fd5b5050506040513d6020811015610dc057600080fd5b5051604080516020818101949094528082019290925280518083038201815260609092019081905281519192909182918401908083835b60208310610e3457805182527fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffe09092019160209182019101610df7565b51815160209384036101000a7fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff01801990921691161790526040519190930194509192505080830381855afa158015610e91573d6000803e3d6000fd5b5050506040513d6020811015610ea657600080fd5b50519050858114610f02576040517f08c379a00000000000000000000000000000000000000000000000000000000081526004018080602001828103825260548152602001806117486054913960600191505060405180910390fd5b60205463ffffffff11610f60576040517f08c379a00000000000000000000000000000000000000000000000000000000081526004018080602001828103825260218152602001806117276021913960400191505060405180910390fd5b602080546001019081905560005b60208110156110a9578160011660011415610fa0578260008260208110610f9157fe5b0155506110ac95505050505050565b600260008260208110610faf57fe5b01548460405160200180838152602001828152602001925050506040516020818303038152906040526040518082805190602001908083835b6020831061102557805182527fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffe09092019160209182019101610fe8565b51815160209384036101000a7fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff01801990921691161790526040519190930194509192505080830381855afa158015611082573d6000803e3d6000fd5b5050506040513d602081101561109757600080fd5b50519250600282049150600101610f6e565b50fe5b50505050505050565b60606110c26020546114ba565b905090565b6020546000908190815b60208110156112f05781600116600114156111e6576002600082602081106110f557fe5b01548460405160200180838152602001828152602001925050506040516020818303038152906040526040518082805190602001908083835b6020831061116b57805182527fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffe0909201916020918201910161112e565b51815160209384036101000a7fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff01801990921691161790526040519190930194509192505080830381855afa1580156111c8573d6000803e3d6000fd5b5050506040513d60208110156111dd57600080fd5b505192506112e2565b600283602183602081106111f657fe5b015460405160200180838152602001828152602001925050506040516020818303038152906040526040518082805190602001908083835b6020831061126b57805182527fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffe0909201916020918201910161122e565b51815160209384036101000a7fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff01801990921691161790526040519190930194509192505080830381855afa1580156112c8573d6000803e3d6000fd5b5050506040513d60208110156112dd57600080fd5b505192505b6002820491506001016110d1565b506002826112ff6020546114ba565b600060401b6040516020018084815260200183805190602001908083835b6020831061135a57805182527fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffe0909201916020918201910161131d565b51815160209384036101000a7fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff01801990921691161790527fffffffffffffffffffffffffffffffffffffffffffffffff000000000000000095909516920191825250604080518083037ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff8018152601890920190819052815191955093508392850191508083835b6020831061143f57805182527fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffe09092019160209182019101611402565b51815160209384036101000a7fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff01801990921691161790526040519190930194509192505080830381855afa15801561149c573d6000803e3d6000fd5b5050506040513d60208110156114b157600080fd5b50519250505090565b60408051600880825281830190925260609160208201818036833701905050905060c082901b8060071a60f81b826000815181106114f457fe5b60200101907effffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff1916908160001a9053508060061a60f81b8260018151811061153757fe5b60200101907effffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff1916908160001a9053508060051a60f81b8260028151811061157a57fe5b60200101907effffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff1916908160001a9053508060041a60f81b826003815181106115bd57fe5b60200101907effffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff1916908160001a9053508060031a60f81b8260048151811061160057fe5b60200101907effffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff1916908160001a9053508060021a60f81b8260058151811061164357fe5b60200101907effffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff1916908160001a9053508060011a60f81b8260068151811061168657fe5b60200101907effffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff1916908160001a9053508060001a60f81b826007815181106116c957fe5b60200101907effffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff1916908160001a90535050919050565b6000808585111561170d578182fd5b83861115611719578182fd5b505082019391909203915056fe4465706f736974436f6e74726163743a206d65726b6c6520747265652066756c6c4465706f736974436f6e74726163743a207265636f6e7374727563746564204465706f7369744461746120646f6573206e6f74206d6174636820737570706c696564206465706f7369745f646174615f726f6f744465706f736974436f6e74726163743a20696e76616c6964207769746864726177616c5f63726564656e7469616c73206c656e6774684465706f736974436f6e74726163743a206465706f7369742076616c7565206e6f74206d756c7469706c65206f6620677765694465706f736974436f6e74726163743a20696e76616c6964207075626b6579206c656e6774684465706f736974436f6e74726163743a206465706f7369742076616c756520746f6f20686967684465706f736974436f6e74726163743a206465706f7369742076616c756520746f6f206c6f774465706f736974436f6e74726163743a20696e76616c6964207369676e6174757265206c656e677468a2646970667358221220dceca8706b29e917dacf25fceef95acac8d90d765ac926663ce4096195952b6164736f6c634300060b0033";
    (*accountData)["nonce"] = "0x01";
    (*accountData).atKeyPointer("storage") = sDataObject(DataType::Object);
    spAccountBase acc(new State::Account(accountData));
    return  acc;
}

spAccountBase makeWithdrawalsAccount()
{
    spDataObject accountData;
    (*accountData).setKey(teststruct::C_FH20_WITHDRAWALS_7002.asString());
    (*accountData)["balance"] = "0x00";
    (*accountData)["code"] = "0x3373fffffffffffffffffffffffffffffffffffffffe1460cb5760115f54807fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff146101f457600182026001905f5b5f82111560685781019083028483029004916001019190604d565b909390049250505036603814608857366101f457346101f4575f5260205ff35b34106101f457600154600101600155600354806003026004013381556001015f35815560010160203590553360601b5f5260385f601437604c5fa0600101600355005b6003546002548082038060101160df575060105b5f5b8181146101835782810160030260040181604c02815460601b8152601401816001015481526020019060020154807fffffffffffffffffffffffffffffffff00000000000000000000000000000000168252906010019060401c908160381c81600701538160301c81600601538160281c81600501538160201c81600401538160181c81600301538160101c81600201538160081c81600101535360010160e1565b910180921461019557906002556101a0565b90505f6002555f6003555b5f54807fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff14156101cd57505f5b6001546002828201116101e25750505f6101e8565b01600290035b5f555f600155604c025ff35b5f5ffd";
    (*accountData)["nonce"] = "0x01";
    (*accountData).atKeyPointer("storage") = sDataObject(DataType::Object);
    spAccountBase acc(new State::Account(accountData));
    return  acc;
}

spAccountBase makeWithdrawalsAccount2()
{
    spDataObject accountData;
    (*accountData).setKey(teststruct::C_FH20_WITHDRAWALS_7251.asString());
    (*accountData)["balance"] = "0x00";
    (*accountData)["code"] = "0x3373fffffffffffffffffffffffffffffffffffffffe1460d35760115f54807fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff1461019a57600182026001905f5b5f82111560685781019083028483029004916001019190604d565b9093900492505050366060146088573661019a573461019a575f5260205ff35b341061019a57600154600101600155600354806004026004013381556001015f358155600101602035815560010160403590553360601b5f5260605f60143760745fa0600101600355005b6003546002548082038060021160e7575060025b5f5b8181146101295782810160040260040181607402815460601b815260140181600101548152602001816002015481526020019060030154905260010160e9565b910180921461013b5790600255610146565b90505f6002555f6003555b5f54807fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff141561017357505f5b6001546001828201116101885750505f61018e565b01600190035b5f555f6001556074025ff35b5f5ffd";
    (*accountData)["nonce"] = "0x01";
    (*accountData).atKeyPointer("storage") = sDataObject(DataType::Object);
    spAccountBase acc(new State::Account(accountData));
    return  acc;
}

void makeCancunPrecompiledAccounts(State const& _state, VALUE const& _timestamp, std::vector<spAccountBase>& _result)
{
    constexpr int HISTORY_BUFFER_LENGTH = 8191;
    if (_state.hasAccount(teststruct::C_FH20_BEACON))
        return;

    ETH_DC_MESSAGE(DC::RPC, "Retesteth inserts beacon root account into the pre state!");
    auto beaconAcc = makeBeaconAccount();
    Storage& str = const_cast<Storage&>(beaconAcc->storage());

    // header.timestamp % HISTORY_BUFFER_LENGTH to be header.timestamp
    spVALUE key = sVALUE(_timestamp.asBigInt() % HISTORY_BUFFER_LENGTH);
    spVALUE val = sVALUE(_timestamp.asBigInt());
    if (val->asBigInt() != 0)
        str.addRecord({key, val});

    _result.emplace_back(beaconAcc);
}

void makePraguePrecompiledAccounts(State const& _state, std::vector<spAccountBase>& _result)
{
    if(!_state.hasAccount(teststruct::C_FH20_HISTORY_2935))
    {
        ETH_DC_MESSAGE(DC::RPC, "Retesteth inserts history contract account into the pre state!");
        auto historyAcc = makeHistoryAccount();
        _result.emplace_back(historyAcc);
    }

    if (!_state.hasAccount(teststruct::C_FH20_DEPOSITS_6110))
    {
        ETH_DC_MESSAGE(DC::RPC, "Retesteth inserts deposits contract account into the pre state!");
        auto depositAcc = makeDepositsAccount();
        _result.emplace_back(depositAcc);
    }

    if (!_state.hasAccount(teststruct::C_FH20_WITHDRAWALS_7002))
    {
        ETH_DC_MESSAGE(DC::RPC, "Retesteth inserts withdrawals contract account into the pre state!");
        auto withdrawalsAcc = makeWithdrawalsAccount();
        _result.emplace_back(withdrawalsAcc);
    }

    if (!_state.hasAccount(teststruct::C_FH20_WITHDRAWALS_7251))
    {
        ETH_DC_MESSAGE(DC::RPC, "Retesteth inserts withdrawals contract 2 account into the pre state!");
        auto withdrawalsAcc = makeWithdrawalsAccount2();
        _result.emplace_back(withdrawalsAcc);
    }
}

}  // namespace teststruct
