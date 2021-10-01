#include <retesteth/TestHelper.h>
#include <retesteth/TestOutputHelper.h>
#include <retesteth/testStructures/basetypes.h>
#include <boost/test/unit_test.hpp>
#include <functional>

using namespace std;
using namespace dev;
using namespace test;
using namespace test::teststruct;

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

void checkSerialize(VALUE const& _a, string const& _rlpForm, string const& _expectedAfter = string())
{
    RLPStream sout(1);
    sout << _a.serializeRLP();

    auto out = sout.out();
    ETH_ERROR_REQUIRE_MESSAGE(
        toHexPrefixed(out) == _rlpForm, "RLP Serialize (out != expected) " + toHexPrefixed(out) + " != " + _rlpForm);

    size_t i = 0;
    RLP rlp(out);
    VALUE aa(rlp[i++]);

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

BOOST_FIXTURE_TEST_SUITE(StructTest, TestOutputHelperFixture)

BOOST_AUTO_TEST_CASE(value_normal)
{
    VALUE a(DataObject("0x1122"));
    VALUE b(DataObject("0x01"));
    VALUE c(DataObject("0x1"));
    VALUE d(DataObject("0x00"));
    VALUE e(DataObject("0x0"));

    RLPStream sout(5);
    sout << a.serializeRLP();
    sout << b.serializeRLP();
    sout << c.serializeRLP();
    sout << d.serializeRLP();
    sout << e.serializeRLP();

    auto out = sout.out();
    // std::cerr << toHexPrefixed(out) << std::endl;
    ETH_ERROR_REQUIRE_MESSAGE(toHexPrefixed(out) == "0xc782112201018080", "RLP Serialize");

    size_t i = 0;
    RLP rlp(out);
    VALUE aa(rlp[i++]);
    VALUE bb(rlp[i++]);
    VALUE cc(rlp[i++]);

    ETH_ERROR_REQUIRE_MESSAGE(aa.asString() == a.asString(), "Var a Serialize");
    ETH_ERROR_REQUIRE_MESSAGE(bb.asString() == b.asString(), "Var b Serialize");
    ETH_ERROR_REQUIRE_MESSAGE(cc.asString() == c.asString(), "Var c Serialize");
}

BOOST_AUTO_TEST_CASE(value_notPrefixed)
{
    checkException([]() { VALUE a(DataObject("1122")); }, "is not prefixed hex");
}

BOOST_AUTO_TEST_CASE(value_leadingZero)
{
    checkException([]() { VALUE a(DataObject("0x0002")); }, "has leading 0");
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
    checkSerialize(a, "0xc3820022");
}

BOOST_AUTO_TEST_CASE(valueb_normal)
{
    VALUE a(DataObject("0x:bigint 0x22"));
    checkSerialize(a, "0xc122", "0x22");
}

BOOST_AUTO_TEST_CASE(valueb_empty)
{
    VALUE a(DataObject("0x:bigint 0x"));
    checkSerialize(a, "0xc180", "0x00");
}

BOOST_AUTO_TEST_CASE(valueb_zero)
{
    VALUE a(DataObject("0x:bigint 0x00"));
    checkSerialize(a, "0xc100", "0x00");
}

BOOST_AUTO_TEST_CASE(valueb_zeroPrefixed)
{
    VALUE a(DataObject("0x:bigint 0x0000"));
    checkSerialize(a, "0xc3820000");
}

BOOST_AUTO_TEST_CASE(valueb_oversize)
{
    VALUE a(DataObject("0x:bigint 0xffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff"));
    checkSerialize(a, "0xe2a1ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
}


BOOST_AUTO_TEST_SUITE_END()
