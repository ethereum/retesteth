#include "testSuites.h"
#include <retesteth/EthChecks.h>
#include <retesteth/Options.h>
#include <retesteth/TestHelper.h>
#include <retesteth/TestOutputHelper.h>
#include <retesteth/testSuites/statetests/StateTests.h>
#include <retesteth/testStructures/types/StateTests/GeneralStateTest.h>
#include <libdataobj/ConvertFile.h>

using namespace std;
using namespace dev;
using namespace test;
using namespace test::unittests;
using namespace test::teststruct;

static std::ostringstream strCout;
std::streambuf* oldCoutStreamBuf;
std::streambuf* oldCerrStreamBuf;
void interceptOutput()
{
    strCout.str("");
    strCout.clear();
    oldCoutStreamBuf = std::cout.rdbuf();
    oldCerrStreamBuf = std::cerr.rdbuf();
    std::cout.rdbuf(strCout.rdbuf());
    std::cerr.rdbuf(strCout.rdbuf());
}
void restoreOutput()
{
    std::cout.rdbuf(oldCoutStreamBuf);
    std::cerr.rdbuf(oldCerrStreamBuf);
}

#define OPTIONS_OVERRIDE(ARGV) \
    TestOptions opt(std::size(ARGV), ARGV); \
    opt.overrideMainOptions();              \
    auto const& config = Options::getDynamicOptions().getClientConfigs().at(0);  \
    Options::getDynamicOptions().setCurrentConfig(config);

void fixInfoSection(spDataObject _test)
{
    for (auto& test : _test.getContent().getSubObjectsUnsafe())
    {
        if (test->count("_info"))
        {
            auto& info = test.getContent().atKeyUnsafe("_info");
            info["filling-rpc-server"] = "test";
            info["filling-tool-version"] = "test";
            info["generatedTestHash"] = "test";
            info["lllcversion"] = "test";
            info["solidity"] = "test";
            info["source"] = "test";
            info["sourceHash"] = "test";
        }
    }
}

enum class Mode
{
    FILL,
    RUN
};

template <class T>
spDataObject executeSample(string const& _sample, Mode _filling = Mode::RUN)
{
    T suite;
    TestSuite::TestSuiteOptions suiteOpt;
    suiteOpt.doFilling = (_filling == Mode::FILL);
    spDataObject input = dataobject::ConvertJsoncppStringToData(_sample);
    spDataObject output = suite.doTests(input, suiteOpt);
    fixInfoSection(output);
    return output;
}

BOOST_FIXTURE_TEST_SUITE(TestSuites, TestOutputHelperFixture)
#if defined(UNITTESTS) || defined(__DEBUG__)

BOOST_AUTO_TEST_CASE(fill_StateTest_multisinglenet)
{
    const char* argv[] = {"./retesteth", "--", "--singlenet", ">=Merge"};
    OPTIONS_OVERRIDE(argv);
    auto res = executeSample<StateTestSuite>(c_sampleStateTestFiller, Mode::FILL);
    auto const test = GeneralStateTest(res).tests().at(0);

    BOOST_CHECK(!test.Post().count("Berlin"));
    BOOST_CHECK(!test.Post().count("London"));
    BOOST_CHECK(test.Post().count("Merge"));
    BOOST_CHECK(test.Post().count("Shanghai"));
}

BOOST_AUTO_TEST_CASE(run_StateTest_multisinglenet)
{
    const char* argv[] = {"./retesteth", "--", "--singlenet", ">=Merge", "--verbosity", "5"};
    OPTIONS_OVERRIDE(argv);
    interceptOutput();
    executeSample<StateTestSuite>(c_sampleStateTestFilled, Mode::RUN);
    restoreOutput();

    BOOST_CHECK(strCout.str().find("fork: Berlin") == string::npos);
    BOOST_CHECK(strCout.str().find("fork: London") == string::npos);
    BOOST_CHECK(strCout.str().find("fork: Merge") != string::npos);
    BOOST_CHECK(strCout.str().find("fork: Shanghai") != string::npos);
}

#endif
BOOST_AUTO_TEST_SUITE_END()
