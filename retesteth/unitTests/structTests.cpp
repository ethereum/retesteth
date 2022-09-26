#include <retesteth/EthChecks.h>
#include <retesteth/Options.h>
#include <retesteth/TestHelper.h>
#include <retesteth/TestOutputHelper.h>
#include <retesteth/testStructures/basetypes.h>
#include <retesteth/testStructures/types/Ethereum/TransactionReader.h>
#include <boost/test/unit_test.hpp>
#include <functional>

using namespace std;
using namespace dev;
using namespace test;
using namespace test::teststruct;

class Initializer : public TestOutputHelperFixture
{
public:
    Initializer()
    {
        for (auto const& config : Options::getDynamicOptions().getClientConfigs())
        {
            Options::getDynamicOptions().setCurrentConfig(config);
            break;
        }
    }
};

void checkException(std::function<void()> _job, string const& _exStr)
{
    bool exception = false;
    try
    {
        _job();
    }
    catch (std::exception const& _ex)
    {
        exception = true;
        ETH_ERROR_REQUIRE_MESSAGE(string(_ex.what()).find(_exStr) != string::npos,
            "(Exception != Expected Exception) " + string(_ex.what()) + " != " + _exStr);
    }
    ETH_ERROR_REQUIRE_MESSAGE(exception, "Expected error! `" + _exStr);
}

template <class T>
void checkSerializeBigint(T const& _a, string const& _rlpForm, string const& _expectedAfter = string())
{
    RLPStream sout(1);
    sout << _a.serializeRLP();

    auto out = sout.out();
    ETH_ERROR_REQUIRE_MESSAGE(
        toHexPrefixed(out) == _rlpForm, "RLP Serialize (out != expected) " + toHexPrefixed(out) + " != " + _rlpForm);

    size_t i = 0;
    RLP rlp(out);
    T aa(rlp[i++]);

    if (!_expectedAfter.empty())
    {
        ETH_ERROR_REQUIRE_MESSAGE(aa.asString() == _expectedAfter,
            "Var Serialize (before != after, expectedafter) " + _a.asString() + " != " + aa.asString() + ", " + _expectedAfter);
    }
    else
        ETH_ERROR_REQUIRE_MESSAGE(
            aa.asString() == _a.asString(), "Var Serialize (before != after) " + _a.asString() + " != " + aa.asString());

    // check that 0x:bigint 0x00 encode into rlp 00 and not 80
}

BOOST_FIXTURE_TEST_SUITE(StructTest, Initializer)

BOOST_AUTO_TEST_CASE(value_normal)
{
    std::vector<VALUE> tests = {
        VALUE(DataObject("0x1122")),
        VALUE(DataObject("0x01")),
        VALUE(DataObject("0x1")),
        VALUE(DataObject("0x00")),         // bigint serializes 0 value into `80` instead of `00`
        VALUE(DataObject("0x0")),
        VALUE(DataObject("0x22")),
        VALUE(DataObject("0x112233445500"))
    };

    std::vector<VALUE> testsBigint = {
        VALUE(DataObject("0x:bigint 0x1122")),
        VALUE(DataObject("0x:bigint 0x01")),
        VALUE(DataObject("0x:bigint 0x1")),
        VALUE(DataObject("0x:bigint 0x")),  // 0x is empty `80` encoding
        VALUE(DataObject("0x:bigint 0x")),  // use `0x:bigint 0x00` to actually encode `00`
        VALUE(DataObject("0x:bigint 0x22")),
        VALUE(DataObject("0x:bigint 0x112233445500"))
    };

    RLPStream sout(tests.size());
    RLPStream sout2(tests.size());
    RLPStream soutBigint(tests.size());
    size_t i = 0;
    for (auto const& el : tests)
    {
        sout << el.serializeRLP();
        sout2 << el.asBigInt();
        soutBigint << testsBigint.at(i++).serializeRLP();
    }

    auto out = sout.out();
    auto out2 = sout2.out();
    auto outbigint = soutBigint.out();
    // std::cerr << toHexPrefixed(out) << std::endl;
    ETH_ERROR_REQUIRE_MESSAGE(toHexPrefixed(out) == "0xcf821122010180802286112233445500", "RLP Serialize different to expected: `" + toHexPrefixed(out));
    ETH_ERROR_REQUIRE_MESSAGE(toHexPrefixed(out) == toHexPrefixed(out2), "RLP (serializeRLP != asBigInt) " + toHexPrefixed(out) + " != " + toHexPrefixed(out2));
    ETH_ERROR_REQUIRE_MESSAGE(toHexPrefixed(out) == toHexPrefixed(outbigint), "RLP (serializeRLP != bigint serializeRLP) " + toHexPrefixed(out) + " != " + toHexPrefixed(outbigint));
    ETH_ERROR_REQUIRE_MESSAGE(toHexPrefixed(out2) == toHexPrefixed(outbigint), "RLP (asBigInt != bigint serializeRLP) " + toHexPrefixed(out2) + " != " + toHexPrefixed(outbigint));


    i = 0;
    RLP rlp(out);
    for (auto const& el : tests)
    {
        VALUE deserialized(rlp[i++]);
        ETH_ERROR_REQUIRE_MESSAGE(deserialized.asString() == el.asString(), "Var (Deserialize != Serialize) " + deserialized.asString() + " != " + el.asString());
    }
}

