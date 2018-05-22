#pragma once
#include <retesteth/EthChecks.h>
#include <retesteth/DataObject.h>
#include <retesteth/TestOutputHelper.h>

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

        static DigitsType stringIntegerType(std::string const& _string);
        static std::string makeHexAddress(std::string const& _address);

		protected:
		void makeKeyHex(DataObject& _key);
		void makeAllFieldsHex(DataObject& _data);
		DataObject m_data;
	};

	/// check the presents of fields in a DataObject with a validation map
	typedef std::set<DataType> possibleType;
	void requireJsonFields(DataObject const& _o, std::string const& _section, std::map<std::string, possibleType> const& _validationMap);
}

