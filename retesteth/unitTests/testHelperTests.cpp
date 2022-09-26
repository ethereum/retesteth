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

#include <libdevcore/CommonIO.h>
#include <retesteth/EthChecks.h>
#include <retesteth/Options.h>
#include <retesteth/TestHelper.h>
#include <retesteth/TestOutputHelper.h>
#include <retesteth/configs/ClientConfig.h>
#include <boost/test/unit_test.hpp>

using namespace std;
using namespace dev;
using namespace test;

namespace
{
bool hasNetwork(std::vector<FORK> const& _container, FORK const& _net)
{
    for (auto const& el : _container)
        if (el == _net)
            return true;
    return false;
}
static vector<FORK> exampleNets = {FORK("Frontier"), FORK("Homestead"), FORK("EIP150"), FORK("EIP158"), FORK("Byzantium"),
    FORK("Constantinople"), FORK("ConstantinopleFix")};

class Initializer : public TestOutputHelperFixture
{
public:
    Initializer()
    {
        for (auto const& config : Options::getDynamicOptions().getClientConfigs())
        {
            Options::getDynamicOptions().setCurrentConfig(config);
            vector<FORK> const& forks = Options::getCurrentConfig().cfgFile().forks();
            vector<FORK>& forksCheat = const_cast<vector<FORK>&>(forks);
            forksCheat = exampleNets;
            break;
        }
    }
};

}  // namespace

BOOST_FIXTURE_TEST_SUITE(TestHelperSuite, Initializer)

BOOST_AUTO_TEST_CASE(tostr_std)
{
    for (size_t i = 0; i < 1000000; i++)
        std::to_string(i);
}

BOOST_AUTO_TEST_CASE(tostr_dev)
{
    for (size_t i = 0; i < 1000000; i++)
        dev::toString(i);
}

BOOST_AUTO_TEST_CASE(comapreForks_lotsSteps_Bajo)
{
    ETH_FAIL_REQUIRE(
    compareFork(FORK("Homestead"), CMP::lt, FORK("Constantinople")) == true);

    ETH_FAIL_REQUIRE(
    compareFork(FORK("Homestead"), CMP::gt, FORK("Constantinople")) == false);

    ETH_FAIL_REQUIRE(
    compareFork(FORK("Homestead"), CMP::le, FORK("Constantinople")) == true);

    ETH_FAIL_REQUIRE(
    compareFork(FORK("Homestead"), CMP::ge, FORK("Constantinople")) == false);
}

BOOST_AUTO_TEST_CASE(comapreForks_OneStep_Bajo)
{
    ETH_FAIL_REQUIRE(
    compareFork(FORK("Homestead"), CMP::lt, FORK("EIP150")) == true);

    ETH_FAIL_REQUIRE(
    compareFork(FORK("Homestead"), CMP::gt, FORK("EIP150")) == false);

    ETH_FAIL_REQUIRE(
    compareFork(FORK("Homestead"), CMP::le, FORK("EIP150")) == true);

    ETH_FAIL_REQUIRE(
    compareFork(FORK("Homestead"), CMP::ge, FORK("EIP150")) == false);
}

BOOST_AUTO_TEST_CASE(comapreForks_lotsSteps_Arriba)
{
    ETH_FAIL_REQUIRE(
    compareFork(FORK("Constantinople"), CMP::lt, FORK("Homestead")) == false);

    ETH_FAIL_REQUIRE(
    compareFork(FORK("Constantinople"), CMP::gt, FORK("Homestead")) == true);

    ETH_FAIL_REQUIRE(
    compareFork(FORK("Constantinople"), CMP::le, FORK("Homestead")) == false);

    ETH_FAIL_REQUIRE(
    compareFork(FORK("Constantinople"), CMP::ge, FORK("Homestead")) == true);
}

BOOST_AUTO_TEST_CASE(comapreForks_OneStep_Arriba)
{
    ETH_FAIL_REQUIRE(
    compareFork(FORK("EIP150"), CMP::lt, FORK("Homestead")) == false);

    ETH_FAIL_REQUIRE(
    compareFork(FORK("EIP150"), CMP::gt, FORK("Homestead")) == true);

    ETH_FAIL_REQUIRE(
    compareFork(FORK("EIP150"), CMP::le, FORK("Homestead")) == false);

    ETH_FAIL_REQUIRE(
    compareFork(FORK("EIP150"), CMP::ge, FORK("Homestead")) == true);
}

BOOST_AUTO_TEST_CASE(comapreForks_ZeroStep)
{
    ETH_FAIL_REQUIRE(
    compareFork(FORK("Homestead"), CMP::lt, FORK("Homestead")) == false);

    ETH_FAIL_REQUIRE(
    compareFork(FORK("Homestead"), CMP::gt, FORK("Homestead")) == false);

    ETH_FAIL_REQUIRE(
    compareFork(FORK("Homestead"), CMP::le, FORK("Homestead")) == true);

    ETH_FAIL_REQUIRE(
    compareFork(FORK("Homestead"), CMP::ge, FORK("Homestead")) == true);
}

