/*
	This file is part of solidity.

	solidity is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	solidity is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with solidity.  If not, see <http://www.gnu.org/licenses/>.

	The Implementation originally from https://msdn.microsoft.com/en-us/library/windows/desktop/aa365592(v=vs.85).aspx
*/
/// @file RPCSession.cpp
/// Low-level IPC communication between the test framework and the Ethereum node.

#include "RPCSession.h"
#include <json/reader.h>
#include <json/writer.h>

#include <string>
#include <stdio.h>
#include <thread>
#include <chrono>
#include <cstdio>
#include <mutex>
#include <csignal>

#include <dataObject/ConvertJsoncpp.h>
#include <retesteth/EthChecks.h>
#include <retesteth/ExitHandler.h>
#include <retesteth/TestHelper.h>
#include <retesteth/TestOutputHelper.h>

using namespace std;
using namespace dev;

struct sessionInfo
{
    sessionInfo(
        FILE* _pipe, RPCSession* _session, std::string const& _tmpDir, int _pid, test::ClientConfigID const& _configId)
    {
        session.reset(_session);
        filePipe.reset(_pipe);
        tmpDir = _tmpDir;
        pipePid = _pid;
        isUsed = RPCSession::NotExist;
        configId = _configId;
    }
    std::unique_ptr<RPCSession> session;
    std::unique_ptr<FILE> filePipe;
    int pipePid;
    RPCSession::SessionStatus isUsed;
    std::string tmpDir;
    test::ClientConfigID configId;
};

void closeSession(const string& _threadID);

std::mutex g_socketMapMutex;
static std::map<std::string, sessionInfo> socketMap;
void RPCSession::runNewInstanceOfAClient(string const& _threadID, ClientConfig const& _config)
{
    if (_config.getType() == Socket::IPC)
    {
        fs::path tmpDir = test::createUniqueTmpDirectory();
        string ipcPath = tmpDir.string() + "/geth.ipc";

        string command = "bash";
        std::vector<string> args;
        args.push_back(_config.getShellPath().c_str());
        args.push_back(tmpDir.string());
        args.push_back(ipcPath);

        int pid = 0;
        test::popenOutput mode = (Options::get().enableClientsOutput) ?
                                     test::popenOutput::EnableALL :
                                     test::popenOutput::DisableAll;
        FILE* fp = test::popen2(command, args, "r", pid, mode);
        if (!fp)
        {
            ETH_ERROR_MESSAGE("Failed to start the client: '" + command + "'");
            std::raise(SIGABRT);
        }
        else
        {
            int maxSeconds = 25;
            while (!boost::filesystem::exists(ipcPath) && maxSeconds-- > 0)
                std::this_thread::sleep_for(std::chrono::seconds(1));
            ETH_FAIL_REQUIRE_MESSAGE(maxSeconds > 0, "Client took too long to start ipc!");
            // Client has opened ipc socket. wait for it to initialize
            std::this_thread::sleep_for(std::chrono::seconds(4));
        }
        sessionInfo info(fp, new RPCSession(Socket::SocketType::IPC, ipcPath), tmpDir.string(), pid,
            _config.getId());
        {
            std::lock_guard<std::mutex> lock(
                g_socketMapMutex);  // function must be called from lock
            socketMap.insert(std::pair<string, sessionInfo>(_threadID, std::move(info)));
        }
    }
    else if (_config.getType() == Socket::TCP)
    {
        sessionInfo info(NULL, new RPCSession(Socket::SocketType::TCP, _config.getAddress()), "", 0,
            _config.getId());
        {
            std::lock_guard<std::mutex> lock(
                g_socketMapMutex);  // function must be called from lock
            socketMap.insert(std::pair<string, sessionInfo>(_threadID, std::move(info)));
        }
    }
    else if (_config.getType() == Socket::IPCDebug)
    {
        // connect to already opend .ipc socket
        fs::path tmpDir = test::createUniqueTmpDirectory();
        string ipcPath = _config.getAddress();
        int pid = 0;
        FILE* fp = NULL;
        sessionInfo info(fp, new RPCSession(Socket::SocketType::IPC, ipcPath), tmpDir.string(), pid,
            _config.getId());
        {
            std::lock_guard<std::mutex> lock(
                g_socketMapMutex);  // function must be called from lock
            socketMap.insert(std::pair<string, sessionInfo>(_threadID, std::move(info)));
        }
    }
    else
        ETH_FAIL_MESSAGE("Unknown Socket Type in runNewInstanceOfAClient");
}

