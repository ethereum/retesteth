/*
This file is part of cpp-ethereum.

cpp-ethereum is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

cpp-ethereum is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with cpp-ethereum.  If not, see <http://www.gnu.org/licenses/>.
*/
/** @file BlockchainTests.cpp
 * BlockchainTests functions.
 */

#include "BlockchainTests.h"
#include <dataObject/DataObject.h>
#include <retesteth/EthChecks.h>
#include <retesteth/Options.h>
#include <retesteth/RPCSession.h>
#include <retesteth/TestOutputHelper.h>
#include <retesteth/ethObjects/common.h>
#include <retesteth/testSuites/Common.h>
#include <boost/filesystem/operations.hpp>
#include <boost/test/unit_test.hpp>
#include <thread>

using namespace std;
namespace fs = boost::filesystem;

namespace
{
/// Read and execute the test from the file
bool RunTest(DataObject const& _testObject)
{
    scheme_blockchainTest inputTest(_testObject);
    RPCSession& session = RPCSession::instance(TestOutputHelper::getThreadID());

    session.test_setChainParams(inputTest.getGenesisForRPC().asJson());

    // for all blocks
    for (auto const& brlp : inputTest.getBlockRlps())
        session.test_importRawBlock(brlp);

    // wait for blocks to process
    // std::this_thread::sleep_for(std::chrono::seconds(10));

    // compare post state hash
    DataObject remoteState = getRemoteState(session, "", true);
    scheme_state postState(remoteState.at("postState"));
    CompareResult res = test::compareStates(inputTest.getPost(), postState);
    ETH_CHECK_MESSAGE(res == CompareResult::Success, "Error in " + inputTest.getData().getKey());
    return (res != CompareResult::Success);
}

}  // namespace closed

namespace test
{
DataObject BlockchainTestSuite::doTests(DataObject const& _input, TestSuiteOptions& _opt) const
{
    DataObject tests;
    ETH_REQUIRE_MESSAGE(
        _input.type() == DataType::Object, TestOutputHelper::get().get().testFile().string() +
                                               " A BlockchainTest file should contain an object.");

    // A blockchain test file contains many tests in one .json file
    for (auto const& i : _input.getSubObjects())
    {
        string const& testname = i.getKey();
        // Select test by name if --singletest is set
        if (!Options::get().singleTestNet.empty() && Options::get().singleTest)
            if (testname != Options::get().singleTestName + "_" + Options::get().singleTestNet)
                continue;
        TestOutputHelper::get().setCurrentTestName(testname);

        if (_opt.doFilling)
        {
            /*BOOST_REQUIRE(inputTest.count("expect") > 0);
            set<eth::Network> allnetworks = ImportTest::getAllNetworksFromExpectSections(
                inputTest.at("expect").get_array(), ImportTest::testType::BlockchainTest);

            //create a blockchain test for each network
            for (auto& network : allnetworks)
            {
                if (test::isDisabledNetwork(network))
                    continue;
                if (!Options::get().singleTestNet.empty() && Options::get().singleTestNet !=
            test::netIdToString(network)) continue;

                dev::test::TestBlockChain::s_sealEngineNetwork = network;
                string newtestname = testname + "_" + test::netIdToString(network);

                json_spirit::mObject jObjOutput = inputTest;
                // prepare the corresponding expect section for the test
                json_spirit::mArray const& expects = inputTest.at("expect").get_array();
                bool found = false;

                for (auto& expect : expects)
                {
                    set<string> netlist;
                    json_spirit::mObject const& expectObj = expect.get_obj();
                    ImportTest::parseJsonStrValueIntoSet(expectObj.at("network"), netlist);

                    if (netlist.count(test::netIdToString(network)) || netlist.count("ALL"))
                    {
                        jObjOutput["expect"] = expectObj.at("result");
                        found = true;
                        break;
                    }
                }
                if (!found)
                    jObjOutput.erase(jObjOutput.find("expect"));

                TestOutputHelper::get().setCurrentTestName(newtestname);
                jObjOutput = fillBCTest(jObjOutput);
                jObjOutput["network"] = test::netIdToString(network);
                if (inputTest.count("_info"))
                    jObjOutput["_info"] = inputTest.at("_info");
                tests[newtestname] = jObjOutput;
            }*/
        }
        else
            _opt.wasErrors = RunTest(i);
    }

    return tests;
}

fs::path BlockchainTestSuite::suiteFolder() const
{
    return "BlockchainTests";
}

fs::path BlockchainTestSuite::suiteFillerFolder() const
{
    return fs::path("src") / "BlockchainTestsFiller";
}

}  // Namespace Close

class BlockchainTestFixture
{
public:
    BlockchainTestFixture()
    {
        test::BlockchainTestSuite suite;
        string casename = boost::unit_test::framework::current_test_case().p_name;
        if (casename == "stQuadraticComplexityTest" && !test::Options::get().all)
        {
            std::cout << "Skipping " << casename << " because --all option is not specified.\n";
            return;
        }
        suite.runAllTestsInFolder(casename);
    }
};

BOOST_FIXTURE_TEST_SUITE(BlockchainTests, BlockchainTestFixture)

BOOST_AUTO_TEST_CASE(bcStateTests) {}
BOOST_AUTO_TEST_CASE(bcBlockGasLimitTest) {}
BOOST_AUTO_TEST_CASE(bcGasPricerTest) {}
BOOST_AUTO_TEST_CASE(bcInvalidHeaderTest) {}
BOOST_AUTO_TEST_CASE(bcUncleHeaderValidity) {}
BOOST_AUTO_TEST_CASE(bcUncleTest) {}
BOOST_AUTO_TEST_CASE(bcValidBlockTest) {}
BOOST_AUTO_TEST_CASE(bcWalletTest) {}
BOOST_AUTO_TEST_CASE(bcTotalDifficultyTest) {}
BOOST_AUTO_TEST_CASE(bcMultiChainTest) {}
BOOST_AUTO_TEST_CASE(bcForkStressTest) {}
BOOST_AUTO_TEST_CASE(bcForgedTest) {}
BOOST_AUTO_TEST_CASE(bcRandomBlockhashTest) {}
// BOOST_AUTO_TEST_CASE(bcExploitTest){}

BOOST_AUTO_TEST_SUITE_END()