BOOST_AUTO_TEST_CASE(translateNetworks_doubleNet)
{
    set<string> rawnetworks = {"Frontier", "<Homestead"};
    std::vector<FORK> networks;
    ClientConfig::translateNetworks(rawnetworks, exampleNets, networks);
    ETH_FAIL_REQUIRE(hasNetwork(networks, FORK("Frontier")));
    ETH_FAIL_REQUIRE(hasNetwork(networks, FORK("Homestead")) == false);
    ETH_FAIL_REQUIRE(networks.size() == 1);
}

BOOST_AUTO_TEST_CASE(translateNetworks_london)
{
    set<string> rawnetworks = {">=London"};
    std::vector<FORK> exampleNets2 = {FORK("Frontier"), FORK("London")};
    std::vector<FORK> networks;
    ClientConfig::translateNetworks(rawnetworks, exampleNets2, networks);
    ETH_FAIL_REQUIRE(hasNetwork(networks, FORK("London")));
    ETH_FAIL_REQUIRE(networks.size() == 1);
}

BOOST_AUTO_TEST_CASE(translateNetworks_gtHomestead)
{
    set<string> rawnetworks = {"Frontier", ">Homestead"};
    std::vector<FORK> networks;
    ClientConfig::translateNetworks(rawnetworks, exampleNets, networks);
    ETH_FAIL_REQUIRE(hasNetwork(networks, FORK("Frontier")));
    ETH_FAIL_REQUIRE(hasNetwork(networks, FORK("Homestead")) == false);
    for (auto const& net : exampleNets)
    {
        if (net != "Frontier" && net != "Homestead")
            ETH_FAIL_REQUIRE(hasNetwork(networks, net));
    }
}

BOOST_AUTO_TEST_CASE(translateNetworks_geHomestead)
{
    set<string> rawnetworks = {"Frontier", ">=Homestead"};
    std::vector<FORK> networks;
    ClientConfig::translateNetworks(rawnetworks, exampleNets, networks);
    for (auto const& net : exampleNets)
        ETH_FAIL_REQUIRE(hasNetwork(networks, net));
}

BOOST_AUTO_TEST_CASE(translateNetworks_ltHomestead)
{
    set<string> rawnetworks = {"<Homestead"};
    std::vector<FORK> networks;
    ClientConfig::translateNetworks(rawnetworks, exampleNets, networks);
    ETH_FAIL_REQUIRE(hasNetwork(networks, FORK("Frontier")));
    for (auto const& net : exampleNets)
    {
        if (net != "Frontier")
            ETH_FAIL_REQUIRE(hasNetwork(networks, net) == false);
    }
}

BOOST_AUTO_TEST_CASE(translateNetworks_ltTest)
{
    set<string> rawnetworks = {"<=EIP150", "<EIP158"};
    std::vector<FORK> networks;
    ClientConfig::translateNetworks(rawnetworks, exampleNets, networks);
    ETH_FAIL_REQUIRE(hasNetwork(networks, FORK("Frontier")));
    ETH_FAIL_REQUIRE(hasNetwork(networks, FORK("Homestead")));
    ETH_FAIL_REQUIRE(hasNetwork(networks, FORK("EIP150")));
    ETH_FAIL_REQUIRE(hasNetwork(networks, FORK("EIP158")) == false);
    ETH_FAIL_REQUIRE(hasNetwork(networks, FORK("Byzantium")) == false);
}

BOOST_AUTO_TEST_CASE(translateNetworks_leHomestead)
{
    set<string> rawnetworks = {"<=Homestead"};
    std::vector<FORK> networks;
    ClientConfig::translateNetworks(rawnetworks, exampleNets, networks);
    ETH_FAIL_REQUIRE(hasNetwork(networks, FORK("Frontier")));
    ETH_FAIL_REQUIRE(hasNetwork(networks, FORK("Homestead")));
    for (auto const& net : exampleNets)
    {
        if (net != "Frontier" && net != "Homestead")
            ETH_FAIL_REQUIRE(hasNetwork(networks, net) == false);
    }
}

BOOST_AUTO_TEST_CASE(translateNetworks_leFrontier)
{
    set<string> rawnetworks = {"<=Frontier"};
    std::vector<FORK> networks;
    ClientConfig::translateNetworks(rawnetworks, exampleNets, networks);
    ETH_FAIL_REQUIRE(hasNetwork(networks, FORK("Frontier")));
    for (auto const& net : exampleNets)
    {
        if (net != "Frontier")
            ETH_FAIL_REQUIRE(hasNetwork(networks, net) == false);
    }
}

BOOST_AUTO_TEST_CASE(getTestSuggestions)
{
    vector<string> const testList = {
        "test1", "test2", "BlockSuite", "BlockSuite/TestCase", "GeneralBlockchainTests"};
    auto list = test::levenshteinDistance("blocksuit", testList);
    BOOST_CHECK(test::inArray(list, string("BlockSuite")));
}

BOOST_AUTO_TEST_CASE(getTestSuggestions2)
{
    vector<string> const testList = {"test1", "test2", "BlockSuite", "BlockSuite/TestCase",
        "GeneralBlockchainTests", "GeneralStateTests/stExample", "BCGeneralStateTests/stExample"};

    auto list = test::levenshteinDistance("GeneralStateTests/stExample2", testList);
    BOOST_CHECK(test::inArray(list, string("GeneralStateTests/stExample")));
    BOOST_CHECK(test::inArray(list, string("BCGeneralStateTests/stExample")));
}

BOOST_AUTO_TEST_SUITE_END()
