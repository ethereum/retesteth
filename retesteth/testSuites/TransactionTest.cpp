#include "TransactionTest.h"
#include "retesteth/testSuites/TestFixtures.h"
#include <retesteth/TestOutputHelper.h>
#include <retesteth/session/Session.h>
#include <retesteth/testStructures/types/TransactionTests/TransactionTest.h>
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
            (*result)["sender"] = res.sender().asString();
        }
        else
            (*result)["exception"] = _test.getExpectException(el);
        (*filledTest)["result"].addSubObject(result);
    }

    (*filledTest)["txbytes"] = _test.transaction()->getRawBytes().asString();
    return filledTest;
}

void RunTest(TransactionTestInFilled const& _test)
{
    TestOutputHelper::get().setCurrentTestName(_test.testName());
    SessionInterface& session = RPCSession::instance(TestOutputHelper::getThreadID());
    for (auto const& el : Options::getCurrentConfig().cfgFile().forks())
    {
        TestRawTransaction res = session.test_rawTransaction(_test.rlp(), el);
        if (_test.transaction().isEmpty())
        {
            // Fake the hash of the valid transaction to search for exception
            spTransaction tr(new TransactionLegacy(BYTES(DataObject("0xf85f800182520894000000000000000000000000000b9331677e6ebf0a801ca098ff921201554726367d2be8c804a7ff89ccf285ebc57dff8ae4c44b9c19ac4aa01887321be575c8095f789dd4c743dfe42c1820f9231f98a962b210e3ac2452a3"))));
            string const& chash = tr.getContent().hash().asStringBytes();
            string& hash = const_cast<string&>(chash);
            hash = "0x" + dev::toString(dev::sha3(fromHex(_test.rlp().asString())));
            compareTransactionException(tr, res, _test.getExpectException(el));
        }
        else
            compareTransactionException(_test.transaction(), res, _test.getExpectException(el));

        if (_test.getExpectException(el).empty())
        {
            spFH32 remoteHash = std::get<0>(_test.getAcceptedTransaction(el));
            spFH20 remoteSender = std::get<1>(_test.getAcceptedTransaction(el));
            ETH_ERROR_REQUIRE_MESSAGE(res.trhash() == remoteHash.getCContent(),
                "Remote trHash != test trHash! (`" + res.trhash().asString() + "` != `" + remoteHash->asString());
            ETH_ERROR_REQUIRE_MESSAGE(res.sender() == remoteSender.getCContent(),
                "Remote sender != test sender! (`" + res.sender().asString() + "` != `" + remoteSender->asString());
        }
    }
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
        {
            (*filledTest).addSubObject(test.testName(), FillTest(test));
            TestOutputHelper::get().registerTestRunSuccess();
        }
        return filledTest;
    }
    else
    {
        TransactionTest filledTest(_input);
        // Just check the test structure if running with --checkhash
        if (Options::get().checkhash)
            return spDataObject(new DataObject());

        for (auto const& test : filledTest.tests())
        {
            RunTest(test);
            TestOutputHelper::get().registerTestRunSuccess();
        }
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
