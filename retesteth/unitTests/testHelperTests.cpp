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
/** @file testHelperTest.cpp
 * Unit tests for TestHelper functions.
 */

#include <retesteth/TestHelper.h>
#include <retesteth/TestOutputHelper.h>
#include <boost/test/unit_test.hpp>

using namespace std;
using namespace dev;
using namespace test;

BOOST_FIXTURE_TEST_SUITE(TestHelperSuite, TestOutputHelperFixture)

BOOST_AUTO_TEST_CASE(translateNetworks_gtHomestead)
{
    set<string> networks = {"Frontier", ">Homestead"};
    networks = test::translateNetworks(networks);
    ETH_REQUIRE(networks.count("Frontier") > 0);
    ETH_REQUIRE(networks.count("Homestead") == 0);
    for (auto const& net : test::getNetworks())
    {
        if (net != "Frontier" && net != "Homestead")
            ETH_REQUIRE(networks.count(net) > 0);
    }
}

BOOST_AUTO_TEST_CASE(translateNetworks_geHomestead)
{
    set<string> networks = {"Frontier", ">=Homestead"};
    networks = test::translateNetworks(networks);
    for (auto const& net : test::getNetworks())
        ETH_REQUIRE(networks.count(net) > 0);
}

BOOST_AUTO_TEST_CASE(translateNetworks_ltHomestead)
{
    set<string> networks = {"<Homestead"};
    networks = test::translateNetworks(networks);
    ETH_REQUIRE(networks.count("Frontier") > 0);
    for (auto const& net : test::getNetworks())
    {
        if (net != "Frontier")
            ETH_REQUIRE(networks.count(net) == 0);
    }
}

BOOST_AUTO_TEST_CASE(translateNetworks_ltTest)
{
    set<string> networks = {"<=EIP150", "<EIP158"};
    networks = test::translateNetworks(networks);
    ETH_REQUIRE(networks.count("Frontier") > 0);
    ETH_REQUIRE(networks.count("Homestead") > 0);
    ETH_REQUIRE(networks.count("EIP150") > 0);
    ETH_REQUIRE(networks.count("EIP158") == 0);
    ETH_REQUIRE(networks.count("Byzantium") == 0);
}

BOOST_AUTO_TEST_CASE(translateNetworks_leHomestead)
{
    set<string> networks = {"<=Homestead"};
    networks = test::translateNetworks(networks);
    ETH_REQUIRE(networks.count("Frontier") > 0);
    ETH_REQUIRE(networks.count("Homestead") > 0);
    for (auto const& net : test::getNetworks())
    {
        if (net != "Frontier" && net != "Homestead")
            ETH_REQUIRE(networks.count(net) == 0);
    }
}

BOOST_AUTO_TEST_CASE(translateNetworks_leFrontier)
{
    set<string> networks = {"<=Frontier"};
    networks = test::translateNetworks(networks);
    ETH_REQUIRE(networks.count("Frontier") > 0);
    for (auto const& net : test::getNetworks())
    {
        if (net != "Frontier")
            ETH_REQUIRE(networks.count(net) == 0);
    }
}

BOOST_AUTO_TEST_SUITE_END()

