#include "testSuites.h"
#include <retesteth/EthChecks.h>
#include <retesteth/Options.h>
#include <retesteth/helpers/TestHelper.h>
#include <retesteth/helpers/TestOutputHelper.h>
#include <retesteth/testSuites/statetests/StateTests.h>
#include <retesteth/testSuites/blockchain/BlockchainTests.h>
#include <retesteth/testStructures/types/StateTests/GeneralStateTest.h>
#include <libdataobj/ConvertFile.h>
#include <libdevcore/SHA3.h>

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
    TestOptions opt(std::size(ARGV), ARGV);                     \
    opt.overrideMainOptions();                                  \
    auto& dopt = Options::getDynamicOptions();                  \
    BOOST_CHECK(dopt.getClientConfigs().size() > 0);            \
    auto const& config = dopt.getClientConfigs().at(0);         \
    dopt.setCurrentConfig(config);

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
    try
    {
        T suite;
        TestSuite::TestSuiteOptions suiteOpt;
        suiteOpt.doFilling = (_filling == Mode::FILL);
        spDataObject input = dataobject::ConvertJsoncppStringToData(_sample);
        spDataObject output = suite.doTests(input, suiteOpt);
        fixInfoSection(output);
        return output;
    }
    catch (std::exception const& _ex) {
        BOOST_ERROR(_ex.what());
    }
    return spDataObject(0);
}

BOOST_FIXTURE_TEST_SUITE(TestSuites, TestOutputHelperFixture)
#if defined(UNITTESTS) || defined(__DEBUG__)

BOOST_AUTO_TEST_CASE(fill_StateTest_multisinglenet)
{
    const char* argv[] = {"./retesteth", "--", "--singlenet", ">=Merge", "--filltests"};
    OPTIONS_OVERRIDE(argv);

    auto res = executeSample<StateTestSuite>(c_sampleStateTestFiller, Mode::FILL);
    auto const test = GeneralStateTest(res).tests().at(0);

    BOOST_CHECK(!test.Post().count("Berlin"));
    BOOST_CHECK(!test.Post().count("London"));
    BOOST_CHECK(!test.Post().count("Berlin+1153"));
    BOOST_CHECK(test.Post().count("Merge"));
    BOOST_CHECK(test.Post().count("Shanghai"));

    auto const mergePost = test.Post().at("Merge");
    BOOST_CHECK(mergePost.size() == 1);

    spDataObject mergePostData = mergePost.at(0).asDataObject();
    BOOST_CHECK(mergePostData->count("indexes"));
    BOOST_CHECK(mergePostData->count("hash"));
    BOOST_CHECK(mergePostData->count("txbytes"));
    BOOST_CHECK(mergePostData->count("logs"));
}

BOOST_AUTO_TEST_CASE(fill_StateTest_singlenet)
{
    const char* argv[] = {"./retesteth", "--", "--singlenet", "Merge", "--filltests"};
    OPTIONS_OVERRIDE(argv);
    auto res = executeSample<StateTestSuite>(c_sampleStateTestFiller, Mode::FILL);
    BOOST_CHECK(!res.isEmpty());
    auto const test = GeneralStateTest(res).tests().at(0);

    BOOST_CHECK(!test.Post().count("Berlin"));
    BOOST_CHECK(!test.Post().count("London"));
    BOOST_CHECK(!test.Post().count("Berlin+1153"));
    BOOST_CHECK(test.Post().count("Merge"));
    BOOST_CHECK(!test.Post().count("Shanghai"));
}

BOOST_AUTO_TEST_CASE(fill_StateTest_poststate)
{
    const char* argv[] = {"./retesteth", "--", "--poststate", "--filltests"};
    OPTIONS_OVERRIDE(argv);
    interceptOutput();
    auto res = executeSample<StateTestSuite>(c_sampleStateTestFiller, Mode::FILL);
    restoreOutput();
    BOOST_CHECK(!res.isEmpty());
    auto const test = GeneralStateTest(res).tests().at(0);

    size_t expected = 5;
    if (Options::getCurrentConfig().checkForkInProgression("Cancun"))
        expected++;
    BOOST_CHECK(substrCount(strCout.str(), "State Dump") == expected);
}

