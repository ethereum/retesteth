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
/** @file RPCTests.cpp
 * @author Dimitry Khokhlov <dimitry@ethereum.org>
 * @date 2018
 * RPC protocol unit tests.
 */

#include <dataObject/ConvertJsoncpp.h>
#include <dataObject/DataObjectScheme.h>
#include <dataObject/DataObjectValidator.h>
#include <retesteth/Options.h>
#include <retesteth/RPCSession.h>
#include <retesteth/TestHelper.h>
#include <retesteth/TestOutputHelper.h>
#include <retesteth/ethObjects/common.h>
#include <retesteth/testSuites/Common.h>
#include <retesteth/testSuites/RPCTests.h>
#include <boost/test/unit_test.hpp>

using namespace std;

namespace test
{
/// Rewrite the test file. Fill RPC Test
DataObject FillTest(DataObject const& _testFile, TestSuite::TestSuiteOptions& _opt)
{
    DataObject filledTest;
    scheme_RPCTestFiller rpcTestFiller(_testFile);
    _opt.wasErrors = false;

    RPCSession& session = RPCSession::instance(TestOutputHelper::getThreadID());

    if (rpcTestFiller.hasGenesis())
        session.test_setChainParams(rpcTestFiller.getGenesisForRPC().asJson());

    Json::Value v = session.rpcCall(rpcTestFiller.get_method(), rpcTestFiller.get_params());
    DataObject returnedData = dataobject::ConvertJsoncppToData(v);

    DataObjectScheme scheme(rpcTestFiller.get_expectReturn());
    DataObjectValidator validator(scheme);
    ETH_ERROR_CHECK_MESSAGE(validator.validatie(returnedData), validator.getError());

    filledTest = _testFile;  // Just copy the test filler because the way RPC tests are.
    _opt.disableSecondRun = true;
    return filledTest;
}

void RunTest(DataObject const& _testFile)
{
    // Run Test logic works the same as filler. It is here to keep the test structure (fill / run
    // the test)
    TestSuite::TestSuiteOptions opt;
    opt.doFilling = false;
    FillTest(_testFile, opt);
}


DataObject RPCTestSuite::doTests(DataObject const& _input, TestSuiteOptions& _opt) const
{
    checkDataObject(_input);
    checkOnlyOneTest(_input);

    DataObject filledTest;
    DataObject const& inputTest = _input.getSubObjects().at(0);
    string const testname = inputTest.getKey();
    if (!TestOutputHelper::get().checkTest(testname))
        return filledTest;

    if (_opt.doFilling)
    {
        checkTestNameIsEqualToFileName(_input);
        DataObject outputTest;
        outputTest[testname] = FillTest(inputTest, _opt);
        filledTest = outputTest;
    }
    else
        RunTest(inputTest);
    return filledTest;
}

TestSuite::TestPath RPCTestSuite::suiteFolder() const
{
    return TestSuite::TestPath(fs::path("RPCTests"));
}

TestSuite::FillerPath RPCTestSuite::suiteFillerFolder() const
{
    return TestSuite::FillerPath(fs::path("src") / "RPCTestsFiller");
}

}  // Namespace Close


class RPCTestFixture
{
public:
    RPCTestFixture()
    {
        test::RPCTestSuite suite;
        string casename = boost::unit_test::framework::current_test_case().p_name;
        suite.runAllTestsInFolder(casename);
    }
};

BOOST_FIXTURE_TEST_SUITE(RPCTests, RPCTestFixture)

BOOST_AUTO_TEST_CASE(AvailabilityTests) {}

BOOST_AUTO_TEST_SUITE_END()

