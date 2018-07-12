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
        TestOutputHelper::get().setCurrentTestName(
            boost::unit_test::framework::current_test_case().p_name);
    }

    RPCSession *session;
};

DataObject getResponse(RPCSession* _session, string const& _request)
{
    Json::Value result;
    ETH_REQUIRE(Json::Reader().parse(_session->sendRawRequest(_request), result, false));
    return test::convertJsonCPPtoData(result);
}

const string cRpcError = "Reply RPC is different: ";

void checkIdAndVersion(DataObject const& _obj, int id)
{
    static const string cRpcVersion = "2.0";
    static const string cRpcIdError = "Reply RPC id is different: ";
    static const string cRpcVersionError = "Reply RPC version is different: ";
    ETH_CHECK_EQUAL(_obj.at("id").asInt(), id, cRpcIdError);
    ETH_CHECK_EQUAL(_obj.at("jsonrpc").asString(), cRpcVersion, cRpcVersionError);
}

BOOST_FIXTURE_TEST_SUITE(RPCTests, RPCTestFixture)

BOOST_AUTO_TEST_CASE(web3_clientVersion)
{
    string const requestStr = R"(
       {"jsonrpc":"2.0","method":"web3_clientVersion","params":[],"id":67}
    )";
    //{"id":67,"jsonrpc":"2.0","result":"aleth/1.4.0.dev1-99+commit.6426af1e.dirty/linux/gnu5.4.0/relwithdebinfo"}
    DataObject obj = getResponse(session, requestStr);
    test::requireJsonFields(obj, "web3_clientVersion",
        {{"id", {DataType::Integer}}, {"jsonrpc", {DataType::String}},
            {"result", {DataType::String}}});
    checkIdAndVersion(obj, 67);
}

BOOST_AUTO_TEST_CASE(web3_sha3)
{
    string const requestStr = R"(
       {"jsonrpc":"2.0","method":"web3_sha3","params":["0x68656c6c6f20776f726c64"],"id":64}
    )";

    DataObject obj = getResponse(session, requestStr);
    test::requireJsonFields(obj, "web3_sha3",
        {{"id", {DataType::Integer}}, {"jsonrpc", {DataType::String}},
            {"result", {DataType::String}}});
    checkIdAndVersion(obj, 64);
    ETH_CHECK_EQUAL(obj.at("result").asString(),
        string("0x47173285a8d7341e5e972fc677286384f802f8ef42a5ec5f03bbfa254cb01fad"), cRpcError);
}

BOOST_AUTO_TEST_CASE(net_version)
{
    string const requestStr = R"(
       {"jsonrpc":"2.0","method":"net_version","params":[],"id":67}
    )";

    DataObject obj = getResponse(session, requestStr);
    test::requireJsonFields(obj, "net_version",
        {{"id", {DataType::Integer}}, {"jsonrpc", {DataType::String}},
            {"result", {DataType::String}}});
    checkIdAndVersion(obj, 67);
    /*
     * "1": Ethereum Mainnet
       "2": Morden Testnet (deprecated)
       "3": Ropsten Testnet
       "4": Rinkeby Testnet
       "42": Kovan Testnet
    */
    ETH_CHECK_EQUAL(obj.at("result").asString(), string("1"), cRpcError);
}

BOOST_AUTO_TEST_CASE(net_listening)
{
    string const requestStr = R"(
       {"jsonrpc":"2.0","method":"net_listening","params":[],"id":67}
    )";

    DataObject obj = getResponse(session, requestStr);
    test::requireJsonFields(obj, "net_listening",
        {{"id", {DataType::Integer}}, {"jsonrpc", {DataType::String}},
            {"result", {DataType::Bool}}});
    checkIdAndVersion(obj, 67);
    ETH_CHECK_EQUAL(obj.at("result").asBool(), false, cRpcError);
}

BOOST_AUTO_TEST_CASE(net_peerCount)
{
    string const requestStr = R"(
       {"jsonrpc":"2.0","method":"net_peerCount","params":[],"id":74}
    )";

    DataObject obj = getResponse(session, requestStr);
    test::requireJsonFields(obj, "net_peerCount",
        {{"id", {DataType::Integer}}, {"jsonrpc", {DataType::String}},
            {"result", {DataType::String}}});
    checkIdAndVersion(obj, 74);
    ETH_CHECK_EQUAL(obj.at("result").asString(), string("0x0"), cRpcError);
}

