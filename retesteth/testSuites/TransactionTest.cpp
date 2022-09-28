#include "TransactionTest.h"
#include "retesteth/testSuites/TestFixtures.h"
#include <libdevcore/CommonIO.h>
#include <libdevcore/SHA3.h>
#include <retesteth/ExitHandler.h>
#include <retesteth/TestOutputHelper.h>
#include <retesteth/session/Session.h>
#include <retesteth/testStructures/types/Ethereum/TransactionLegacy.h>
#include <retesteth/testStructures/types/TransactionTests/TransactionTest.h>
#include <retesteth/testStructures/types/TransactionTests/TransactionTestFiller.h>
#include <retesteth/testSuites/Common.h>
#include <retesteth/Options.h>

using namespace std;
using namespace test;
using namespace test::session;
namespace fs = boost::filesystem;
namespace
{
spDataObject FillTest(TransactionTestInFiller const& _test)
{
    spDataObject filledTest;
    TestOutputHelper::get().setCurrentTestName(_test.testName());

    SessionInterface& session = RPCSession::instance(TestOutputHelper::getThreadID());

    if (_test.hasInfo())
        (*filledTest).atKeyPointer("_info") = _test.info().rawData();

    std::set<FORK> executionForks;
    for (auto const& fork : Options::getCurrentConfig().cfgFile().forks())
        executionForks.emplace(fork);
    for (auto const& fork : _test.additionalForks())
    {
        if (Options::getDynamicOptions().getCurrentConfig().checkForkAllowed(fork))
            executionForks.emplace(fork);
        else
            ETH_WARNING("Client config does not support fork `" + fork.asString() + "`, skipping test generation!");
    }

    for (auto const& fork : executionForks)
    {
        if (ExitHandler::receivedExitSignal())
            break;

        Options const& opt = Options::get();
        if (!opt.singleTestNet.empty() && FORK(opt.singleTestNet) != fork)
            continue;

        TestRawTransaction res = session.test_rawTransaction(_test.transaction()->getRawBytes(), fork);
        compareTransactionException(_test.transaction(), res, _test.getExpectException(fork));

        spDataObject result;
        (*result).setKey(fork.asString());
        if (_test.getExpectException(fork).empty())
        {
            (*result)["hash"] = res.trhash().asString();
            (*result)["sender"] = res.sender().asString();
        }
        else
            (*result)["exception"] = _test.getExpectException(fork);
        (*result)["intrinsicGas"] = res.intrinsicGas().asString();
        (*filledTest)["result"].addSubObject(result);
    }

    (*filledTest)["txbytes"] = _test.transaction()->getRawBytes().asString();
    return filledTest;
}

void RunTest(TransactionTestInFilled const& _test)
{
    TestOutputHelper::get().setCurrentTestName(_test.testName());
    SessionInterface& session = RPCSession::instance(TestOutputHelper::getThreadID());
    for (auto const& fork : _test.allForks())
    {
        if (ExitHandler::receivedExitSignal())
            break;

        Options const& opt = Options::get();
        if (!opt.singleTestNet.empty() && FORK(opt.singleTestNet) != fork)
            continue;

        if (!Options::getDynamicOptions().getCurrentConfig().checkForkAllowed(fork))
        {
            ETH_WARNING("Client config does not support fork `" + fork.asString() + "`, skipping test!");
            continue;
        }

        TestRawTransaction res = session.test_rawTransaction(_test.rlp(), fork);
        if (_test.transaction().isEmpty())
        {
            // Retesteth was unable to read the transaction rlp from the test into a valid transaction
            // Fake the hash of the valid transaction to search for exception. (compareTransactionException requires transaction object to print debug in case of error)
            spTransaction tr(new TransactionLegacy(BYTES(DataObject("0xf85f800182520894000000000000000000000000000b9331677e6ebf0a801ca098ff921201554726367d2be8c804a7ff89ccf285ebc57dff8ae4c44b9c19ac4aa01887321be575c8095f789dd4c743dfe42c1820f9231f98a962b210e3ac2452a3"))));
            string const& chash = tr.getContent().hash().asStringBytes();
            string& hash = const_cast<string&>(chash);
            hash = "0x" + dev::toString(dev::sha3(dev::fromHex(_test.rlp().asString())));
            compareTransactionException(tr, res, _test.getExpectException(fork));
        }
        else
        {
            // Fake the hash anyway, because of serialization issues S(0) = 80, S(D(00)) = S(0) = 80 (and not 00)
            // (compareTransactionException requires transaction object to print debug in case of error)
            spTransaction tr = _test.transaction();
            string const& chash = tr.getContent().hash().asStringBytes();
            string& hash = const_cast<string&>(chash);
            hash = "0x" + dev::toString(dev::sha3(dev::fromHex(_test.rlp().asString())));
            compareTransactionException(tr, res, _test.getExpectException(fork));
        }

        if (_test.getExpectException(fork).empty())
        {
            auto const& testResult = _test.getAcceptedTransaction(fork);
            spVALUE const& testIntrinsicGas = testResult.m_intrinsicGas;
            spFH32 const& testHash = testResult.m_hash;
            spFH20 const& testSender = testResult.m_sender;
            ETH_ERROR_REQUIRE_MESSAGE(res.trhash() == testHash.getCContent(),
                "Remote trHash != test trHash! (`" + res.trhash().asString() + "` != `" + testHash->asString());
            ETH_ERROR_REQUIRE_MESSAGE(res.sender() == testSender.getCContent(),
                "Remote sender != test sender! (`" + res.sender().asString() + "` != `" + testSender->asString());
            ETH_ERROR_REQUIRE_MESSAGE(res.intrinsicGas() == testIntrinsicGas,
                "Remote intrGas != test intrGas! (`" + res.intrinsicGas().asDecString() + "` != `" + testIntrinsicGas->asDecString());
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
        spDataObject filledTest;
        TransactionTestFiller filler(_input);

        for (auto const& test : filler.tests())
        {
            if (ExitHandler::receivedExitSignal())
                break;
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
            return spDataObject();

        for (auto const& test : filledTest.tests())
        {
            if (ExitHandler::receivedExitSignal())
                break;
            RunTest(test);
            TestOutputHelper::get().registerTestRunSuccess();
        }
    }

    return spDataObject();
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
BOOST_AUTO_TEST_CASE(ttEIP1559) {}
BOOST_AUTO_TEST_CASE(ttEIP2930) {}

BOOST_AUTO_TEST_SUITE_END()