BOOST_AUTO_TEST_CASE(value_notPrefixed)
{
    checkException([]() { VALUE a(DataObject("1122")); }, "is not prefixed hex");
}

BOOST_AUTO_TEST_CASE(value_leadingZero)
{
    checkException([]() { VALUE a(DataObject("0x0002")); }, "has leading 0");
    checkException([]() { VALUE a(DataObject("0x0001000000000000000000000000000000000000000000000000000000000000000001")); },
        "has leading 0");
}

BOOST_AUTO_TEST_CASE(value_wrongChar)
{
    checkException([]() { VALUE a(DataObject("0xh2")); }, "Unexpected content found while parsing character string");
}

BOOST_AUTO_TEST_CASE(value_emptyByte)
{
    checkException([]() { VALUE a(DataObject("0x")); }, "set as empty byte string");
}

BOOST_AUTO_TEST_CASE(value_emptyString)
{
    checkException([]() { VALUE a(DataObject("")); }, "element must be at least 0x prefix");
}

BOOST_AUTO_TEST_CASE(value_oversize)
{
    checkException(
        []() { VALUE a(DataObject("0xffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff")); }, ">u256");
}

//--- OVERLOADED VALUE FEAUTURES ---

BOOST_AUTO_TEST_CASE(valueb_emptyString)
{
    checkException([]() { VALUE a(DataObject("0x:bigint ")); }, "element must be at least 0x prefix");
    checkException([]() { VALUE a(DataObject("0x:bigint")); }, "Unexpected content found while parsing character string");
}

BOOST_AUTO_TEST_CASE(valueb_prefixed00)
{
    VALUE a(DataObject("0x:bigint 0x0022"));
    checkSerializeBigint(a, "0xc3820022");

    VALUE b(DataObject(" 0x:bigint 0x0001000000000000000000000000000000000000000000000000000000000000000001"));
    checkSerializeBigint(b, "0xe4a30001000000000000000000000000000000000000000000000000000000000000000001");
}

BOOST_AUTO_TEST_CASE(valueb_normal)
{
    VALUE a(DataObject("0x:bigint 0x22"));
    checkSerializeBigint(a, "0xc122", "0x22");
}

BOOST_AUTO_TEST_CASE(valueb_normal2)
{
    VALUE a(DataObject("0x:bigint 0x01"));
    checkSerializeBigint(a, "0xc101", "0x01");
}

BOOST_AUTO_TEST_CASE(valueb_empty)
{
    VALUE a(DataObject("0x:bigint 0x"));
    checkSerializeBigint(a, "0xc180", "0x00");
}

BOOST_AUTO_TEST_CASE(valueb_zero)
{
    VALUE a(DataObject("0x:bigint 0x00"));
    checkSerializeBigint(a, "0xc100", "0x00");
}

