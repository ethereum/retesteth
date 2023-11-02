#include <libdataobj/ConvertFile.h>
#include <retesteth/Options.h>
#include <retesteth/helpers/TestOutputHelper.h>
#include <testStructures/types/StateTests/Filler/StateTestFillerExpectSection.h>

using namespace std;
using namespace test;
using namespace dataobject;

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

string const transactionCommon = R"(
    "gasLimit" : ["400000"],
    "gasPrice" : "1",
    "nonce" : "0",
    "secretKey" : "45a915e4d060149eb4365960e6a7a45f334393093061116b197e3240065ff2d8",
    "to" : "095e7baea6a6c7c4c2dfeb977efac326af552d87",
    "value" : ["100000"]
)";

string const expectSectionCommon = R"(
    "network" : ["Frontier"],
    "result" : {
       "095e7baea6a6c7c4c2dfeb977efac326af552d87" : { "storage" : { "0x00" : "0x00" } }
    }
)";

StateTestFillerExpectSection makeExpectSection(string const& _tr, string const& _exp)
{
    spDataObject res = ConvertJsoncppStringToData(_tr);
    spStateTestFillerTransaction spTransaction =
        spStateTestFillerTransaction(new StateTestFillerTransaction(dataobject::move(res)));
    spDataObject res2 = ConvertJsoncppStringToData(_exp);
    return StateTestFillerExpectSection(dataobject::move(res2), spTransaction);
}

BOOST_FIXTURE_TEST_SUITE(ExpectSectionSuite, Initializer)

BOOST_AUTO_TEST_CASE(expectIndexesM1)
{
    const string sTr = R"(
            {
                "data" : [
                    ":label firstData :raw 0x11223344",
                    ":label secondData :raw 0x11223341"
                ],
            )" + transactionCommon + " }";

    const string sExp = R"(
        {
            "indexes" : {
                "data" : -1,
                "gas" : 0,
                "value" : -1
            }, )" + expectSectionCommon + " }";

    auto expectSection = makeExpectSection(sTr, sExp);
    BOOST_CHECK(expectSection.checkIndexes(0, 0, 0) == true);
    BOOST_CHECK(expectSection.checkIndexes(1, 0, 0) == true);
}

BOOST_AUTO_TEST_CASE(expectIndexesSingle)
{
    const string sTr = R"(
            {
                "data" : [
                    ":raw 0x11223344",
                    ":raw 0x11223341"
                ],
            )" + transactionCommon + " }";

    const string sExp = R"(
        {
            "indexes" : {
                "data" : 1,
                "gas" : 0,
                "value" : -1
            }, )" + expectSectionCommon + " }";

    auto expectSection = makeExpectSection(sTr, sExp);
    BOOST_CHECK(expectSection.checkIndexes(0, 0, 0) == false);
    BOOST_CHECK(expectSection.checkIndexes(1, 0, 0) == true);
}

BOOST_AUTO_TEST_CASE(expectIndexesArray)
{
    const string sTr = R"(
            {
                "data" : [
                    ":raw 0x11223344",
                    ":raw 0x11223341"
                ],
            )" + transactionCommon + " }";

    const string sExp = R"(
        {
            "indexes" : {
                "data" : [0,1],
                "gas" : 0,
                "value" : -1
            }, )" + expectSectionCommon + " }";

    auto expectSection = makeExpectSection(sTr, sExp);
    BOOST_CHECK(expectSection.checkIndexes(0, 0, 0) == true);
    BOOST_CHECK(expectSection.checkIndexes(1, 0, 0) == true);
}

