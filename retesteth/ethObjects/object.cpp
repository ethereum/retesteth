#include "object.h"
#include <retesteth/TestHelper.h>
#include <mutex>

namespace test {

std::mutex g_strFindMutex;
object::DigitsType object::stringIntegerType(std::string const& _string)
{
    if (_string[0] == '0' && _string[1] == 'x')
    {
        DigitsType substringType = stringIntegerType(_string.substr(2));
        if (substringType == DigitsType::Hex || substringType == DigitsType::Decimal)
            return DigitsType::HexPrefixed;
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

    return DigitsType::Hex;
}

std::string object::makeHexAddress(std::string const& _address)
{
    if (_address[0] == '0' && _address[1] == 'x')
        ETH_CHECK_MESSAGE(_address.length() == 42, TestOutputHelper::get().testName() + ": Wrong address: '" + _address + "'");
    else
        ETH_CHECK_MESSAGE(_address.length() == 40, TestOutputHelper::get().testName() + ": Wrong address: '" + _address + "'");

	ETH_CHECK_MESSAGE(_address.length() % 2 == 0, TestOutputHelper::get().testName() + ": Hex data is expected to be of odd length: '" + _address + "'");
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

void object::makeKeyHex(DataObject& _key)
{
    if (_key.getKey() == "to" && _key.asString().empty())
        return;
    // make empty data and code fields as "0x", others as "0x00" if 0
    static std::set<std::string> empty0xFields = {"data", "code"};
    if(stringIntegerType(_key.asString()) == DigitsType::Decimal)
        _key = dev::toCompactHexPrefixed(dev::u256(_key.asString()),
                                        empty0xFields.count(_key.getKey()) ? 0 : 1);
    else if(stringIntegerType(_key.asString()) == DigitsType::Hex)
        _key = dev::toCompactHexPrefixed(dev::u256("0x" + _key.asString()),
                                        empty0xFields.count(_key.getKey()) ? 0 : 1);
}

void object::makeAllFieldsHex(DataObject& _data)
{
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
    std::map<std::string, possibleType> const& _validationMap,
    std::set<std::string> const& _ignoreFields)
{
	// check for unexpected fiedls
	for (auto const field : _o.getSubObjects())
		ETH_CHECK_MESSAGE(_validationMap.count(field.getKey()),
			field.getKey() + " should not be declared in " + _section + " section!");

	// check field types with validation map
	for (auto const vmap : _validationMap)
	{
        if (!_o.count(vmap.first) && _ignoreFields.count(vmap.first))
            continue;
        ETH_REQUIRE_MESSAGE(_o.count(vmap.first) > 0, vmap.first + " not found in " + _section +
															" section! " +
															TestOutputHelper::get().testName());
		bool matched = false;
		std::string sTypes;
		for(auto const& type: vmap.second)
		{
			if (sTypes.size())
				sTypes += ", or ";
			sTypes += DataObject::dataTypeAsString(type);
			if (_o.at(vmap.first).type() == type)
				matched = true;
		}
		if (matched == false)
		{
			BOOST_ERROR(_section + " " + vmap.first + " expected to be " + sTypes +
					", but set to " + DataObject::dataTypeAsString(_o.at(vmap.first).type()) + " in " +
					TestOutputHelper::get().testName());
		}
	}
}

DataObject object::prepareGenesisParams(std::string const& _network, std::string const& _engine)
{
    test::checkAllowedNetwork(_network);
    DataObject genesis;
    genesis["sealEngine"] = _engine;
    if (_network == "Frontier")
    {
        genesis["params"] = DataObject(DataType::Object);
    }
    else if (_network == "Homestead")
    {
        genesis["params"]["homesteadForkBlock"] = "0x00";
    }
    else if (_network == "EIP150")
    {
        genesis["params"]["homesteadForkBlock"] = "0x00";
        genesis["params"]["EIP150ForkBlock"] = "0x00";
    }
    else if (_network == "EIP158")
    {
        genesis["params"]["homesteadForkBlock"] = "0x00";
        genesis["params"]["EIP150ForkBlock"] = "0x00";
        genesis["params"]["EIP158ForkBlock"] = "0x00";
    }
    else if (_network == "Byzantium")
    {
        genesis["params"]["homesteadForkBlock"] = "0x00";
        genesis["params"]["EIP150ForkBlock"] = "0x00";
        genesis["params"]["EIP158ForkBlock"] = "0x00";
        genesis["params"]["byzantiumForkBlock"] = "0x00";
    }
    else if (_network == "Constantinople")
    {
        genesis["params"]["homesteadForkBlock"] = "0x00";
        genesis["params"]["EIP150ForkBlock"] = "0x00";
        genesis["params"]["EIP158ForkBlock"] = "0x00";
        genesis["params"]["byzantiumForkBlock"] = "0x00";
        genesis["params"]["constantinopleForkBlock"] = "0x00";
    }
    return genesis;
}
}
