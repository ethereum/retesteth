#include <retesteth/helpers/TestHelper.h>
#include <retesteth/helpers/TestOutputHelper.h>
#include <libdevcore/CommonIO.h>
#include <libdataobj/ConvertFile.h>
#include "ssz/ssz.h"

#if defined(UNITTESTS) || defined(__DEBUG__)

using namespace std;
using namespace dev;
using namespace test;
using namespace ssz;
using namespace dataobject;
namespace fs = boost::filesystem;

const dev::u256 UINT256_MAX("0xffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");

template <class Type, class DataType = string>
void streamUintString(SSZStream& _s, DataType const& _data, string const& _type, Type _max, bool& _error)
{
    auto const& testType = _type;
    if (std::is_same_v<Type, uint8_t> && testType != "uint8")
        return;
    if (std::is_same_v<Type, uint16_t> && testType != "uint16")
        return;
    if (std::is_same_v<Type, uint32_t> && testType != "uint32")
        return;
    if (std::is_same_v<Type, uint64_t> && testType != "uint64")
        return;
    if (std::is_same_v<Type, dev::u128> && testType != "uint128")
        return;
    if (std::is_same_v<Type, dev::u256> && testType != "uint256")
        return;
    auto const val = dev::bigint(_data);
    if (val > _max)
        _error = true;
    _s << (Type)val;
}

void tryBasicUints(SSZStream& _s, DataObject const& _test, bool& _error)
{
    if (_test.atKey("in").type() != DataType::String)
        return;
    auto const& type = _test.atKey("inType").asString();
    auto const& data = _test.atKey("in").asString();
    streamUintString<uint8_t>(_s, data, type, UINT8_MAX, _error);
    streamUintString<uint16_t>(_s, data, type, UINT16_MAX, _error);
    streamUintString<uint32_t>(_s, data, type, UINT32_MAX, _error);
    streamUintString<uint64_t>(_s, data, type, UINT64_MAX, _error);
    streamUintString<dev::u128>(_s, data, type, UINT128_MAX, _error);
    streamUintString<dev::u256>(_s, data, type, UINT256_MAX, _error);
}

void tryBasicVectors(SSZStream& _s, DataObject const& _test, bool& _error)
{
    auto const& inputType = _test.atKey("inType").asString();
    const size_t pos = inputType.find("vector");
    if (pos + 7 > inputType.size())
        return;
    if (pos != string::npos)
    {
        const string type = inputType.substr(pos + 7);
        for (auto const& el : _test.atKey("in").getSubObjects())
        {
            if (type == "bool")
                _s << el->asBool();
            else if (type == "uint8")
                streamUintString<uint8_t, int>(_s, el->asInt(), type, UINT8_MAX, _error);
            else if (type == "uint16")
                streamUintString<uint16_t, int>(_s, el->asInt(), type, UINT16_MAX, _error);
            else if (type == "uint32")
                streamUintString<uint32_t>(_s, el->asString(), type, UINT32_MAX, _error);
            else if (type == "uint64")
                streamUintString<uint64_t>(_s, el->asString(), type, UINT64_MAX, _error);
            else if (type == "uint128")
                streamUintString<dev::u128>(_s, el->asString(), type, UINT128_MAX, _error);
            else if (type == "uint256")
                streamUintString<dev::u256>(_s, el->asString(), type, UINT256_MAX, _error);
        }
    }
}

void readTestEncoding(SSZStream& _s, DataObject const& _test, bool& _error)
{
    tryBasicUints(_s, _test, _error);
    tryBasicVectors(_s, _test, _error);

    auto const& inputType = _test.atKey("inType").asString();
    if (inputType == "bool")
        _s << _test.atKey("in").asBool();
    if (inputType == "bitvector")
    {
        BitVector vec;
        for (auto const& el : _test.atKey("in").getSubObjects())
            vec.emplace_back(el->asBool());
        _s << vec;
    }
    if (inputType == "bitlist")
    {
        BitList lst;
        for (auto const& el : _test.atKey("in").getSubObjects())
            lst.emplace_back(el->asBool());
        _s << lst;
    }
}

void runSerializationCheck(fs::path const& _file)
{
    if (!fs::exists(_file))
        BOOST_ERROR("File is missing: " + _file.string());
    auto const s = dev::contentsString(_file);
    auto const data = ConvertJsoncppStringToData(s);
    for (auto const& test : data->getSubObjects())
    {
        bool error = false;
        std::cout << "Subtest " << test->getKey() << std::endl;
        SSZStream stream;
        readTestEncoding(stream, test, error);
        auto const res = error ? "error" : dev::toHexPrefixed(stream.data());
        auto const required = test->atKey("out").asString();
        BOOST_REQUIRE_MESSAGE(res == required, "Serialization does not match: `" + res + "` vs required: `" + required + "`");
    }
}

BOOST_FIXTURE_TEST_SUITE(SSZSuite, TestOutputHelperFixture)
BOOST_AUTO_TEST_CASE(ssz_integralTypes)
{
    runSerializationCheck(getTestPath() / "SSZTests/integralTypes.json");
}

BOOST_AUTO_TEST_CASE(ssz_uint)
{
    runSerializationCheck(getTestPath() / "SSZTests/uint/uint_valid.json");
}

BOOST_AUTO_TEST_CASE(ssz_bool)
{
    runSerializationCheck(getTestPath() / "SSZTests/bool/bool_valid.json");
}

BOOST_AUTO_TEST_CASE(ssz_vector)
{
    for (auto const& test : test::getFiles(getTestPath()/ "SSZTests/vector", {".json"}))
        runSerializationCheck(test);
}

BOOST_AUTO_TEST_CASE(ssz_bitvector)
{
    // https://www.ssz.dev/visualizer
    for (auto const& test : test::getFiles(getTestPath()/ "SSZTests/bitvector", {".json"}))
        runSerializationCheck(test);
}

BOOST_AUTO_TEST_CASE(ssz_bitlist)
{
    // https://www.ssz.dev/visualizer
    for (auto const& test : test::getFiles(getTestPath()/ "SSZTests/bitlist", {".json"}))
        runSerializationCheck(test);
}
BOOST_AUTO_TEST_SUITE_END()

#endif
