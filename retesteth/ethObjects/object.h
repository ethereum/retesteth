#pragma once
#include <dataObject/DataObject.h>
#include <retesteth/EthChecks.h>
#include <retesteth/TestOutputHelper.h>
#include <boost/test/unit_test.hpp>
//#include "ethtypes.h"

//using namespace test::ethtypes;
using namespace dataobject;

namespace test {
void ver_ethereumfields(DataObject const&);

class object
{
public:
    object(DataObject const& _json) : m_data(_json) {}
    DataObject const& getData() const { return m_data; }

    enum DigitsType
    {
        Decimal,
        Hex,
        UnEvenHex,
        HexPrefixed,
        UnEvenHexPrefixed,
        String
    };

    static string DigitsTypeToString(DigitsType _t)
    {
        switch (_t)
        {
        case DigitsType::Decimal:
            return "Decimal";
        case DigitsType::Hex:
            return "Hex";
        case DigitsType::UnEvenHex:
            return "UnEvenHex";
        case DigitsType::HexPrefixed:
            return "HexPrefixed";
        case DigitsType::UnEvenHexPrefixed:
            return "UnEvenHexPrefixed";
        case DigitsType::String:
            return "String";
        default:
            return "Unrecognized type";
        }
    }

    static std::string emptyString;
    static DigitsType stringIntegerType(std::string const& _string);
    static std::string makeHexAddress(std::string const& _address);
    static DataObject prepareGenesisParams(std::string const& _network, std::string const& _engine);
    static bool validateHash(std::string const&, size_t);

protected:
    static void makeKeyHex(DataObject&);
    static void makeAllFieldsHex(DataObject&);
    DataObject m_data;

private:
    static std::string convertStringToHexPrefixed(string const& _input, short _minimumBytes);
};


}

