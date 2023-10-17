#include "basic.h"
#include <stdexcept>
using namespace ssz;

namespace
{
bytes decimalStringToBigIntBytes(const std::string& _input)
{
    if (_input.empty() || !std::all_of(_input.begin(), _input.end(), ::isdigit))
        throw std::invalid_argument("Expected string `" + _input + "` to be a valid non-negative decimal integer");

    bytes result;
    for (const char& digit : _input)
    {
        int carry = digit - '0';
        for (auto& byte : result)
        {
            int temp = byte * 10 + carry;
            byte = temp % 256;
            carry = temp / 256;
        }
        while (carry)
        {
            result.push_back(carry % 256);
            carry /= 256;
        }
    }

    return result;
}

std::string bigIntBytesToDecimalString(bytes const& _input)
{
    if (_input.empty())
        throw std::invalid_argument("Expected non-empty input bytes");

    std::string result;

    bytes temp = _input;
    while (!temp.empty())
    {
        int carry = 0;
        for (auto it = temp.rbegin(); it != temp.rend(); ++it)
        {
            int value = *it + carry * 256;
            *it = value / 10;
            carry = value % 10;
        }

        result.push_back(carry + '0');

        while (!temp.empty() && temp.back() == 0)
            temp.pop_back();
    }

    std::reverse(result.begin(), result.end());
    return result;
}

byte hexToByte(char hex)
{
    if (hex >= '0' && hex <= '9')
        return static_cast<byte>(hex - '0');
    else if (hex >= 'A' && hex <= 'F')
        return static_cast<byte>(hex - 'A' + 10);
    else if (hex >= 'a' && hex <= 'f')
        return static_cast<byte>(hex - 'a' + 10);
    else
        throw std::invalid_argument("Invalid hex character");
}

bytes hexStringToBytes(std::string const& _input)
{
    if (_input.size() % 2 != 0)
        throw std::invalid_argument("Invalid input length");

    bytes result((_input.size() - 2) / 2);
    for (size_t i = 2, j = 0; i < _input.size(); i += 2, j += 2)
        result[j / 2] = (hexToByte(_input[i]) << 4) | hexToByte(_input[i + 1]);

    return result;
}

}  // namespace

namespace ssz
{
bytes stringToBytes(std::string const& _input)
{
    if (_input.size() >= 2 && _input.at(0) == '0' && _input.at(1) == 'x')
        return hexStringToBytes(_input);
    return decimalStringToBigIntBytes(_input);
}

std::string bytesToString(bytes const& _input)
{
    return bigIntBytesToDecimalString(_input);
}

}  // namespace ssz


void print_bytes(const ssz::bytes& data)
{
    std::cout << "Data bytes: ";
    for (const auto& byte : data)
    {
        std::cout << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(byte) << " ";
    }
    std::cout << std::endl;
}
