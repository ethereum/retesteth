#include "DifficultyTest.h"
#include "retesteth/testSuites/TestFixtures.h"
#include <retesteth/TestOutputHelper.h>
#include <retesteth/session/Session.h>
#include <retesteth/testSuites/Common.h>
#include <retesteth/testStructures/types/DifficultyTests/DifficultyTest.h>
#include <retesteth/testStructures/types/DifficultyTests/DifficultyTestFiller.h>
#include <retesteth/testStructures/PrepareChainParams.h>

#include <retesteth/testStructures/types/StateTests/Filler/StateTestFillerEnv.h>
#include <retesteth/testStructures/types/Ethereum/State.h>

using namespace test;
namespace fs = boost::filesystem;
namespace
{

spDataObject makeTest(FORK const& _fork, VALUE const& _bn, VALUE const& _td, VALUE const& _pd, VALUE const& _un)
{
    spDataObject test(new DataObject());
    SessionInterface& session = RPCSession::instance(TestOutputHelper::getThreadID());

    VALUE const res = session.test_calculateDifficulty(_fork, _bn, 0, _pd, _td, _un);
    (*test)["network"] = _fork.asString();
    (*test)["parentTimestamp"] = "0x00";
    (*test)["parentUncles"] = _un.asString();
    (*test)["parentDifficulty"] = _pd.asString();
    (*test)["currentTimestamp"] = _td.asString();
    (*test)["currentBlockNumber"] = _bn.asString();
    (*test)["currentDifficulty"] = res.asString();
    return test;
}

spDataObject FillTest(DifficultyTestInFiller const& _test)
{
    TestOutputHelper::get().setCurrentTestName(_test.testName());
    spDataObject filledTest(new DataObject());
    if (_test.hasInfo())
        (*filledTest).atKeyPointer("_info") = _test.info().rawData();

    size_t i = 0;
    for (auto const& fork : _test.networks())
    {
        // Skip by --singlenet option
        bool networkSkip = false;
        Options const& opt = Options::get();

        if ((!opt.singleTestNet.empty() && FORK(opt.singleTestNet) != fork) ||
            !Options::getDynamicOptions().getCurrentConfig().checkForkAllowed(fork))
            networkSkip = true;

        if (networkSkip)
            continue;

        for (auto const& bn : _test.blocknumbers().vector())
        {
            for (auto const& td : _test.timestumps().vector())
            {
                for (auto const& pd : _test.parentdiffs().vector())
                {
                    for (auto const& un : _test.uncles())
                    {
                        string const testname = _test.testName() + "-" + test::fto_string(i++);
                        (*filledTest).atKeyPointer(testname) = makeTest(fork, bn, td, pd, un);
                    }
                }
            }
        }
    }
    return filledTest;
}

void RunTest(DifficultyTestInFilled const& _test)
{
    TestOutputHelper::get().setCurrentTestName(_test.testName());
    SessionInterface& session = RPCSession::instance(TestOutputHelper::getThreadID());

    for (auto const& v : _test.testVectors())
    {
        VALUE const res = session.test_calculateDifficulty(
            v.network, v.currentBlockNumber, v.parentTimestamp, v.parentDifficulty, v.currentTimestamp, v.parentUncles);
        ETH_ERROR_REQUIRE_MESSAGE(res == v.currentDifficulty, _test.testName() + "/" + v.testName +
                                                                  " difficulty mismatch got: `" + res.asDecString() +
                                                                  ", test want: `" + v.currentDifficulty->asDecString());
    }
}

}  // namespace

namespace test
{
spDataObject DifficultyTestSuite::doTests(spDataObject& _input, TestSuiteOptions& _opt) const
{
    TestOutputHelper::get().setCurrentTestInfo(TestInfo("DifficultyTestSuite::doTests init"));

    if (_opt.doFilling)
    {
        spDataObject filledTest(new DataObject());
        DifficultyTestFiller filler(_input);

        for (auto const& test : filler.tests())
        {
            (*filledTest).addSubObject(test.testName(), FillTest(test));
            TestOutputHelper::get().registerTestRunSuccess();
        }
        return filledTest;
    }
    else
    {
        DifficultyTest filledTest(_input);
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

TestSuite::TestPath DifficultyTestSuite::suiteFolder() const
{
    return TestSuite::TestPath(fs::path("DifficultyTests"));
}

TestSuite::FillerPath DifficultyTestSuite::suiteFillerFolder() const
{
    return TestSuite::FillerPath(fs::path("src") / "DifficultyTestsFiller");
}

}  // namespace test
using DifficultyTestsFixture = TestFixture<DifficultyTestSuite, DefaultFlags>;
BOOST_FIXTURE_TEST_SUITE(DifficultyTests, DifficultyTestsFixture)

BOOST_AUTO_TEST_CASE(dfExample) {}

BOOST_AUTO_TEST_SUITE_END()
