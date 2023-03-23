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
using namespace test::teststruct;

string const c_sampleStateTestFiller = R"({
    "add11" : {
        "_info" : {
            "comment" : "A test for (add 1 1) opcode result"
        },
        "env" : {
             "currentCoinbase" : "2adc25665018aa1fe0e6bc666dac8fc2697ff9ba",
             "currentDifficulty" : "0x20000",
             "currentGasLimit" : "0xFF112233445566",
             "currentNumber" : "1",
             "currentTimestamp" : "1000",
             "previousHash" : "5e20a0453cecd065ea59c37ac63e079ee08998b6045136a8ce6635c7912ec0b6"
        },
       "expect" : [
       {
           "indexes" : { "data" : -1, "gas" : -1, "value" : -1 },
           "network" : [">=Berlin", "Berlin+1153"],
           "result" : {
               "095e7baea6a6c7c4c2dfeb977efac326af552d87" : {
                   "code" : "0x600160010160005500",
                   "storage" : {
                       "0x00" : "0x02"
                  }
               },
               "2adc25665018aa1fe0e6bc666dac8fc2697ff9ba" : {
                   "nonce" : "1"
               },
               "a94f5374fce5edbc8e2a8697c15331677e6ebf0b" : {
                   "code" : "0x",
                   "nonce" : "1",
                   "storage" : {
                   }
               },
               "e94f5374fce5edbc8e2a8697c15331677e6ebf0b" : {
                   "shouldnotexist" : "1"
               }
           }
       }
       ],
       "pre" : {
           "2adc25665018aa1fe0e6bc666dac8fc2697ff9ba" : {
               "balance" : "0",
               "code" : "0x",
               "nonce" : "1",
               "storage" : {
               }
           },
           "095e7baea6a6c7c4c2dfeb977efac326af552d87" : {
               "balance" : "1000000000000000000",
               "code" : "0x600160010160005500",
               "code" : "{ [[0]] (ADD 1 1) }",
               "nonce" : "0",
               "storage" : {
               }
           },
           "a94f5374fce5edbc8e2a8697c15331677e6ebf0b" : {
               "balance" : "1000000000000000000",
               "code" : "0x",
               "nonce" : "0",
               "storage" : {
               }
           }
       },
       "transaction" : {
          "data" : [
               ""
           ],
          "gasLimit" : [
               "400000"
          ],
         "gasPrice" : "10",
         "nonce" : "0",
         "secretKey" : "45a915e4d060149eb4365960e6a7a45f334393093061116b197e3240065ff2d8",
         "to" : "095e7baea6a6c7c4c2dfeb977efac326af552d87",
         "value" : [
             "100000"
         ]
       }
  }
})";

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

BOOST_FIXTURE_TEST_SUITE(TestSuites, TestOutputHelperFixture)
#if defined(UNITTESTS) || defined(__DEBUG__)

BOOST_AUTO_TEST_CASE(fillStateTest)
{
    const char* argv[] = {"./retesteth", "--", "--clients", "default", "--singlenet", ">=Merge"};
    OPTIONS_OVERRIDE(argv);

    StateTestSuite state;
    TestSuite::TestSuiteOptions suiteOpt;
    suiteOpt.doFilling = true;
    spDataObject input = dataobject::ConvertJsoncppStringToData(c_sampleStateTestFiller);
    spDataObject output = state.doTests(input, suiteOpt);
    fixInfoSection(output);
    GeneralStateTest filledTest(output);
    auto const& test = filledTest.tests().at(0);

    BOOST_CHECK(!test.Post().count("Berlin"));
    BOOST_CHECK(!test.Post().count("London"));
    BOOST_CHECK(test.Post().count("Merge"));
    BOOST_CHECK(test.Post().count("Shanghai"));
}

#endif
BOOST_AUTO_TEST_SUITE_END()