RPCSession& RPCSession::instance(const string& _threadID)
{
    bool needToCreateNew = false;
    {
        std::lock_guard<std::mutex> lock(g_socketMapMutex);
        test::ClientConfigID currentConfigId = Options::getDynamicOptions().getCurrentConfig().getId();
        if (socketMap.count(_threadID) && socketMap.at(_threadID).configId != currentConfigId)
        {
            // For this thread a session is opened but it is opened not for current tested client
            ETH_FAIL_MESSAGE("A session opened for another client id!");
        }

        if (!socketMap.count(_threadID))
        {
            // look for free clients that already instantiated
            for (auto& socket : socketMap)
            {
                if (socket.second.isUsed == SessionStatus::Available)
                    if (socket.second.configId == currentConfigId)
                    {
                        socket.second.isUsed = SessionStatus::Working;
                        socketMap.insert(
                            std::pair<string, sessionInfo>(_threadID, std::move(socket.second)));
                        socketMap.erase(socketMap.find(socket.first));  // remove previous threadID
                                                                        // assigment to this socket
                        return *(socketMap.at(_threadID).session.get());
                }
            }
            needToCreateNew = true;
        }
    }
    if (needToCreateNew)
        runNewInstanceOfAClient(_threadID, Options::getDynamicOptions().getCurrentConfig());

    std::lock_guard<std::mutex> lock(g_socketMapMutex);
    ETH_FAIL_REQUIRE_MESSAGE(socketMap.size() <= Options::get().threadCount,
        "Something went wrong. Retesteth create more instances than needed!");
    return *(socketMap.at(_threadID).session.get());
}

void RPCSession::sessionStart(std::string const& _threadID)
{
    RPCSession::instance(_threadID);  // initialize the client if not exist
    std::lock_guard<std::mutex> lock(g_socketMapMutex);
    if (socketMap.count(_threadID))
        socketMap.at(_threadID).isUsed = SessionStatus::Working;
}

void RPCSession::sessionEnd(std::string const& _threadID, SessionStatus _status)
{
    std::lock_guard<std::mutex> lock(g_socketMapMutex);
    assert(socketMap.count(_threadID));
    if (socketMap.count(_threadID))
        socketMap.at(_threadID).isUsed = _status;
}

RPCSession::SessionStatus RPCSession::sessionStatus(std::string const& _threadID)
{
    std::lock_guard<std::mutex> lock(g_socketMapMutex);
    if (socketMap.count(_threadID))
        return socketMap.at(_threadID).isUsed;
    return RPCSession::NotExist;
}

void closeSession(const string& _threadID)
{
    ETH_FAIL_REQUIRE_MESSAGE(socketMap.count(_threadID), "Socket map is empty in closeSession!");
    sessionInfo& element = socketMap.at(_threadID);
    if (element.session.get()->getSocketType() == Socket::SocketType::IPC)
    {
        test::pclose2(element.filePipe.get(), element.pipePid);
        std::this_thread::sleep_for(std::chrono::seconds(4));
        boost::filesystem::remove_all(boost::filesystem::path(element.tmpDir));
        element.filePipe.release();
        element.session.release();
    }
}

void RPCSession::clear()
{
    std::lock_guard<std::mutex> lock(g_socketMapMutex);
    std::vector<thread> closingThreads;
    for (auto& element : socketMap)
        closingThreads.push_back(thread(closeSession, element.first));
    for (auto& th : closingThreads)
        th.join();

    socketMap.clear();
    closingThreads.clear();
}

