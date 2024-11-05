#include <libdataobj/ConvertFile.h>
#include <libdevcore/CommonIO.h>
#include <retesteth/Options.h>
#include <retesteth/helpers/TestHelper.h>
#include <retesteth/helpers/TestOutputHelper.h>

#if defined(UNITTESTS)
#include <libssz/ssz.h>


using namespace std;
using namespace test;
using namespace ssz;
using namespace dataobject;
namespace fs = boost::filesystem;


/*void tryBasicVectors(SSZStream& _s, DataObject const& _test, bool& _error)
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
}*/

template <class T, class JsonInputType>
void stramUint(SSZStream& _s, JsonInputType _in)
{
    _s << (T)_in;
}

void testSerialize(SSZStream& _s, DataObject const& _test)
{
    auto const& testIn = _test.atKey("in");
    auto const& inputType = _test.atKey("inType").asString();

    size_t size = 0;
    const ssz::SSZType sszType = StringToSSZType(inputType, size);
    if (sszType == SSZType::UNDEFINED)
        BOOST_ERROR("Tying to parse unknow ssz type from test: " + inputType);

    switch (sszType)
    {
    case Uint8:
        stramUint<ssz::uint8, int>(_s, testIn.asInt());
        break;
    case Uint16:
        stramUint<ssz::uint16, int>(_s, testIn.asInt());
        break;
    case Uint32:
        stramUint<ssz::uint32, string>(_s, testIn.asString());
        break;
    case Uint64:
        stramUint<ssz::uint64, string>(_s, testIn.asString());
        break;
    case Uint128:
        stramUint<ssz::uint128, string>(_s, testIn.asString());
        break;
    case Uint256:
        stramUint<ssz::uint256, string>(_s, testIn.asString());
        break;
    default:
        break;
    }

    // tryBasicVectors(_s, _test, _error);


    if (inputType == "Null")
    { /* don't touch _s */
    }
    else if (inputType == "Bool")
        _s << _test.atKey("in").asBool();
    else if (inputType.find("Bitvector") != string::npos)
    {
        BitVector vec;
        for (auto const& el : _test.atKey("in").getSubObjects())
            vec.emplace_back(el->asBool());
        _s << vec;
    }
    else if (inputType.find("Bitlist") != string::npos)
    {
        BitList lst;
        for (auto const& el : _test.atKey("in").getSubObjects())
            lst.emplace_back(el->asBool());
        _s << lst;
    }
}

void runSerialization(DataObject const& _test)
{
    auto const& in = _test.atKey("in");
    if (in.type() == DataType::String && in.asString() == "error")
        return;

    SSZStream stream;
    auto const required = _test.atKey("out").asString();
    try
    {
        testSerialize(stream, _test);
        auto const res = dev::toHexPrefixed(stream.data());
        BOOST_REQUIRE_MESSAGE(
            res == required, "\nSerialization does not match: \n`" + res + "` vs required: `" + required + "`");
    }
    catch (std::exception const& _ex)
    {
        if (required != "error")
            BOOST_ERROR(string() + "Serialization cought unexpected exception: " + _ex.what());
        std::cout << _ex.what() << std::endl;
    }
}

void runDeserialization(DataObject const& _test)
{
    string const& out = _test.atKey("out").asString();
    if (out == "error")
        return;
    const ssz::bytes serialized = ssz::stringToBytes(out);
    spDataObject schema = _test.atKey("inType").copy();
    try
    {
        SSZ decoded(serialized, schema);
        BOOST_REQUIRE_MESSAGE(decoded.data() == _test.atKey("in"),
            "\nDesrialization does not match: \n" + decoded.data().asJson() + " \n vs \n" + _test.atKey("in").asJson());
    }
    catch (std::exception const& _ex)
    {
        auto const& in = _test.atKey("in");

        const bool errorExpected = in.type() == DataType::String && in.asString() == "error";
        if (!errorExpected)
            BOOST_ERROR(string() + "\nDeserialization cought unexpected exception: " + _ex.what());
        else
            std::cout << _ex.what() << std::endl;
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
        auto const& opt = Options::get();
        if (opt.singletest.initialized() && opt.singletest.name != test->getKey())
            continue;
        std::cout << "Subtest " << test->getKey() << std::endl;
        runSerialization(test);
        runDeserialization(test);
    }
}

BOOST_FIXTURE_TEST_SUITE(SSZSuite, TestOutputHelperFixture)

BOOST_AUTO_TEST_CASE(ssz_uint)
{
    for (auto const& test : test::getFiles(getTestPath() / "SSZTests/uint", {".json"}))
        runSerializationCheck(test);
}

BOOST_AUTO_TEST_CASE(ssz_bool)
{
    for (auto const& test : test::getFiles(getTestPath() / "SSZTests/bool", {".json"}))
        runSerializationCheck(test);
}

BOOST_AUTO_TEST_CASE(ssz_vector)
{
    //    for (auto const& test : test::getFiles(getTestPath()/ "SSZTests/vector", {".json"}))
    //        runSerializationCheck(test);
}

BOOST_AUTO_TEST_CASE(ssz_bitvector)
{
    // https://www.ssz.dev/visualizer
    for (auto const& test : test::getFiles(getTestPath()/ "SSZTests/bitvector", {".json"}))
        runSerializationCheck(test);
}

BOOST_AUTO_TEST_CASE(ssz_bitlist)
{
    for (auto const& test : test::getFiles(getTestPath()/ "SSZTests/bitlist", {".json"}))
        runSerializationCheck(test);
}

// https://eth2book.info/bellatrix/part2/building_blocks/ssz/
BOOST_AUTO_TEST_SUITE_END()

#endif