BOOST_AUTO_TEST_CASE(valueb_zeroPrefixed)
{
    VALUE a(DataObject("0x:bigint 0x0000"));
    checkSerializeBigint(a, "0xc3820000");
}

BOOST_AUTO_TEST_CASE(valueb_oversize)
{
    VALUE a(DataObject("0x:bigint 0xffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff"));
    checkSerializeBigint(a, "0xe2a1ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
}


// HASH FUNCTIONS
BOOST_AUTO_TEST_CASE(hash32)
{
    FH32 a("0x1122334455667788991011121314151617181920212223242526272829303132");
    checkSerializeBigint(a, "0xe1a01122334455667788991011121314151617181920212223242526272829303132");
    FH32 b("0x0000334455667788991011121314151617181920212223242526272829303132");
    checkSerializeBigint(b, "0xe1a00000334455667788991011121314151617181920212223242526272829303132");
    FH32 c("0x0022334455667788991011121314151617181920212223242526272829303132");
    checkSerializeBigint(c, "0xe1a00022334455667788991011121314151617181920212223242526272829303132");
}

BOOST_AUTO_TEST_CASE(hash256)
{
    FH256 a(
        "0x00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
        "0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
        "0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
        "0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
        "000000000000000000000000000000000000000000");
    checkSerializeBigint(a,
        "0xf90103b9010000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
        "0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
        "0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
        "0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
        "000000000000000000000000000000000000000000000000000000");
}

BOOST_AUTO_TEST_CASE(hash32_exceptions)
{
    checkException([]() { FH32 a("0x112233"); }, "Initializing FH32 from string that is not hash32");
    checkException([]() { FH32 a("0x12233"); }, "Initializing FH32 from string that is not hash32");
    checkException([]() { FH32 a("0x0000112233"); }, "Initializing FH32 from string that is not hash32");
    checkException([]() { FH32 a("0x112233445566778899101112131415161718192021222324252627282930313233"); },
        "Initializing FH32 from string that is not hash32");
    checkException([]() { FH32 a("112233"); }, "Initializing FH32 from string that is not hash32");
    checkException([]() { FH32 a("0x"); }, "Initializing FH32 from string that is not hash32");
    checkException([]() { FH32 a(""); }, "Initializing FH32 from string that is not hash32");
}

BOOST_AUTO_TEST_CASE(hash32_bigint)
{
    checkSerializeBigint(FH32("0x:bigint 0x112233"), "0xc483112233");
    checkSerializeBigint(FH32("0x:bigint 0x12233"), "0xc483012233", "0x:bigint 0x012233");
    checkSerializeBigint(FH32("0x:bigint 0x0000112233"), "0xc6850000112233");
    checkSerializeBigint(FH32("0x:bigint 0x112233445566778899101112131415161718192021222324252627282930313233"),
        "0xe2a1112233445566778899101112131415161718192021222324252627282930313233");
    checkSerializeBigint(FH32("0x:bigint 0x"), "0xc180");
    checkSerializeBigint(FH32("0x:bigint 0x00"), "0xc100");
}


BOOST_AUTO_TEST_CASE(hash_serialization)
{
    typedef std::tuple<FH*, string> HashType;
    std::vector<HashType> tests = {
        {new FH32(DataObject("0x1122334455667788991011121314151617181920212223242526272829303132")), "32"},
        {new FH32(DataObject("0x0022334455667788991011121314151617181920212223242526272829303132")), "32"},
        {new FH32(DataObject("0x0002334455667788991011121314151617181920212223242526272829303132")), "32"},
        {new FH32(DataObject("0x0000000000000000000000000000000000000000000000000000000000000001")), "32"},
        {new FH256(DataObject(
             "0x000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
             "00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
             "00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
             "00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
             "00000000000000000000000000000000000000000000000000000000000000")),
            "256"}};

    std::vector<HashType> testsBigint = {
        {new FH32(DataObject("0x:bigint 0x1122334455667788991011121314151617181920212223242526272829303132")), "32"},
        {new FH32(DataObject("0x:bigint 0x0022334455667788991011121314151617181920212223242526272829303132")), "32"},
        {new FH32(DataObject("0x:bigint 0x0002334455667788991011121314151617181920212223242526272829303132")), "32"},
        {new FH32(DataObject("0x:bigint 0x0000000000000000000000000000000000000000000000000000000000000001")), "32"},
        {new FH256(DataObject(
             "0x:bigint "
             "0x000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
             "00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
             "00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
             "00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
             "00000000000000000000000000000000000000000000000000000000000000")),
            "256"}};

    RLPStream sout(tests.size());
    RLPStream sout2(tests.size());
    RLPStream soutBigint(tests.size());
    size_t i = 0;
    for (auto const& el : tests)
    {
        sout << std::get<0>(el)->serializeRLP();
        sout2 << test::sfromHex(std::get<0>(el)->asString());
        soutBigint << std::get<0>(testsBigint.at(i++))->serializeRLP();
    }

    auto out = sout.out();
    auto out2 = sout2.out();
    auto outbigint = soutBigint.out();
    // std::cerr << toHexPrefixed(out) << std::endl;
    ETH_ERROR_REQUIRE_MESSAGE(
        toHexPrefixed(out) ==
            "0xf90187a01122334455667788991011121314151617181920212223242526272829303132a000223344556677889910111213141516171819"
            "20212223242526272829303132a00002334455667788991011121314151617181920212223242526272829303132a000000000000000000000"
            "00000000000000000000000000000000000000000001b901000000000000000000000000000000000000000000000000000000000000000000"
            "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
            "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
            "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
            "0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000",
        "RLP Serialize different to expected: `" + toHexPrefixed(out));
    ETH_ERROR_REQUIRE_MESSAGE(toHexPrefixed(out) == toHexPrefixed(out2),
        "RLP (serializeRLP != asBigInt) " + toHexPrefixed(out) + " != " + toHexPrefixed(out2));
    ETH_ERROR_REQUIRE_MESSAGE(toHexPrefixed(out) == toHexPrefixed(outbigint),
        "RLP (serializeRLP != bigint serializeRLP) " + toHexPrefixed(out) + " != " + toHexPrefixed(outbigint));
    ETH_ERROR_REQUIRE_MESSAGE(toHexPrefixed(out2) == toHexPrefixed(outbigint),
        "RLP (asBigInt != bigint serializeRLP) " + toHexPrefixed(out2) + " != " + toHexPrefixed(outbigint));


    i = 0;
    RLP rlp(out);
    for (auto const& el : tests)
    {
        FH* deserialized = 0;
        auto const& type = std::get<1>(el);
        if (type == "32")
            deserialized = new FH32(rlp[i]);
        else if (type == "256")
            deserialized = new FH256(rlp[i]);
        ETH_ERROR_REQUIRE_MESSAGE(deserialized->asString() == std::get<0>(el)->asString(),
            "Var (Deserialize != Serialize) " + deserialized->asString() + " != " + std::get<0>(el)->asString());
        delete deserialized;
        i++;
    }

    // delete pointers
    for (auto& el : tests)
        delete std::get<0>(el);
    for (auto& el : testsBigint)
        delete std::get<0>(el);
}


// TRANSACTIONS
BOOST_AUTO_TEST_CASE(transactionLegacy_serialization)
{
    spDataObject tr;
    (*tr)["data"] = "0x00112233";
    (*tr)["gasLimit"] = "0x112233";
    (*tr)["gasPrice"] = "0x0a";
    (*tr)["nonce"] = "0x01";
    (*tr)["secretKey"] = "0x45a915e4d060149eb4365960e6a7a45f334393093061116b197e3240065ff2d8";
    (*tr)["to"] = "";
    (*tr)["value"] = "0x11";
    spTransaction spTr = readTransaction(dataobject::move(tr));

    auto const trRead = spTr->asDataObject()->asJson(0, false);
    const string expectedRead =
        R"({"data":"0x00112233","gasLimit":"0x112233","gasPrice":"0x0a","nonce":"0x01","to":"","value":"0x11","v":"0x1c","r":"0x20705a98ccbb2eff7872ba4df9854597732e6a4436252ff7acc56ce7aeebe17a","s":"0x1869c076e616f3aeeaa160ab6050a1c009a58eda3ab6752ddb87dc6762e03112"})";
    ETH_ERROR_REQUIRE_MESSAGE(trRead == expectedRead, "Transaction read different to expected '" + trRead + "'");

    BYTES const& trSerialized = spTr->getRawBytes();
    ETH_ERROR_REQUIRE_MESSAGE(trSerialized.asString() ==
                                  "0xf850010a83112233801184001122331ca020705a98ccbb2eff7872ba4df9854597732e6a4436252ff7acc56ce7"
                                  "aeebe17aa01869c076e616f3aeeaa160ab6050a1c009a58eda3ab6752ddb87dc6762e03112",
        "Transaction rawBytes different: " + trSerialized.asString());

    spTransaction spTr2 = readTransaction(trSerialized);
    auto const trRead2 = spTr2->asDataObject()->asJson(0, false);
    ETH_ERROR_REQUIRE_MESSAGE(
        trRead == trRead2, "Transaction deserialized read different (before != after) " + trRead + " != " + trRead2);
    ETH_ERROR_REQUIRE_MESSAGE(spTr->hash() == spTr2->hash(), "Transaction deserialized hash is different (before != after) " + spTr->hash().asString() + " != " + spTr2->hash().asString())
}

BOOST_AUTO_TEST_CASE(transactionLegacy_vbigint_serialization)
{
    spDataObject tr;
    (*tr)["data"] = "0x00112233";
    (*tr)["gasLimit"] = "0x112233";
    (*tr)["gasPrice"] = "0x0a";
    (*tr)["nonce"] = "0x01";
    (*tr)["to"] = "";
    (*tr)["value"] = "0x11";
    (*tr)["v"] = "0x01000000000000001b";
    (*tr)["r"] = "0x48b55bfa915ac795c431978d8a6a992b628d557da5ff759b307d495a36649353";
    (*tr)["s"] = "0x1fffd310ac743f371de3b9f7f9cb56c0b28ad43601b4ab949f53faa07bd2c804";
    spTransaction spTr = readTransaction(dataobject::move(tr));

    auto const trRead = spTr->asDataObject()->asJson(0, false);
    const string expectedRead =
        R"({"data":"0x00112233","gasLimit":"0x112233","gasPrice":"0x0a","nonce":"0x01","to":"","value":"0x11","v":"0x01000000000000001b","r":"0x48b55bfa915ac795c431978d8a6a992b628d557da5ff759b307d495a36649353","s":"0x1fffd310ac743f371de3b9f7f9cb56c0b28ad43601b4ab949f53faa07bd2c804"})";
    ETH_ERROR_REQUIRE_MESSAGE(trRead == expectedRead, "Transaction read different to expected '" + trRead + "'");

    BYTES const& trSerialized = spTr->getRawBytes();
    ETH_ERROR_REQUIRE_MESSAGE(trSerialized.asString() ==
                                  "0xf859010a83112233801184001122338901000000000000001ba048b55bfa915ac795c431978d8a6a992b628d557da5ff759b307d495a36649353a01fffd310ac743f371de3b9f7f9cb56c0b28ad43601b4ab949f53faa07bd2c804",
        "Transaction rawBytes different: " + trSerialized.asString());

    spTransaction spTr2 = readTransaction(trSerialized);
    auto const trRead2 = spTr2->asDataObject()->asJson(0, false);
    ETH_ERROR_REQUIRE_MESSAGE(
        trRead == trRead2, "Transaction deserialized read different (before != after) " + trRead + " != " + trRead2);
    ETH_ERROR_REQUIRE_MESSAGE(spTr->hash() == spTr2->hash(), "Transaction deserialized hash is different (before != after) " + spTr->hash().asString() + " != " + spTr2->hash().asString())
}

BOOST_AUTO_TEST_SUITE_END()
