#include "EOFTest.h"
#include <libdataobj/ConvertFile.h>
#include <retesteth/EthChecks.h>
#include <retesteth/TestHelper.h>
#include <retesteth/TestOutputHelper.h>
#include <retesteth/testStructures/Common.h>
#include <retesteth/testStructures/types/StateTests/Filler/StateTestFillerExpectSection.h>
#include <retesteth/testSuites/TestFixtures.h>

using namespace std;
using namespace test;
using namespace dataobject;
using namespace test::teststruct;
namespace fs = boost::filesystem;

const FH20 C_CREATE_ACC("0x6295ee1b4f6dd65047762f924ecd367c17eabf8f");

namespace
{
void makeBasicSections(DataObject& _test)
{
    // Make ENV section
    _test["env"]["currentCoinbase"] = "2adc25665018aa1fe0e6bc666dac8fc2697ff9ba";
    _test["env"]["currentDifficulty"] = "0x20000";
    _test["env"]["currentGasLimit"] = "0x2233445566";
    _test["env"]["currentNumber"] = "1";
    _test["env"]["currentTimestamp"] = "1000";
    _test["env"]["previousHash"] = "5e20a0453cecd065ea59c37ac63e079ee08998b6045136a8ce6635c7912ec0b6";

    // Make Pre section
    spDataObject preState(new DataObject(DataType::Object));
    spDataObject account(new DataObject(DataType::Object));
    (*account)["balance"] = "1000000000000000000";
    (*account)["code"] = "0x";
    (*account)["nonce"] = "0";
    (*account).atKeyPointer("storage") = spDataObject(new DataObject(DataType::Object));
    (*account).setKey("a94f5374fce5edbc8e2a8697c15331677e6ebf0b");
    (*preState).addSubObject(account);
    (*preState).setKey("pre");
    _test.atKeyPointer("pre") = preState;
}

spDataObject makeTransactionWithoutData()
{
    spDataObject transaction(new DataObject(DataType::Object));
    (*transaction)["gasLimit"].addArrayObject(spDataObject(new DataObject("400000")));
    (*transaction)["gasPrice"] = "10";
    (*transaction)["nonce"] = "0";
    (*transaction)["secretKey"] = "45a915e4d060149eb4365960e6a7a45f334393093061116b197e3240065ff2d8";
    (*transaction)["to"] = "";
    (*transaction)["value"].addArrayObject(spDataObject(new DataObject("1000")));
    (*transaction).setKey("transaction");
    return transaction;
}

spDataObject makeValidResultExpect()
{
    spDataObject result;
    spDataObject account(new DataObject(DataType::Object));
    (*account)["balance"] = "1000";
    (*account).setKey(C_CREATE_ACC.asString());
    (*result).addSubObject(account);
    return result;
}

spDataObject makeInvalidResultExpect()
{
    spDataObject result;
    spDataObject account(new DataObject(DataType::Object));
    (*account)["shouldnotexist"] = "1";
    (*account).setKey(C_CREATE_ACC.asString());
    (*result).addSubObject(account);
    return result;
}

void convertEOFTestFillerToStateTestFiller(spDataObject& _input)
{
    DataObject& test = (*_input).atUnsafe(0);
    makeBasicSections(test);

    // Make Transaction section
    spDataObject transaction = makeTransactionWithoutData();
    (*transaction).atKeyPointer("data") = test.atKeyPointerUnsafe("data");
    test.removeKey("data");
    test.atKeyPointer("transaction") = transaction;

    // Make expect section
    for (auto& expect : test.atKeyUnsafe("expect").getSubObjectsUnsafe())
    {
        if (!expect->count("result") && expect->atKey("result").type() != DataType::Bool)
            throw UpwardsException("Expect section required to have 'result' field of type bool!");

        bool const valid = expect->atKey("result").asBool();
        expect.getContent().removeKey("result");
        if (valid)
            (*expect).atKeyPointer("result") = makeValidResultExpect();
        else
            (*expect).atKeyPointer("result") = makeInvalidResultExpect();
    }
}

spDataObject makeGeneratedEOFTest(DataObject& _stateTestFiller)
{
    spDataObject expectSectionArray = _stateTestFiller.atKeyPointer("expect");

    // Copy transaction section data and initialize the reader
    auto const trJsonCopy = _stateTestFiller.atKey("transaction").asJson();
    spDataObject transactionCpy = ConvertJsoncppStringToData(trJsonCopy);
    spStateTestFillerTransaction spTr(new StateTestFillerTransaction(MOVE(transactionCpy, "transaction")));

    // Copy expect section data and initialize the reader
    vector<StateTestFillerExpectSection> expects;
    expects.reserve(expectSectionArray->getSubObjects().size());
    for (auto const& expect : expectSectionArray->getSubObjects())
    {
        spDataObject expectCpy = ConvertJsoncppStringToData(expect->asJson());
        StateTestFillerExpectSection expectSecion(dataobject::move(expectCpy), spTr);
        expects.emplace_back(expectSecion);
    }

    spDataObject eofTest;
    string const testName = _stateTestFiller.getKey();

    size_t number = 0;
    for (auto const& data : spTr->databoxVector())
    {
        spDataObject testVector;
        (*testVector).setKey(testName + "_" + test::fto_string(number));
        (*testVector)["code"] = data.m_data.asString();

        for (auto const& expect : expects)
        {
            if (expect.checkIndexes(number, -1, -1))
            {
                auto const& acc = expect.result().accounts().at(C_CREATE_ACC.asString());
                bool valid = !acc->shouldNotExist();
                for (auto const& fork : expect.forks())
                {
                    spDataObject result;
                    (*result).setKey(fork.asString());
                    (*result)["result"].setBool(valid);
                    (*testVector)["results"].addSubObject(result);
                }
            }
        }

        number++;
        (*eofTest)[testName]["vectors"].addSubObject(testVector);
    }
    return eofTest;
}
}  // namespace
spDataObject EOFTestSuite::doTests(spDataObject& _input, TestSuiteOptions& _opt) const
{
    if (_opt.doFilling)
    {
        auto& test = _input.getContent().atUnsafe(0);
        convertEOFTestFillerToStateTestFiller(_input);
        auto eofTest = makeGeneratedEOFTest(test);
        StateTestSuite::doTests(_input, _opt);
        return eofTest;
    }
    return spDataObject(0);
}

TestSuite::TestPath EOFTestSuite::suiteFolder() const
{
    return TestSuite::TestPath(fs::path("EOFTests" + m_fillerPathAdd));
}

TestSuite::FillerPath EOFTestSuite::suiteFillerFolder() const
{
    return TestSuite::FillerPath(fs::path("src") / string("EOFTestsFiller" + m_fillerPathAdd));
}

using EOFTestsFixture = TestFixture<EOFTestSuite, DefaultFlags>;
ETH_REGISTER_DYNAMIC_TEST_SEARCH(EOFTestsFixture, "EOFTests")
BOOST_FIXTURE_TEST_SUITE(EOFTests, EOFTestsFixture)

BOOST_AUTO_TEST_CASE(efExample) {}

BOOST_AUTO_TEST_SUITE_END()