BOOST_AUTO_TEST_CASE(eth_protocolVersion)
{
    string const requestStr = R"(
       {"jsonrpc":"2.0","method":"eth_protocolVersion","params":[],"id":67}
    )";

    DataObject obj = getResponse(session, requestStr);
    test::requireJsonFields(obj, "eth_protocolVersion",
        {{"id", {DataType::Integer}}, {"jsonrpc", {DataType::String}},
            {"result", {DataType::String}}});
    checkIdAndVersion(obj, 67);
    ETH_CHECK_EQUAL(obj.at("result").asString(), string("0x3f"), cRpcError);
}

BOOST_AUTO_TEST_CASE(eth_syncing)
{
    string const requestStr = R"(
       {"jsonrpc":"2.0","method":"eth_syncing","params":[],"id":1}
    )";

    DataObject obj = getResponse(session, requestStr);
    test::requireJsonFields(obj, "eth_syncing",
        {{"id", {DataType::Integer}}, {"jsonrpc", {DataType::String}},
            {"result", {DataType::Object, DataType::Bool}}});
    checkIdAndVersion(obj, 1);
    ETH_CHECK_EQUAL(obj.at("result").asBool(), false, cRpcError);
}

BOOST_AUTO_TEST_CASE(eth_coinbase)
{
    string const requestStr = R"(
       {"jsonrpc":"2.0","method":"eth_coinbase","params":[],"id":64}
    )";

    DataObject obj = getResponse(session, requestStr);
    test::requireJsonFields(obj, "eth_coinbase",
        {{"id", {DataType::Integer}}, {"jsonrpc", {DataType::String}},
            {"result", {DataType::String}}});
    checkIdAndVersion(obj, 64);
    ETH_CHECK_EQUAL(obj.at("result").asString(),
        string("0x0000000000000000000000000000000000000000"), cRpcError);
}

BOOST_AUTO_TEST_CASE(eth_mining)
{
    string const requestStr = R"(
       {"jsonrpc":"2.0","method":"eth_mining","params":[],"id":71}
    )";

    DataObject obj = getResponse(session, requestStr);
    test::requireJsonFields(obj, "eth_mining",
        {{"id", {DataType::Integer}}, {"jsonrpc", {DataType::String}},
            {"result", {DataType::Bool}}});
    checkIdAndVersion(obj, 71);
    ETH_CHECK_EQUAL(obj.at("result").asBool(), false, cRpcError);
}

BOOST_AUTO_TEST_CASE(eth_hashrate)
{
    string const requestStr = R"(
       {"jsonrpc":"2.0","method":"eth_hashrate","params":[],"id":71}
    )";

    DataObject obj = getResponse(session, requestStr);
    test::requireJsonFields(obj, "eth_hashrate",
        {{"id", {DataType::Integer}}, {"jsonrpc", {DataType::String}},
            {"result", {DataType::String}}});
    checkIdAndVersion(obj, 71);
    ETH_CHECK_EQUAL(obj.at("result").asString(), string("0x0"), cRpcError);
}

BOOST_AUTO_TEST_CASE(eth_gasPrice)
{
    string const requestStr = R"(
       {"jsonrpc":"2.0","method":"eth_gasPrice","params":[],"id":73}
    )";

    DataObject obj = getResponse(session, requestStr);
    test::requireJsonFields(obj, "eth_gasPrice",
        {{"id", {DataType::Integer}}, {"jsonrpc", {DataType::String}},
            {"result", {DataType::String}}});
    checkIdAndVersion(obj, 73);
    ETH_CHECK_EQUAL(obj.at("result").asString(), string("0x4a817c800"), cRpcError);
}

BOOST_AUTO_TEST_CASE(eth_accounts)
{
    string const requestStr = R"(
       {"jsonrpc":"2.0","method":"eth_accounts","params":[],"id":1}
    )";

    DataObject obj = getResponse(session, requestStr);
    test::requireJsonFields(obj, "eth_accounts",
        {{"id", {DataType::Integer}}, {"jsonrpc", {DataType::String}},
            {"result", {DataType::Array}}});
    checkIdAndVersion(obj, 1);
    ETH_CHECK_EQUAL((int)obj.at("result").getSubObjects().size(), 0, cRpcError);
}

BOOST_AUTO_TEST_CASE(eth_blockNumber)
{
    string const requestStr = R"(
       {"jsonrpc":"2.0","method":"eth_blockNumber","params":[],"id":83}
    )";

    DataObject obj = getResponse(session, requestStr);
    test::requireJsonFields(obj, "eth_blockNumber",
        {{"id", {DataType::Integer}}, {"jsonrpc", {DataType::String}},
            {"result", {DataType::String}}});
    checkIdAndVersion(obj, 83);
    ETH_CHECK_EQUAL(obj.at("result").asString(), string("0x0"), cRpcError);
}

