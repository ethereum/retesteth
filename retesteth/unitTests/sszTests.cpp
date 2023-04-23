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

template <class type>
void streamUint(SSZStream& _s, DataObject const& _test, type _max, bool& _error)
{
    auto const& testType = _test.atKey("inType").asString();
    if (std::is_same_v<type, uint8_t> && testType != "uint8")
        return;
    if (std::is_same_v<type, uint16_t> && testType != "uint16")
        return;
    auto const val = _test.atKey("in").asInt();
    if (val > _max)
        _error = true;
    _s << (type)val;
}

template <class type>
void streamUintString(SSZStream& _s, DataObject const& _test, type _max, bool& _error)
{
    auto const& testType = _test.atKey("inType").asString();
    if (std::is_same_v<type, uint32_t> && testType != "uint32")
        return;
    if (std::is_same_v<type, uint64_t> && testType != "uint64")
        return;
    if (std::is_same_v<type, dev::u128> && testType != "uint128")
        return;
    if (std::is_same_v<type, dev::u256> && testType != "uint256")
        return;
    auto const val = dev::bigint(_test.atKey("in").asString());
    if (val > _max)
        _error = true;
    _s << (type)val;
}

void readTestEncoding(SSZStream& _s, DataObject const& _test, bool& _error)
{
    streamUint<uint8_t>(_s, _test, UINT8_MAX, _error);
    streamUint<uint16_t>(_s, _test, UINT16_MAX, _error);
    streamUintString<uint32_t>(_s, _test, UINT32_MAX, _error);
    streamUintString<uint64_t>(_s, _test, UINT64_MAX, _error);
    streamUintString<dev::u128>(_s, _test, UINT128_MAX, _error);
    streamUintString<dev::u256>(_s, _test, dev::u256("0xffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff"), _error);
    if (_test.atKey("inType").asString() == "bool")
        _s << _test.atKey("in").asBool();
    if (_test.atKey("inType").asString() == "bitvector")
    {
        BitVector vec;
        for (auto const& el : _test.atKey("in").getSubObjects())
            vec.emplace_back(el->asBool());
        _s << vec;
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

BOOST_AUTO_TEST_CASE(ssz_bitvector)
{
    // https://www.ssz.dev/visualizer
    runSerializationCheck(getTestPath() / "SSZTests/bitvector.json");
}
BOOST_AUTO_TEST_SUITE_END()

#endif
