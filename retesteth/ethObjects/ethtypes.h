#pragma once
#include "dataObject/DataObject.h"

using namespace  dataobject;

namespace test {
namespace ethtypes {

enum class EthereumType
{
    eAddress,
    eFH256,
    eFH32,
    eFH8,
    eValue,
    eBytes
};

// Ethereum Types
struct eAddress
{
    eAddress(std::string const& _data) : data(_data)
    {

    }
    string data;
};

template <class T>
class DataObjectExt : DataObject
{
public:
    DataObjectExt(std::string const& _init) : m_value(_init) {}
    T const& get() { return m_value; }
private:
    T m_value;
};

//void requireJsonTypes(DataObject const& _o, std::string const& _section,
//    std::map<std::string, EthereumType> const& _validationMap, bool _fail = false);

}
}

