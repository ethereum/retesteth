#pragma once
#include <testeth/DataObject.h>
#include <testeth/TestOutputHelper.h>

namespace test {
	class object
	{
        public:
		object(DataObject const& _json):
			m_data(_json)
		{}
        DataObject const& getData() const { return m_data; }

        enum DigitsType
        {
            Decimal,
            Hex,
            HexPrefixed,
            String
        };

        static DigitsType stringIntegerType(std::string const& _string)
        {
            if (_string[0] == '0' && _string[1] == 'x')
            {
                DigitsType substringType = stringIntegerType(_string.substr(2));
                if (substringType == DigitsType::Hex || substringType == DigitsType::Decimal)
                    return DigitsType::HexPrefixed;
            }

            bool isDecimalOnly = true;
            static std::string hexAlphabet = "0123456789abcdef";
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

        static std::string makeHexCode(std::string const& _data)
		{
			BOOST_CHECK_MESSAGE(_data.length() % 2 == 0, TestOutputHelper::get().testName() + ": Hex data is expected to be of odd length: '" + _data + "'");
			switch (stringIntegerType(_data))
			{
				case DigitsType::HexPrefixed: return _data;
				case DigitsType::Hex: return "0x" + _data;
				case DigitsType::Decimal:
				case DigitsType::String:
					BOOST_ERROR(TestOutputHelper::get().testName() + ": Hex data is expected to be hex string: " + _data);
				default:
					BOOST_ERROR(TestOutputHelper::get().testName() + "Unknown digits type! " + _data);
					break;
			}
			return "";
        }

        static std::string makeHexAddress(std::string const& _address)
        {
            if (_address[0] == '0' && _address[1] == 'x')
				BOOST_CHECK_MESSAGE(_address.length() == 42, TestOutputHelper::get().testName() + ": Wrong address: " + _address);
            else
				BOOST_CHECK_MESSAGE(_address.length() == 40, TestOutputHelper::get().testName() + ": Wrong address: " + _address);
            return makeHexCode(_address);
        }

		protected:
        void makeKeyHex(DataObject& _key)
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
        void makeAllFieldsHex(DataObject& _data)
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
		DataObject m_data;
	};
}

