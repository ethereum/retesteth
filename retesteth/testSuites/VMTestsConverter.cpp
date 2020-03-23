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

/** @file VMTestToStateConverterSuite.cpp
 * @author Dimitry Khokhlov <dimitry@ethereum.org>
 * @date 2020
 * VM Tests parser.
 */

#include <boost/filesystem/operations.hpp>
#include <boost/test/unit_test.hpp>
#include <mutex>
#include <thread>

#include <dataObject/DataObject.h>
#include <libdevcore/CommonData.h>
#include <libdevcore/CommonIO.h>
#include <retesteth/ExitHandler.h>
#include <retesteth/Options.h>
#include <retesteth/RPCSession.h>
#include <retesteth/TestHelper.h>
#include <retesteth/TestOutputHelper.h>
#include <retesteth/TestSuite.h>
#include <retesteth/ethObjects/common.h>
#include <retesteth/testSuites/Common.h>
#include <retesteth/testSuites/StateTests.h>
#include <retesteth/testSuites/VMTestsConverter.h>
#include <retesteth/testSuites/blockchain/BlockchainTests.h>

using namespace std;
using namespace dev;
namespace fs = boost::filesystem;

namespace test
{
// Most Recent StateTestSuite
TestSuite::TestPath VMTestConverterSuite::suiteFolder() const
{
    return TestSuite::TestPath(fs::path("VMTestsAsStateFillers"));
}

TestSuite::FillerPath VMTestConverterSuite::suiteFillerFolder() const
{
    return TestSuite::FillerPath(fs::path("src") / "VMTestsFiller");
}


DataObject VMTestConverterSuite::doTests(DataObject const& _input, TestSuiteOptions& _opt) const
{
    DataObject obj;
    checkAtLeastOneTest(_input);
    if (_opt.doFilling)  // convert vmTestFiller into StateTestFiller
    {
        DataObject stateFiller;
        DataObject const& vmFiller = _input.getSubObjects().at(0);
        string const& testname = vmFiller.getKey();
        TestOutputHelper::get().setCurrentTestInfo(TestInfo(testname));

        string comment;
        if (vmFiller.count("_info"))
            stateFiller["_info"] = vmFiller.atKey("_info");
        if (stateFiller["_info"].count("comment"))
            comment = stateFiller["_info"]["comment"].asString();
        stateFiller["_info"]["comment"] = "Converted from VMTest source. " + comment;

        stateFiller["env"] = vmFiller.atKey("env");
        stateFiller["pre"] = vmFiller.atKey("pre");

        string const sender = "a94f5374fce5edbc8e2a8697c15331677e6ebf0b";
        stateFiller["pre"][sender]["balance"] = "0x7ffffffffffffff0";
        stateFiller["pre"][sender]["nonce"] = "0";
        stateFiller["pre"][sender]["code"] = "";
        stateFiller["pre"][sender]["storage"] = DataObject(DataType::Object);

        stateFiller["transaction"]["data"].addArrayObject(
            DataObject(vmFiller.atKey("exec").atKey("data").asString()));
        stateFiller["transaction"]["gasLimit"].addArrayObject(
            DataObject(vmFiller.atKey("exec").atKey("gas").asString()));
        stateFiller["transaction"]["gasPrice"] = vmFiller.atKey("exec").atKey("gasPrice");
        stateFiller["transaction"]["nonce"] = "0";
        stateFiller["transaction"]["secretKey"] =
            "45a915e4d060149eb4365960e6a7a45f334393093061116b197e3240065ff2d8";
        stateFiller["transaction"]["to"] = vmFiller.atKey("exec").atKey("address");
        stateFiller["transaction"]["value"].addArrayObject(
            DataObject(vmFiller.atKey("exec").atKey("value").asString()));

        DataObject expectSection;
        expectSection["indexes"]["data"] = -1;
        expectSection["indexes"]["gas"] = -1;
        expectSection["indexes"]["value"] = -1;
        expectSection["network"].addArrayObject(DataObject(">=Istanbul"));
        if (vmFiller.count("expect"))
            expectSection["result"] = vmFiller.atKey("expect");
        else
        {
            ETH_LOG("VmTest fillere missing expect section! (" + testname + ")", 0);
            expectSection["result"] = DataObject(DataType::Object);
        }
        stateFiller["expect"].addArrayObject(expectSection);
        obj[testname] = stateFiller;

        try
        {
            StateTestSuite stateSuite;
            TestSuiteOptions stateOpt;
            stateOpt.doFilling = true;
            stateSuite.doTests(obj, stateOpt);
        }
        catch (std::exception const& _ex)
        {
            ETH_LOG("Error when filling the state test! (" + testname + "): \n" + _ex.what(), 0);
        }
    }
    return obj;
}

}  // namespace test


using VMTestsConverterFixture = TestFixture<VMTestConverterSuite, DefaultFlags>;
BOOST_FIXTURE_TEST_SUITE(VMTestConverter, VMTestsConverterFixture)

BOOST_AUTO_TEST_CASE(vmArithmeticTest) {}
BOOST_AUTO_TEST_CASE(vmBitwiseLogicOperation) {}
BOOST_AUTO_TEST_CASE(vmBlockInfoTest) {}
BOOST_AUTO_TEST_CASE(vmEnvironmentalInfo) {}
BOOST_AUTO_TEST_CASE(vmIOandFlowOperations) {}
BOOST_AUTO_TEST_CASE(vmLogTest) {}
BOOST_AUTO_TEST_CASE(vmPerformance) {}
BOOST_AUTO_TEST_CASE(vmPushDupSwapTest) {}
BOOST_AUTO_TEST_CASE(vmRandomTest) {}
BOOST_AUTO_TEST_CASE(vmSha3Test) {}
BOOST_AUTO_TEST_CASE(vmSystemOperations) {}
BOOST_AUTO_TEST_CASE(vmTests) {}

BOOST_AUTO_TEST_SUITE_END()