BOOST_AUTO_TEST_CASE(fill_StateTest_statediff)
{
    const char* argv[] = {"./retesteth", "--", "--statediff", "--filltests", "--singlenet", "Shanghai"};
    OPTIONS_OVERRIDE(argv);
    interceptOutput();
    auto res = executeSample<StateTestSuite>(c_sampleStateTestFiller, Mode::FILL);
    restoreOutput();
    BOOST_CHECK(!res.isEmpty());
    auto const test = GeneralStateTest(res).tests().at(0);
    BOOST_CHECK(strCout.str().find("0x -> 0x02 (0x -> 2)") != string::npos);
    BOOST_CHECK(strCout.str().find("0x00 -> 0x01 (0 -> 1)") != string::npos);
    BOOST_CHECK(strCout.str().find("1000000000000000000 -> 1000000000000100000") != string::npos);
    BOOST_CHECK(strCout.str().find("1000000000000000000 -> 999999999999468880") != string::npos);
}

BOOST_AUTO_TEST_CASE(fill_BlockchainTest_poststate_wrongTx)
{
    const char* argv[] = {"./retesteth", "--", "--singlenet", "London", "--poststate", "2:10", "--filltests" };
    OPTIONS_OVERRIDE(argv);
    interceptOutput();
    executeSample<BlockchainTestValidSuite>(c_sampleBlockchainTestFiller, Mode::FILL);
    restoreOutput();
    BOOST_CHECK(strCout.str().find("State Dump") == string::npos);
}

BOOST_AUTO_TEST_CASE(fill_BlockchainTest_poststate_wrongBlock)
{
    const char* argv[] = {"./retesteth", "--", "--singlenet", "London", "--poststate", "3:0", "--filltests" };
    OPTIONS_OVERRIDE(argv);
    interceptOutput();
    executeSample<BlockchainTestValidSuite>(c_sampleBlockchainTestFiller, Mode::FILL);
    restoreOutput();
    BOOST_CHECK(strCout.str().find("State Dump") == string::npos);
}

BOOST_AUTO_TEST_CASE(fill_BlockchainTest_poststate_blockTx)
{
    const char* argv[] = {"./retesteth", "--", "--singlenet", "London", "--poststate", "2:0", "--filltests" };
    OPTIONS_OVERRIDE(argv);
    interceptOutput();
    executeSample<BlockchainTestValidSuite>(c_sampleBlockchainTestFiller, Mode::FILL);
    restoreOutput();

    // State Dump is correct for bl:tx selection
    BOOST_CHECK(strCout.str().find(R"("nonce" : "0x02")") != string::npos);
    BOOST_CHECK(strCout.str().find(R"("storage" : {
            "0x01" : "0x01",
            "0x02" : "0x01"
        })") != string::npos);

    // Only one state Dump
    size_t pos = strCout.str().find("State Dump:");
    BOOST_CHECK(pos != string::npos);
    pos = strCout.str().find("State Dump:", pos + 2);
    BOOST_CHECK(pos == string::npos);
}


BOOST_AUTO_TEST_CASE(fill_BlockchainTest_statediff_blockblock)
{
    const char* argv[] = {"./retesteth", "--", "--singlenet", "London", "--statediff", "0to1", "--filltests" };
    OPTIONS_OVERRIDE(argv);
    interceptOutput();
    executeSample<BlockchainTestValidSuite>(c_sampleBlockchainTestFiller, Mode::FILL);
    restoreOutput();

    size_t pos1 = strCout.str().find("{");
    size_t pos2 = strCout.str().rfind("}");
    BOOST_CHECK(pos1 != string::npos && pos2 != string::npos);

    auto data = dataobject::ConvertJsoncppStringToData(strCout.str().substr(pos1, pos2 - pos1 + 1));
    // {"NEW: 0x2adc25665018aa1fe0e6bc666dac8fc2697ff9ba":{"code":"0x","nonce":"0x00 (0)","balance":"0x1bc16d674f285972 (2000000000006314354)","storage":{}},"0xa94f5374fce5edbc8e2a8697c15331677e6ebf0b":{"balance":"0x016345785d8a0000 -> 0x016345785d206960 (100000000000000000 -> 99999999993080160)","nonce":"0x00 -> 0x01 (0 -> 1)"},"0xb94f5374fce5edbc8e2a8697c15331677e6ebf0b":{"storage":{"0x01":"0x -> 0x01 (0x -> 1)"}}}
    string sha3 = dev::toCompactHexPrefixed(dev::sha3(data->asJson(0, false)));
    BOOST_CHECK(sha3 == "0x68cc7add36815d3592e0bf543355ca077060b8ab2c9afcb9ce37ee271754d621");

    // Only one state Dump
    size_t pos = strCout.str().find("State Diff:");
    BOOST_CHECK(pos != string::npos);
    pos = strCout.str().find("State Diff:", pos + 2);
    BOOST_CHECK(pos == string::npos);
}

