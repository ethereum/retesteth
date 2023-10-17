#include "ssz.h"
#include <functional>
#include <unordered_map>
using namespace std;
using namespace dataobject;
using namespace ssz::encoding;

namespace ssz
{
SSZStream& SSZStream::operator<<(bytes const& _value)
{
    m_out.insert(m_out.end(), _value.begin(), _value.end());
    return *this;
}

inline size_t extractDigitsAsInt(string const& str)
{
    string digits;
    for (char const& c : str)
        if (isdigit(c))
            digits += c;
    return stoi(digits);
}

ssz::SSZType StringToSSZType(string const& _type, size_t& _size)
{
    static std::unordered_map<string, std::function<ssz::SSZType()>> cases = {
        {"Null", []() { return SSZType::Null; }},
        {"Bool", []() { return SSZType::Bool; }},
        {"Uint8", []() { return SSZType::Uint8; }},
        {"Uint16", []() { return SSZType::Uint16; }},
        {"Uint32", []() { return SSZType::Uint32; }},
        {"Uint64", []() { return SSZType::Uint64; }},
        {"Uint128", []() { return SSZType::Uint128; }},
        {"Uint256", []() { return SSZType::Uint256; }}};

    auto const& it = cases.find(_type);
    if (it != cases.end())
        return it->second();

    if (_type.find("Bitvector") != string::npos)
    {
        _size = extractDigitsAsInt(_type);
        return SSZType::Bitvector;
    }
    else if (_type.find("Bitlist") != string::npos)
    {
        _size = extractDigitsAsInt(_type);
        return SSZType::Bitlist;
    }

    return SSZType::UNDEFINED;
}

ssz::SSZ::SSZ(bytes const& _data, DataObject const& _scheme)
{
    size_t size = 0;
    SSZType t = SSZType::UNDEFINED;
    if (_scheme.type() == DataType::String)
        t = StringToSSZType(_scheme.asString(), size);
    if (_scheme.type() == DataType::Integer)
        t = (SSZType)_scheme.asInt();

    switch (t)
    {
    case Bitlist:
        m_out.copyFrom(sDataObject(DataType::Array));
        for (auto const& el : decodeBitList(_data, size))
            m_out.addArrayObject(sDataObject(DataType::Bool, el));
        break;
    case Bitvector:
        for (auto const& el : decodeBitVector(_data, size))
            m_out.addArrayObject(sDataObject(DataType::Bool, el));
        break;
    case Null:
        m_out.copyFrom(DataObject(DataType::Null));
        break;
    case Bool:
        if (_data.size() != 1)
            throw std::invalid_argument("Input bytes must be fixed size 1 for bool");
        if (_data.at(0) != (byte)0 && _data.at(0) != (byte)1)
            throw std::invalid_argument("Allowed input bytes for bool are 00 or 01");
        m_out.setBool(bytesToIntegral<uint8_t>(_data));
        break;
    case Uint8:
        if (_data.size() != 1)
            throw std::invalid_argument("Input bytes must be fixed size 1 for uint8");
        m_out.setInt(bytesToIntegral<uint8_t>(_data));
        break;
    case Uint16:
        if (_data.size() != 2)
            throw std::invalid_argument("Input bytes must be fixed size 2 for uint16");
        m_out.setInt(bytesToIntegral<uint16_t>(_data));
        break;
    case Uint32:
        if (_data.size() != 4)
            throw std::invalid_argument("Input bytes must be fixed size 4 for uint32");
        m_out.setString(bytesToString(_data));
        break;
    case Uint64:
        if (_data.size() != 8)
            throw std::invalid_argument("Input bytes must be fixed size 8 for uint64");
        m_out.setString(bytesToString(_data));
        break;
    case Uint128:
        if (_data.size() != 16)
            throw std::invalid_argument("Input bytes must be fixed size 16 for uint128");
        m_out.setString(bytesToString(_data));
        break;
    case Uint256:
        if (_data.size() != 32)
            throw std::invalid_argument("Input bytes must be fixed size 32 for uint256");
        m_out.setString(bytesToString(_data));
        break;
    default:
        break;
    }
}
}  // namespace ssz
