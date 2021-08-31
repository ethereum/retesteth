#include "TransactionTest.h"
#include "retesteth/testSuites/TestFixtures.h"
#include <retesteth/TestOutputHelper.h>
#include <retesteth/session/Session.h>
#include <retesteth/testStructures/types/TransactionTests/TransactionTestFiller.h>
#include <retesteth/testSuites/Common.h>

using namespace test;
namespace fs = boost::filesystem;
namespace
{
spDataObject FillTest(TransactionTestInFiller const& _test)
{
    spDataObject filledTest(new DataObject());
    TestOutputHelper::get().setCurrentTestName(_test.testName());

    SessionInterface& session = RPCSession::instance(TestOutputHelper::getThreadID());

    if (_test.hasInfo())
        (*filledTest).atKeyPointer("_info") = _test.info().rawData();

    for (auto const& el : Options::getCurrentConfig().cfgFile().forks())
    {
        TestRawTransaction res = session.test_rawTransaction(_test.transaction()->getRawBytes(), el);
        compareTransactionException(_test.transaction(), res, _test.getExpectException(el));

        spDataObject result(new DataObject());
        (*result).setKey(el.asString());
        if (_test.getExpectException(el).empty())
        {
            (*result)["hash"] = res.trhash().asString();
            (*result)["sender"] = res.trhash().asString();
        }
        else
            (*result)["invalid"] = "1";
        (*filledTest).addSubObject(result);
    }

    (*filledTest).atKeyPointer("transaction") = _test.transaction()->asDataObject();
    return filledTest;
}
}  // namespace

namespace test
{
spDataObject TransactionTestSuite::doTests(spDataObject& _input, TestSuiteOptions& _opt) const
{
    TestOutputHelper::get().setCurrentTestInfo(TestInfo("TransactionTestSuite::doTests init"));

    if (_opt.doFilling)
    {
        spDataObject filledTest(new DataObject());
        TransactionTestFiller filler(_input);

        for (auto const& test : filler.tests())
            (*filledTest).addSubObject(test.testName(), FillTest(test));

        TestOutputHelper::get().registerTestRunSuccess();
        return filledTest;
    }
    else
    {
        // Just check the test structure if running with --checkhash
        if (Options::get().checkhash)
            return spDataObject(new DataObject());
    }

    return spDataObject(new DataObject());
}

/// TEST SUITE ///

TestSuite::TestPath TransactionTestSuite::suiteFolder() const
{
    return TestSuite::TestPath(fs::path("TransactionTests"));
}

TestSuite::FillerPath TransactionTestSuite::suiteFillerFolder() const
{
    return TestSuite::FillerPath(fs::path("src") / "TransactionTestsFiller");
}

}  // namespace test
using TransactionTestsFixture = TestFixture<TransactionTestSuite, DefaultFlags>;
BOOST_FIXTURE_TEST_SUITE(TransactionTests, TransactionTestsFixture)

BOOST_AUTO_TEST_CASE(ttAddress) {}
BOOST_AUTO_TEST_CASE(ttEIP2028) {}
BOOST_AUTO_TEST_CASE(ttGasPrice) {}
BOOST_AUTO_TEST_CASE(ttRSValue) {}
BOOST_AUTO_TEST_CASE(ttValue) {}
BOOST_AUTO_TEST_CASE(ttWrongRLP) {}
BOOST_AUTO_TEST_CASE(ttData) {}
BOOST_AUTO_TEST_CASE(ttGasLimit) {}
BOOST_AUTO_TEST_CASE(ttNonce) {}
BOOST_AUTO_TEST_CASE(ttSignature) {}
BOOST_AUTO_TEST_CASE(ttVValue) {}

BOOST_AUTO_TEST_SUITE_END()