BOOST_AUTO_TEST_CASE(eth_getBalance)
{
    string const requestStr = R"(
       {"jsonrpc":"2.0","method":"eth_getBalance","params":["0xc94770007dda54cF92009BFF0dE90c06F603a09f", "latest"],"id":1}
    )";

    DataObject obj = getResponse(session, requestStr);
    test::requireJsonFields(obj, "eth_getBalance",
        {{"id", {DataType::Integer}}, {"jsonrpc", {DataType::String}},
            {"result", {DataType::String}}});
    checkIdAndVersion(obj, 1);
    ETH_CHECK_EQUAL(obj.at("result").asString(), string("0x0"), cRpcError);
}

BOOST_AUTO_TEST_CASE(eth_getStorageAt)
{
    string const requestStr = R"(
       {"jsonrpc":"2.0", "method": "eth_getStorageAt", "params": ["0x295a70b2de5e3953354a6a8344e616ed314d7251", "0x0", "latest"], "id": 1}
    )";

    DataObject obj = getResponse(session, requestStr);
    test::requireJsonFields(obj, "eth_getBalance",
        {{"id", {DataType::Integer}}, {"jsonrpc", {DataType::String}},
            {"result", {DataType::String}}});
    checkIdAndVersion(obj, 1);
    ETH_CHECK_EQUAL(obj.at("result").asString(),
        string("0x0000000000000000000000000000000000000000000000000000000000000000"), cRpcError);
}

BOOST_AUTO_TEST_CASE(eth_getTransactionCount)
{
    string const requestStr = R"(
       {"jsonrpc":"2.0","method":"eth_getTransactionCount","params":["0xc94770007dda54cF92009BFF0dE90c06F603a09f","latest"],"id":1}
    )";

    DataObject obj = getResponse(session, requestStr);
    test::requireJsonFields(obj, "eth_getTransactionCount",
        {{"id", {DataType::Integer}}, {"jsonrpc", {DataType::String}},
            {"result", {DataType::String}}});
    checkIdAndVersion(obj, 1);
    ETH_CHECK_EQUAL(obj.at("result").asString(), string("0x0"), cRpcError);
}

BOOST_AUTO_TEST_CASE(eth_getBlockTransactionCountByHash)
{
    string const requestStr = R"(
       {"jsonrpc":"2.0","method":"eth_getBlockTransactionCountByHash","params":["0xc94770007dda54cF92009BFF0dE90c06F603a09f"],"id":1}
    )";

    DataObject obj = getResponse(session, requestStr);
    test::requireJsonFields(obj, "eth_getTransactionCount",
        {{"id", {DataType::Integer}}, {"jsonrpc", {DataType::String}},
            {"result", {DataType::String}}});
    checkIdAndVersion(obj, 1);
    ETH_CHECK_EQUAL(obj.at("result").asString(), string("0x0"), cRpcError);
}

BOOST_AUTO_TEST_CASE(eth_getBlockTransactionCountByNumber)
{
    string const requestStr = R"(
       {"jsonrpc":"2.0","method":"eth_getBlockTransactionCountByNumber","params":["0xe8"],"id":1}
    )";

    DataObject obj = getResponse(session, requestStr);
    test::requireJsonFields(obj, "eth_getBlockTransactionCountByNumber",
        {{"id", {DataType::Integer}}, {"jsonrpc", {DataType::String}},
            {"result", {DataType::String}}});
    checkIdAndVersion(obj, 1);
    ETH_CHECK_EQUAL(obj.at("result").asString(), string("0x0"), cRpcError);
}

BOOST_AUTO_TEST_CASE(eth_getUncleCountByBlockHash)
{
    string const requestStr = R"(
       {"jsonrpc":"2.0","method":"eth_getUncleCountByBlockHash","params":["0xc94770007dda54cF92009BFF0dE90c06F603a09f"],"id":1}
    )";

    DataObject obj = getResponse(session, requestStr);
    test::requireJsonFields(obj, "eth_getUncleCountByBlockHash",
        {{"id", {DataType::Integer}}, {"jsonrpc", {DataType::String}},
            {"result", {DataType::String}}});
    checkIdAndVersion(obj, 1);
    ETH_CHECK_EQUAL(obj.at("result").asString(), string("0x0"), cRpcError);
}

BOOST_AUTO_TEST_SUITE_END()
