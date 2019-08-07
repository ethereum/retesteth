#include "object.h"
#include "Options.h"
#include <retesteth/TestHelper.h>
#include <mutex>

namespace test {

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
    if (_key.getKey() == "to" && _key.asString().empty())
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
    test::checkAllowedNetwork(_network, cfg.getNetworks());
    bool networkExists = true;
    DataObject genesis;
    genesis["sealEngine"] = _engine;
    do {
        genesis["params"] = DataObject(DataType::Object);
        if (_network == "Frontier") break;

        genesis["params"]["homesteadForkBlock"] = "0x00";
        if (_network == "Homestead") break;

        genesis["params"]["EIP150ForkBlock"] = "0x00";
        if (_network == "EIP150") break;

        genesis["params"]["EIP158ForkBlock"] = "0x00";
        if (_network == "EIP158") break;

        genesis["params"]["byzantiumForkBlock"] = "0x00";
        if (_network == "Byzantium") break;

        genesis["params"]["constantinopleForkBlock"] = "0x00";
        if (_network == "Constantinople") break;

        genesis["params"]["constantinopleFixForkBlock"] = "0x00";
        if (_network == "ConstantinopleFix") break;

        genesis["params"]["istanbulForkBlock"] = "0x00";
        if (_network == "Istanbul") break;

        // fall through, it's not an enumerated network
        networkExists = false;
    } while (false);

    if (!networkExists)
        ETH_FAIL_MESSAGE(
            "Unhandled network: " + _network + " (DataObject object::prepareGenesisParams)");
    return genesis;
}
}
