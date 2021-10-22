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

State const& makeState()
{
    spDataObject stateData(new DataObject());
    static bool init = false;
    if (init == false)
    {
        spDataObject accountData(new DataObject());
        (*accountData)["balance"] = "0x1000";
        (*accountData)["nonce"] = "0x1000";
        (*accountData)["code"] = "0x";
        spDataObject storage(new DataObject(DataType::Object));
        (*accountData).atKeyPointer("storage") = storage;
        (*stateData).atKeyPointer("0x095e7baea6a6c7c4c2dfeb977efac326af552d87") = accountData;
        init = true;
    }
    static State state(dataobject::move(stateData));
    return state;
}

spDataObject makeTest(FORK const& _fork, VALUE const& _bn, VALUE const& _td, VALUE const& _pd, VALUE const& _un)
{
    spDataObject test(new DataObject());
    SessionInterface& session = RPCSession::instance(TestOutputHelper::getThreadID());

    spDataObject envData(new DataObject());
    (*envData)["currentCoinbase"] = "2adc25665018aa1fe0e6bc666dac8fc2697ff9ba";
    (*envData)["currentDifficulty"] = _pd.asString(); // Means parent difficulty of the first block to be mined
    (*envData)["currentGasLimit"] = "0xff112233445566";
    (*envData)["currentNumber"] = (_bn-1).asString();
    (*envData)["currentTimestamp"] = "0x00";
    (*envData)["previousHash"] = "5e20a0453cecd065ea59c37ac63e079ee08998b6045136a8ce6635c7912ec0b6";
    StateTestFillerEnv env(dataobject::move(envData));

    auto const p = prepareChainParams(_fork, SealEngine::NoReward, makeState(), env, ParamsContext::StateTests);
    session.test_setChainParams(p);
    session.test_modifyTimestamp(_td);
    session.test_mineBlocks(1);

    ETH_FAIL_MESSAGE("");

    (void)_td;
    (void)_un;

    return test;
}

void FillTest(DifficultyTestInFiller const& _test, spDataObject& _out)
{
    TestOutputHelper::get().setCurrentTestName(_test.testName());

    if (_test.hasInfo())
        (*_out).atKeyPointer("_info") = _test.info().rawData();

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
                        (*_out).atKeyPointer(testname) = makeTest(fork, bn, td, pd, un);
                    }
                }
            }
        }
    }
}

void RunTest(DifficultyTestInFilled const& _test)
{
    TestOutputHelper::get().setCurrentTestName(_test.testName());
    //SessionInterface& session = RPCSession::instance(TestOutputHelper::getThreadID());
    //for (auto const& el : Options::getCurrentConfig().cfgFile().forks())
    //{

    //}
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
            FillTest(test, filledTest);
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