BOOST_AUTO_TEST_CASE(expectIndexesSingleValue)
{
    const string sTr = R"(
            {
                "data" : [
                    ":raw 0x11223344",
                    ":label secondData :raw 0x11223341"
                ],
            )" + transactionCommon + " }";

    const string sExp = R"(
        {
            "indexes" : {
                "data" : ":label secondData",
                "gas" : 0,
                "value" : -1
            }, )" + expectSectionCommon + " }";

    auto expectSection = makeExpectSection(sTr, sExp);
    BOOST_CHECK(expectSection.checkIndexes(0, 0, 0) == false);
    BOOST_CHECK(expectSection.checkIndexes(1, 0, 0) == true);
}

BOOST_AUTO_TEST_CASE(expectIndexesArrayValue)
{
    const string sTr = R"(
            {
                "data" : [
                    ":raw 0x11223344",
                    ":label second :raw 0x11223341"
                ],
            )" + transactionCommon + " }";

    const string sExp = R"(
        {
            "indexes" : {
                "data" : [0, ":label second"],
                "gas" : 0,
                "value" : -1
            }, )" + expectSectionCommon + " }";

    auto expectSection = makeExpectSection(sTr, sExp);
    BOOST_CHECK(expectSection.checkIndexes(0, 0, 0) == true);
    BOOST_CHECK(expectSection.checkIndexes(1, 0, 0) == true);
}

BOOST_AUTO_TEST_CASE(expectIndexesArrayValueDouble)
{
    const string sTr = R"(
            {
                "data" : [
                    ":label first :raw 0x11223344",
                    ":raw 0x11223341"
                ],
            )" + transactionCommon + " }";

    const string sExp = R"(
        {
            "indexes" : {
                "data" : [0, ":label first"],
                "gas" : 0,
                "value" : -1
            }, )" + expectSectionCommon + " }";

    auto expectSection = makeExpectSection(sTr, sExp);
    BOOST_CHECK(expectSection.checkIndexes(0, 0, 0) == true);
    BOOST_CHECK(expectSection.checkIndexes(1, 0, 0) == false);
}

BOOST_AUTO_TEST_CASE(expectIndexesSingleValueMultiple)
{
    const string sTr = R"(
            {
                "data" : [
                    ":label second :raw 0x11223344",
                    ":label second :raw 0x11223344"
                ],
            )" + transactionCommon + " }";

    const string sExp = R"(
        {
            "indexes" : {
                "data" : ":label second",
                "gas" : 0,
                "value" : -1
            }, )" + expectSectionCommon + " }";

    auto expectSection = makeExpectSection(sTr, sExp);
    BOOST_CHECK(expectSection.checkIndexes(0, 0, 0) == true);
    BOOST_CHECK(expectSection.checkIndexes(1, 0, 0) == true);
}

BOOST_AUTO_TEST_CASE(expectIndexesArrayValueMultiple)
{
    const string sTr = R"(
            {
                "data" : [
                    ":raw 0x11223341",
                    ":label second :raw 0x11223344",
                    ":label second :raw 0x11223344"
                ],
            )" + transactionCommon + " }";

    const string sExp = R"(
        {
            "indexes" : {
                "data" : [":label second"],
                "gas" : 0,
                "value" : -1
            }, )" + expectSectionCommon + " }";

    auto expectSection = makeExpectSection(sTr, sExp);
    BOOST_CHECK(expectSection.checkIndexes(0, 0, 0) == false);
    BOOST_CHECK(expectSection.checkIndexes(1, 0, 0) == true);
    BOOST_CHECK(expectSection.checkIndexes(2, 0, 0) == true);
}

BOOST_AUTO_TEST_CASE(expectIndexesRange)
{
    const string sTr = R"(
            {
                "data" : [
                    ":raw 0x11223341",
                    ":raw 0x11223344",
                    ":raw 0x11223344"
                ],
            )" + transactionCommon + " }";

    const string sExp = R"(
        {
            "indexes" : {
                "data" : "1-2",
                "gas" : 0,
                "value" : -1
            }, )" + expectSectionCommon + " }";

    auto expectSection = makeExpectSection(sTr, sExp);
    BOOST_CHECK(expectSection.checkIndexes(0, 0, 0) == false);
    BOOST_CHECK(expectSection.checkIndexes(1, 0, 0) == true);
    BOOST_CHECK(expectSection.checkIndexes(2, 0, 0) == true);
}