Json::Value RPCSession::debug_accountRangeAt(std::string const& _blockHashOrNumber, int _txIndex, string const& _address, int _maxResults)
{
    return rpcCall("debug_accountRangeAt", { quote(_blockHashOrNumber), to_string(_txIndex), quote(_address), to_string(_maxResults) });
}

Json::Value RPCSession::debug_storageRangeAt(std::string const& _blockHashOrNumber, int _txIndex, string const& _address, string const& _begin, int _maxResults)
{
    return rpcCall("debug_storageRangeAt", { quote(_blockHashOrNumber), to_string(_txIndex), quote(_address) , quote(_begin), to_string(_maxResults) });
}

string RPCSession::web3_clientVersion()
{
	return rpcCall("web3_clientVersion", { }).asString();
}

string RPCSession::eth_getCode(string const& _address, string const& _blockNumber)
{
	return rpcCall("eth_getCode", { quote(_address), quote(_blockNumber) }).asString();
}

test::scheme_block RPCSession::eth_getBlockByNumber(string const& _blockNumber, bool _fullObjects)
{
	// NOTE: to_string() converts bool to 0 or 1
	Json::Value response = rpcCall("eth_getBlockByNumber", { quote(_blockNumber), _fullObjects ? "true" : "false" });
    return test::scheme_block(dataobject::ConvertJsoncppToData(response));
}

test::scheme_transactionReceipt RPCSession::eth_getTransactionReceipt(string const& _transactionHash)
{
	Json::Value const result = rpcCall("eth_getTransactionReceipt", { quote(_transactionHash) });
    return test::scheme_transactionReceipt(dataobject::ConvertJsoncppToData(result));
}

string RPCSession::eth_blockNumber()
{
    return rpcCall("eth_blockNumber", { }).asString();
}

string RPCSession::eth_sendTransaction(TransactionData const& _td)
{
	return rpcCall("eth_sendTransaction", { _td.toJson() }).asString();
}

string RPCSession::eth_call(TransactionData const& _td, string const& _blockNumber)
{
	return rpcCall("eth_call", { _td.toJson(), quote(_blockNumber) }).asString();
}

string RPCSession::eth_sendTransaction(string const& _transaction)
{
	return rpcCall("eth_sendTransaction", { _transaction }).asString();
}

string RPCSession::eth_sendRawTransaction(std::string const& _rlp)
{
    return rpcCall("eth_sendRawTransaction", {quote(_rlp)}, true).asString();
}

std::string RPCSession::eth_getTransactionCount(std::string const& _address, std::string const& _blockNumber)
{
	return rpcCall("eth_getTransactionCount", { quote(_address), quote(_blockNumber) }).asString();
}

string RPCSession::eth_getBalance(string const& _address, string const& _blockNumber)
{
	string address = (_address.length() == 20) ? "0x" + _address : _address;
	return rpcCall("eth_getBalance", { quote(address), quote(_blockNumber) }).asString();
}

string RPCSession::eth_getStorageRoot(string const& _address, string const& _blockNumber)
{
	string address = (_address.length() == 20) ? "0x" + _address : _address;
	return rpcCall("eth_getStorageRoot", { quote(address), quote(_blockNumber) }).asString();
}

string RPCSession::eth_getStorageAt(string const& _address, string const& _position, string const& _blockNumber)
{
	return rpcCall("eth_getStorageAt", { quote(_address), quote(_position), quote(_blockNumber) }).asString();
}

void RPCSession::personal_unlockAccount(string const& _address, string const& _password, int _duration)
{
    rpcCall("personal_unlockAccount", { quote(_address), quote(_password), to_string(_duration) });
}

string RPCSession::personal_newAccount(string const& _password)
{
	string addr = rpcCall("personal_newAccount", { quote(_password) }).asString();
    ETH_TEST_MESSAGE("Created account " + addr);
	return addr;
}

