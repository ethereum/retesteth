#include <dataObject/ConvertFile.h>
#include <retesteth/Options.h>
#include <retesteth/TestHelper.h>
#include <retesteth/TestOutputHelper.h>
#include <testStructures/types/StateTests/Filler/StateTestFillerExpectSection.h>
#include <testStructures/types/StateTests/Filler/StateTestFillerTransaction.h>

using namespace test;
using namespace dataobject;

string const transactionCommon = R"(
    "gasLimit" : ["400000"],
    "gasPrice" : "1",
    "nonce" : "0",
    "secretKey" : "45a915e4d060149eb4365960e6a7a45f334393093061116b197e3240065ff2d8",
    "to" : "095e7baea6a6c7c4c2dfeb977efac326af552d87",
    "value" : ["100000"]
)";

StateTestFillerTransaction makeTransaction(std::vector<string> const& _data)
{
    string str = "{ ";
    str += "\"data\" : [";
    for (vector<string>::const_iterator it = _data.begin(); it != _data.end(); it++)
    {
        str += "\"" + *it + "\"";
        if (it + 1 != _data.end())
            str += ", ";
    }
    str += "]," + transactionCommon + "}";
    return StateTestFillerTransaction(ConvertJsoncppStringToData(str));
}

BOOST_FIXTURE_TEST_SUITE(trDataCompileSuite, TestOutputHelperFixture)

BOOST_AUTO_TEST_CASE(compileRaw)
{
    StateTestFillerTransaction tr = makeTransaction({":raw 0x1234"});
    BOOST_CHECK(tr.asDataObject().atKey("data").at(0).asString() == "0x1234");
}

BOOST_AUTO_TEST_CASE(compileRawLabel)
{
    StateTestFillerTransaction tr = makeTransaction({":label sample :raw 0x1234"});
    BOOST_CHECK(tr.buildTransactions().at(0).transaction().dataLabel() == ":label sample");
    BOOST_CHECK(tr.asDataObject().atKey("data").at(0).asString() == "0x1234");
}

BOOST_AUTO_TEST_CASE(compileRawArray)
{
    StateTestFillerTransaction tr = makeTransaction({":raw 0x1234", ":raw 0x2244"});
    BOOST_CHECK(tr.asDataObject().atKey("data").at(0).asString() == "0x1234");
    BOOST_CHECK(tr.asDataObject().atKey("data").at(1).asString() == "0x2244");
}

BOOST_AUTO_TEST_CASE(compileRawArrayLabel)
{
    StateTestFillerTransaction tr = makeTransaction({":raw 0x1234", ":label sample :raw 0x2244"});
    BOOST_CHECK(tr.buildTransactions().at(0).transaction().dataLabel().empty());
    BOOST_CHECK(tr.buildTransactions().at(1).transaction().dataLabel() == ":label sample");
    BOOST_CHECK(tr.asDataObject().atKey("data").at(0).asString() == "0x1234");
    BOOST_CHECK(tr.asDataObject().atKey("data").at(1).asString() == "0x2244");
}


BOOST_AUTO_TEST_SUITE_END()
