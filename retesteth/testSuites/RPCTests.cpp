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

#include <retesteth/testSuites/Common.h>
#include <retesteth/testSuites/RPCTests.h>
#include <boost/test/unit_test.hpp>

using namespace std;
using namespace test;

namespace test
{

spDataObject RPCTestSuite::doTests(spDataObject& _input, TestSuiteOptions& _opt) const
{
    (void)_input;
    (void)_opt;
    return spDataObject();
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


// FIXTURE_TEST_SUITE(RPCTests, RPCTestFixture)
// AUTO_TEST_CASE(AvailabilityTests) {}
// AUTO_TEST_SUITE_END()