void RPCSession::test_setChainParams(vector<string> const& _accounts)
{
	static std::string const c_configString = R"(
	{
		"sealEngine": "NoProof",
		"params": {
			"accountStartNonce": "0x00",
			"maximumExtraDataSize": "0x1000000",
			"blockReward": "0x",
			"allowFutureBlocks": true,
			"homesteadForkBlock": "0x00",
			"EIP150ForkBlock": "0x00",
			"EIP158ForkBlock": "0x00"
		},
		"genesis": {
			"author": "0000000000000010000000000000000000000000",
			"timestamp": "0x00",
			"parentHash": "0x0000000000000000000000000000000000000000000000000000000000000000",
			"extraData": "0x",
			"gasLimit": "0x1000000000000"
		},
		"accounts": {
			"0000000000000000000000000000000000000001": { "wei": "1", "precompiled": { "name": "ecrecover", "linear": { "base": 3000, "word": 0 } } },
			"0000000000000000000000000000000000000002": { "wei": "1", "precompiled": { "name": "sha256", "linear": { "base": 60, "word": 12 } } },
			"0000000000000000000000000000000000000003": { "wei": "1", "precompiled": { "name": "ripemd160", "linear": { "base": 600, "word": 120 } } },
			"0000000000000000000000000000000000000004": { "wei": "1", "precompiled": { "name": "identity", "linear": { "base": 15, "word": 3 } } },
			"0000000000000000000000000000000000000005": { "wei": "1", "precompiled": { "name": "modexp" } },
			"0000000000000000000000000000000000000006": { "wei": "1", "precompiled": { "name": "alt_bn128_G1_add", "linear": { "base": 500, "word": 0 } } },
			"0000000000000000000000000000000000000007": { "wei": "1", "precompiled": { "name": "alt_bn128_G1_mul", "linear": { "base": 40000, "word": 0 } } },
			"0000000000000000000000000000000000000008": { "wei": "1", "precompiled": { "name": "alt_bn128_pairing_product" } }
		}
	}
	)";

	Json::Value config;
    ETH_FAIL_REQUIRE_MESSAGE(Json::Reader().parse(c_configString, config), "setChainParams json is incorrect!");
	for (auto const& account: _accounts)
		config["accounts"][account]["wei"] = "0x100000000000000000000000000000000000000000";
	test_setChainParams(Json::FastWriter().write(config));
}

string RPCSession::test_getBlockStatus(std::string const& _blockHash)
{
    return rpcCall("test_getBlockStatus", {quote(_blockHash)}).asString();
}

string RPCSession::test_getLogHash(std::string const& _txHash)
{
	return rpcCall("test_getLogHash", { quote(_txHash) }).asString();
}

void RPCSession::test_importRawBlock(std::string const& _blockRLP)
{
    rpcCall("test_importRawBlock", {quote(_blockRLP)}, true);
}

void RPCSession::test_setChainParams(string const& _config)
{
    ETH_FAIL_REQUIRE_MESSAGE(rpcCall("test_setChainParams", { _config }) == true, "remote test_setChainParams = false");
}

void RPCSession::test_rewindToBlock(size_t _blockNr)
{
    ETH_FAIL_REQUIRE_MESSAGE(rpcCall("test_rewindToBlock", { to_string(_blockNr) }) == true, "remote test_rewintToBlock = false");
}

