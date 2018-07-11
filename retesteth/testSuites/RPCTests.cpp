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

#include <boost/test/unit_test.hpp>
#include <retesteth/Options.h>
#include <retesteth/RPCSession.h>
#include <retesteth/TestHelper.h>
#include <retesteth/TestOutputHelper.h>
#include <retesteth/TestSuite.h>
#include <retesteth/ethObjects/common.h>

using namespace std;

class RPCTestFixture: public TestOutputHelperFixture
{
public:
    RPCTestFixture()
    {
        session = &RPCSession::instance(TestOutputHelper::getThreadID());
    }

    RPCSession *session;
};

BOOST_FIXTURE_TEST_SUITE(RPCTests, RPCTestFixture)

BOOST_AUTO_TEST_CASE(web3_clientVersion)
{
    std::cerr << session->web3_clientVersion() << std::endl;
}

BOOST_AUTO_TEST_SUITE_END()