BOOST_AUTO_TEST_CASE(run_BlockchainTest_poststate_blockTx)
{
    const char* argv[] = {"./retesteth", "--", "--singlenet", "London", "--poststate", "2:0" };
    OPTIONS_OVERRIDE(argv);
    interceptOutput();
    executeSample<BlockchainTestValidSuite>(c_sampleBlockchainTestFilled, Mode::RUN);
    restoreOutput();

    // State Dump is correct for bl:tx selection
    BOOST_CHECK(strCout.str().find(R"("nonce" : "0x03")") != string::npos);
    BOOST_CHECK(strCout.str().find(R"("storage" : {
            "0x01" : "0x01",
            "0x02" : "0x01",
            "0x03" : "0x01"
        })") != string::npos);

    // Only one state Dump
    size_t pos = strCout.str().find("State Dump:");
    BOOST_CHECK(pos != string::npos);
    pos = strCout.str().find("State Dump:", pos + 2);
    BOOST_CHECK(pos == string::npos);
}

BOOST_AUTO_TEST_CASE(fill_BlockchainTest_poststate)
{
    const char* argv[] = {"./retesteth", "--", "--singlenet", "London", "--poststate", "--filltests" };
    OPTIONS_OVERRIDE(argv);
    interceptOutput();
    executeSample<BlockchainTestValidSuite>(c_sampleBlockchainTestFiller, Mode::FILL);
    restoreOutput();

    // State Dump is correct for bl:tx selection
    BOOST_CHECK(strCout.str().find(R"("nonce" : "0x04")") != string::npos);
    BOOST_CHECK(strCout.str().find(R"("storage" : {
            "0x01" : "0x01",
            "0x02" : "0x01",
            "0x03" : "0x01",
            "0x05" : "0x01"
        })") != string::npos);

    // Only one state Dump
    size_t pos = strCout.str().find("State Dump:");
    BOOST_CHECK(pos != string::npos);
    pos = strCout.str().find("State Dump:", pos + 2);
    BOOST_CHECK(pos == string::npos);
}

BOOST_AUTO_TEST_CASE(run_BlockchainTest_poststate)
{
    const char* argv[] = {"./retesteth", "--", "--singlenet", "London", "--poststate" };
    OPTIONS_OVERRIDE(argv);
    interceptOutput();
    executeSample<BlockchainTestValidSuite>(c_sampleBlockchainTestFilled, Mode::RUN);
    restoreOutput();

    // State Dump is correct for bl:tx selection
    BOOST_CHECK(strCout.str().find(R"("nonce" : "0x04")") != string::npos);
    BOOST_CHECK(strCout.str().find(R"("storage" : {
            "0x01" : "0x01",
            "0x02" : "0x01",
            "0x03" : "0x01",
            "0x05" : "0x01"
        })") != string::npos);

    // Only one state Dump
    size_t pos = strCout.str().find("State Dump:");
    BOOST_CHECK(pos != string::npos);
    pos = strCout.str().find("State Dump:", pos + 2);
    BOOST_CHECK(pos == string::npos);
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

BOOST_AUTO_TEST_CASE(run_StateTest_singlenet)
{
    const char* argv[] = {"./retesteth", "--", "--singlenet", "Merge", "--verbosity", "5"};
    OPTIONS_OVERRIDE(argv);
    interceptOutput();
    executeSample<StateTestSuite>(c_sampleStateTestFilled, Mode::RUN);
    restoreOutput();

    BOOST_CHECK(strCout.str().find("fork: Berlin") == string::npos);
    BOOST_CHECK(strCout.str().find("fork: London") == string::npos);
    BOOST_CHECK(strCout.str().find("fork: Merge") != string::npos);
    BOOST_CHECK(strCout.str().find("fork: Shanghai") == string::npos);
}

#endif
BOOST_AUTO_TEST_SUITE_END()
