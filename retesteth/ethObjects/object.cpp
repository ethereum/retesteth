#include "object.h"
#include "Options.h"
#include <retesteth/TestHelper.h>
#include <mutex>

namespace test {
string object::emptyString;

// DataObject modifiers
void mod_valuesToLowerCase(DataObject& _obj)
{
    if (_obj.type() == DataType::String)
    {
        string value = _obj.asString();
        std::transform(value.begin(), value.end(), value.begin(),
            [](unsigned char c) { return std::tolower(c); });
        _obj = value;
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
        object::DigitsType t = object::stringIntegerType(_obj.asString());
        if (t == object::DigitsType::UnEvenHexPrefixed)
            _obj.setString("0x0" + _obj.asString().substr(2));
    }
}

// Check the validity of ethereum json representation
// !!! TODO:: implement structures and don't work with text?
// Initially retesteth was supposed not to know about Address, h256, h20, h32 types...
void ver_ethereumfields(DataObject const& _data)
{
    static vector<string> c_fieldsThatAreHashes256{"bloom"};
    static vector<string> c_fieldsThatAreHashes20{"coinbase"};
    static vector<string> c_fieldsThatAreHashes32{"parentHash", "uncleHash", "sha3Uncles",
        "stateRoot", "transactionsRoot", "transactionsTrie", "receiptTrie", "mixHash", "hash"};
    static vector<string> c_fieldsThatAreValues{"difficulty", "number", "gasLimit", "gasUsed",
        "timestamp", "value", "gasPrice", "v", "r", "s", "nonce"};

    // Special fields
    // `nonce` in block, `nonce` in tr/account
    // transaction `to`, `data` field

    // BlockHeader
    // 0 - parentHash           // 8 - number
    // 1 - uncleHash            // 9 - gasLimit
    // 2 - coinbase             // 10 - gasUsed
    // 3 - stateRoot            // 11 - timestamp
    // 4 - transactionsTrie     // 12 - extraData
    // 5 - receiptTrie          // 13 - mixHash
    // 6 - bloom                // 14 - nonce
    // 7 - difficulty

    // Transaction
    // 0 - nonce        3 - to      6 - v
    // 1 - gasPrice     4 - value   7 - r
    // 2 - gasLimit     5 - data    8 - s

    if (_data.type() == DataType::String)
    {
        string const& k = _data.getKey();
        string const& v = _data.asString();
        if (k == "extraData" && v.size() > 32 * 2 + 2)  // extraData, (up to) 32 bytes
            ETH_ERROR_MESSAGE("Key `" + k + "` is larger than 256bits `" + v + "`");
        if (test::inArray(c_fieldsThatAreHashes32, k) && !object::validateHash(v, 32))
            ETH_ERROR_MESSAGE("Key `" + k + "` is not hash32 `" + v + "`");
        if (test::inArray(c_fieldsThatAreHashes20, k) && !object::validateHash(v, 20))
            ETH_ERROR_MESSAGE("Key `" + k + "` is not hash20 `" + v + "`");
        if (test::inArray(c_fieldsThatAreHashes256, k) && !object::validateHash(v, 256))
            ETH_ERROR_MESSAGE("Key `" + k + "` is not hash256 `" + v + "`");
        if (test::inArray(c_fieldsThatAreValues, k))
        {
            if (k == "nonce" && v.size() == 18)  // allow nonce as hash8
                return;
            if (v[0] == '0' && v[1] == 'x')
            {
                if (v.size() == 2)
                    ETH_ERROR_MESSAGE("Key `" + k + "` is value, but set as empty byte string: `" + v + "`");
                // don't allow 0x001, but allow 0x0, 0x00
                if ((v[2] == '0' && v.size() % 2 == 1 && v.size() != 3) ||
                    (v[2] == '0' && v[3] == '0' && v.size() % 2 == 0 && v.size() > 4))
                    ETH_ERROR_MESSAGE("Key `" + k + "` has leading 0 `" + v + "`");
            }
            if (v.size() > 64 + 2)
                ETH_ERROR_MESSAGE("Key `" + k + "` >u256 `" + v + "`");
        }
    }
}

bool object::validateHash(std::string const& _hash, size_t _size)
{
    // validate 0x...... _size bytes hash
    if (_hash.size() != _size * 2 + 2 || stringIntegerType(_hash) != DigitsType::HexPrefixed)
        return false;
    return true;
}


bool isHexDigitsType(test::object::DigitsType _dtype)
{
    return (_dtype == test::object::DigitsType::HexPrefixed ||
            _dtype == test::object::DigitsType::UnEvenHexPrefixed);
}

long int hexOrDecStringToInt(string const& _str)
{
    if (isHexDigitsType(test::object::stringIntegerType(_str)))
        return (long int)dev::u256(_str);
    else
        return atoi(_str.c_str());
}


std::mutex g_strFindMutex;
object::DigitsType object::stringIntegerType(std::string const& _string)
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
    std::lock_guard<std::mutex> lock(
        g_strFindMutex); // string.find is not thread safe + static
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

std::string object::makeHexAddress(std::string const& _address)
{
    if (_address[0] == '0' && _address[1] == 'x')
        ETH_ERROR_REQUIRE_MESSAGE(_address.length() == 42,
            TestOutputHelper::get().testName() + ": Wrong address: '" + _address + "'");
    else
        ETH_ERROR_REQUIRE_MESSAGE(_address.length() == 40,
            TestOutputHelper::get().testName() + ": Wrong address: '" + _address + "'");

    ETH_ERROR_REQUIRE_MESSAGE(_address.length() % 2 == 0,
        TestOutputHelper::get().testName() + ": Hex data is expected to be of odd length: '" +
            _address + "'");
    switch (stringIntegerType(_address))
	{
		case DigitsType::HexPrefixed:
			return _address;
		case DigitsType::Decimal:
			BOOST_FALLTHROUGH;
		case DigitsType::Hex:
			return "0x" + _address;
		case DigitsType::String:
			BOOST_ERROR(TestOutputHelper::get().testName() + ": Hex data is expected to be hex string (but contains illegal char): " + _address);
			break;
		default:
			BOOST_ERROR(TestOutputHelper::get().testName() + "Unknown digits type! " + _address);
			break;
	}
	BOOST_ERROR(TestOutputHelper::get().testName() + "Error checking address field!");
	return _address;
}

std::string object::convertStringToHexPrefixed(string const& _input, short _minimumBytes)
{
    DigitsType type = stringIntegerType(_input);
    if (type == DigitsType::HexPrefixed || type == DigitsType::String)
        return _input;
    if (type == DigitsType::UnEvenHexPrefixed)
        return "0x0" + _input.substr(2);
    if (type == DigitsType::Decimal)
        return dev::toCompactHexPrefixed(dev::u256(_input), _minimumBytes);
    else if (type == DigitsType::Hex || type == DigitsType::UnEvenHex)
        return dev::toCompactHexPrefixed(dev::u256("0x" + _input), _minimumBytes);
    ETH_ERROR_MESSAGE("Unhendeled path in object::convertStringToHexPrefixed");
    return _input;
}

void object::makeKeyHex(DataObject& _key)
{
    // If transaction to field is empty its ok.
    if ((_key.getKey() == "to" || _key.getKey() == "extraData") && _key.asString().empty())
        return;

    // make empty data and code fields as "0x", others as "0x00" if 0
    static const std::set<std::string> empty0xFields = {"data", "code"};
    _key = convertStringToHexPrefixed(_key.asString(), empty0xFields.count(_key.getKey()) ? 0 : 1);

    // make key itself hex
    if (!_key.getKey().empty())
        _key.setKey(convertStringToHexPrefixed(_key.getKey(), 1));
}

void object::makeAllFieldsHex(DataObject& _data)
{
    if (_data.type() == DataType::Null)
        return;
    if (_data.type() == DataType::Object || _data.type() == DataType::Array)
    {
        for (auto& key: _data.getSubObjectsUnsafe())
        {
            if (stringIntegerType(key.getKey()) == DigitsType::Hex)
                key.setKey("0x" + key.getKey());
            if (key.type() == DataType::String)
                makeKeyHex(key);
            else
                makeAllFieldsHex(key);
        }
    }
    else
        makeKeyHex(_data);
}

void requireJsonFields(DataObject const& _o, std::string const& _section,
    std::map<std::string, possibleType> const& _validationMap, bool _fail)
{
    // check for unexpected fiedls
    for (auto const field : _o.getSubObjects())
    {
        string message =
            "'" + field.getKey() + "' should not be declared in '" + _section + "' section!";
        if (_fail)
            ETH_FAIL_REQUIRE_MESSAGE(_validationMap.count(field.getKey()), message);
        else
            ETH_ERROR_REQUIRE_MESSAGE(_validationMap.count(field.getKey()), message);
    }

    // check field types with validation map
    for (auto const vmap : _validationMap)
    {
        string message = vmap.first + " not found in " + _section + " section! " +
                         TestOutputHelper::get().testName();
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
            std::string comment = _section + " '" + vmap.first + "' expected to be '" + sTypes +
                                  "', but set to: '" +
                                  DataObject::dataTypeAsString(_o.atKey(vmap.first).type()) +
                                  "' in " + TestOutputHelper::get().testName();
            if (_fail)
                ETH_FAIL_MESSAGE(comment + "\n" + _o.asJson());
            else
                ETH_ERROR_MESSAGE(comment + "\n" + _o.asJson());
        }
    }
}

void requireJsonFields(DataObject const& _o, std::string const& _config,
    std::map<std::string, jsonType> const& _validationMap)
{
    // check for unexpected fiedls
    for (auto const& field : _o.getSubObjects())
    {
        if (!_validationMap.count(field.getKey()))
        {
            std::string const comment =
                "Unexpected field '" + field.getKey() + "' in config: " + _config;
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
                std::string const comment =
                    "Expected field '" + vmap.first + "' not found in config: " + _config;
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
            std::string const comment =
                "Field '" + vmap.first + "' expected to be " + sTypes + ", but set to " +
                DataObject::dataTypeAsString(_o.atKey(vmap.first).type()) + " in " + _config;
            ETH_ERROR_MESSAGE(comment + "\n" + _o.asJson());
        }
    }
}


DataObject object::prepareGenesisParams(std::string const& _network, std::string const& _engine)
{
    ClientConfig const& cfg = Options::get().getDynamicOptions().getCurrentConfig();
    test::checkAllowedNetwork(_network, cfg.getNetworksPlusAdditional());

    DataObject genesis;
    genesis = cfg.getGenesisTemplate(_network);
    genesis["sealEngine"] = _engine;
    return genesis;
}
}