void RPCSession::test_mineBlocks(int _number, string const& _hash)
{
       (void)_hash;
    u256 startBlock = fromBigEndian<u256>(fromHex(rpcCall("eth_blockNumber").asString()));
    ETH_FAIL_REQUIRE_MESSAGE(rpcCall("test_mineBlocks", { to_string(_number) }, true) == true, "remote test_mineBlocks = false");

	// We auto-calibrate the time it takes to mine the transaction.
	// It would be better to go without polling, but that would probably need a change to the test client

	auto startTime = std::chrono::steady_clock::now();
	unsigned sleepTime = m_sleepTime;
	size_t tries = 0;
	for (; ; ++tries)
    {
        std::this_thread::sleep_for(chrono::milliseconds(sleepTime));
        auto endTime = std::chrono::steady_clock::now();
		unsigned timeSpent = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();
		if (timeSpent > m_maxMiningTime)
			break; // could be that some blocks are invalid.
                   // ETH_FAIL_MESSAGE("Error in test_mineBlocks: block mining timeout! " +
        // test::TestOutputHelper::get().testName());

        // std::cerr << test_getBlockStatus(_hash) << std::endl;
         bigint number = fromBigEndian<u256>(fromHex(rpcCall("eth_blockNumber").asString()));
         if (number >= startBlock + _number)
            break;
        //if (test_getBlockStatus(_hash) == "Ready")
        //    break;
        else
			sleepTime *= 2;
	}
	if (tries > 1)
	{
		m_successfulMineRuns = 0;
		m_sleepTime += 2;
	}
	else if (tries == 1)
	{
		m_successfulMineRuns++;
		if (m_successfulMineRuns > 5)
		{
			m_successfulMineRuns = 0;
			if (m_sleepTime > 2)
				m_sleepTime--;
		}
	}
}

void RPCSession::test_modifyTimestamp(size_t _timestamp)
{
    ETH_FAIL_REQUIRE_MESSAGE(rpcCall("test_modifyTimestamp", { to_string(_timestamp) }) == true, "test_modifyTimestamp was not successfull");
}

std::string RPCSession::sendRawRequest(string const& _request)
{
    return m_socket.sendRequest(_request);
}

Json::Value RPCSession::rpcCall(string const& _methodName, vector<string> const& _args, bool _canFail)
{
    string request = "{\"jsonrpc\":\"2.0\",\"method\":\"" + _methodName + "\",\"params\":[";
    for (size_t i = 0; i < _args.size(); ++i)
    {
        request += _args[i];
        if (i + 1 != _args.size())
            request += ", ";
    }

    request += "],\"id\":" + to_string(m_rpcSequence) + "}";
    ++m_rpcSequence;

    ETH_TEST_MESSAGE("Request: " + request);
    string reply = m_socket.sendRequest(request);
    ETH_TEST_MESSAGE("Reply: " + reply);

    Json::Value result;
    ETH_FAIL_REQUIRE_MESSAGE(Json::Reader().parse(reply, result, false),
        "error parsing json from remote response: " + reply);

    if (result.isMember("error"))
    {
        m_lastRPCErrorString = "Error on JSON-RPC call (" + test::TestOutputHelper::get().testName() + "): "
                            + result["error"]["message"].asString()
                            + " Request: " + request;
        if (_canFail)
            return Json::Value();
        ETH_FAIL_MESSAGE(m_lastRPCErrorString);
    }
    m_lastRPCErrorString = string();    //null the error as last RPC call was success.
    return result["result"];
}

string const& RPCSession::accountCreate()
{
	m_accounts.push_back(personal_newAccount(""));
	personal_unlockAccount(m_accounts.back(), "", 100000);
	return m_accounts.back();
}

string const& RPCSession::accountCreateIfNotExists(size_t _id)
{
	while ((_id + 1) > m_accounts.size())
		accountCreate();
	return m_accounts[_id];
}

RPCSession::RPCSession(Socket::SocketType _type, const string& _path):
    m_socket(_type, _path)
{
	//accountCreate();
	//This will pre-fund the accounts create prior.
	//test_setChainParams(m_accounts);
}

string RPCSession::TransactionData::toJson() const
{
	Json::Value json;
	json["from"] = (from.length() == 20) ? "0x" + from : from;
	json["to"] = (to.length() == 20 || to == "") ? "0x" + to :  to;
	json["gas"] = gas;
	json["gasprice"] = gasPrice;
	json["value"] = value;
	json["data"] = data;
	return Json::FastWriter().write(json);

}