BOOST_AUTO_TEST_CASE(expectIndexesRangeWrong)
{
    const string sTr = R"(
            {
                "data" : [
                    ":raw 0x11223341",
                    ":raw 0x11223344",
                    ":raw 0x11223344"
                ],
            )" + transactionCommon + " }";

    const string sExp = R"(
        {
            "indexes" : {
                "data" : "1-2-3",
                "gas" : 0,
                "value" : -1
            }, )" + expectSectionCommon + " }";

    try
    {
        auto expectSection = makeExpectSection(sTr, sExp);
        BOOST_CHECK(string("exception expected").size() == 0);
    }
    catch (std::exception const&)
    {
        TestOutputHelper::get().unmarkLastError();
    }
}

BOOST_AUTO_TEST_CASE(expectIndexesRangeArray)
{
    const string sTr = R"(
            {
                "data" : [
                    ":raw 0x11223341",
                    ":raw 0x11223344",
                    ":raw 0x11223344"
                ],
            )" + transactionCommon + " }";

    const string sExp = R"(
        {
            "indexes" : {
                "data" : ["1-3", 4, "5-5"],
                "gas" : 0,
                "value" : -1
            }, )" + expectSectionCommon + " }";

    auto expectSection = makeExpectSection(sTr, sExp);
    BOOST_CHECK(expectSection.checkIndexes(0, 0, 0) == false);
    BOOST_CHECK(expectSection.checkIndexes(1, 0, 0) == true);
    BOOST_CHECK(expectSection.checkIndexes(2, 0, 0) == true);
    BOOST_CHECK(expectSection.checkIndexes(3, 0, 0) == true);
    BOOST_CHECK(expectSection.checkIndexes(4, 0, 0) == true);
    BOOST_CHECK(expectSection.checkIndexes(5, 0, 0) == true);
    BOOST_CHECK(expectSection.checkIndexes(6, 0, 0) == false);
}

BOOST_AUTO_TEST_CASE(expectIndexesArrayNormal)
{
    const string sTr = R"(
            {
                "data" : [
                    ":raw 0x11223341",
                    ":raw 0x11223344",
                    ":raw 0x11223344"
                ],
            )" + transactionCommon + " }";

    const string sExp = R"(
        {
            "indexes" : {
                "data" : [0, 2, 3],
                "gas" : 0,
                "value" : -1
            }, )" + expectSectionCommon + " }";

    auto expectSection = makeExpectSection(sTr, sExp);
    BOOST_CHECK(expectSection.checkIndexes(0, 0, 0) == true);
    BOOST_CHECK(expectSection.checkIndexes(1, 0, 0) == false);
    BOOST_CHECK(expectSection.checkIndexes(2, 0, 0) == true);
    BOOST_CHECK(expectSection.checkIndexes(3, 0, 0) == true);
    BOOST_CHECK(expectSection.checkIndexes(4, 0, 0) == false);
}

BOOST_AUTO_TEST_CASE(expectIndexesArrayValueNotFound)
{
    const string sTr = R"(
            {
                "data" : [
                    ":raw 0x11223341",
                    ":label second :raw 0x11223344",
                    ":raw 0x11223344"
                ],
            )" + transactionCommon + " }";

    const string sExp = R"(
        {
            "indexes" : {
                "data" : [0, ":label 0x112233", 3],
                "gas" : 0,
                "value" : -1
            }, )" + expectSectionCommon + " }";

    try
    {
        auto expectSection = makeExpectSection(sTr, sExp);
        BOOST_CHECK(string("exception expected").size() == 0);
    }
    catch (std::exception const&)
    {
        TestOutputHelper::get().unmarkLastError();
    }
}

BOOST_AUTO_TEST_SUITE_END()
