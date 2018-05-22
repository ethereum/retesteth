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

#include <retesteth/TestHelper.h>
#include <retesteth/TestOutputHelper.h>
#include <retesteth/ExitHandler.h>
#include <retesteth/EthChecks.h>

using namespace std;
using namespace dev;

IPCSocket::IPCSocket(string const& _path): m_path(_path)
{
#if defined(_WIN32)
	m_socket = CreateFile(
		m_path.c_str(),   // pipe name
		GENERIC_READ |  // read and write access
		GENERIC_WRITE,
		0,              // no sharing
		NULL,           // default security attribute
		OPEN_EXISTING,  // opens existing pipe
		0,              // default attributes
		NULL);          // no template file

	if (m_socket == INVALID_HANDLE_VALUE)
		ETH_FAIL("Error creating IPC socket object!");

#else
	if (_path.length() >= sizeof(sockaddr_un::sun_path))
		ETH_FAIL("Error opening IPC: socket path is too long!");

	struct sockaddr_un saun;
	memset(&saun, 0, sizeof(sockaddr_un));
	saun.sun_family = AF_UNIX;
	strcpy(saun.sun_path, _path.c_str());

// http://idletechnology.blogspot.ca/2011/12/unix-domain-sockets-on-osx.html
//
// SUN_LEN() might be optimal, but it seemingly affects the portability,
// with at least Android missing this macro.  Just using the sizeof() for
// structure seemingly works, and would only have the side-effect of
// sending larger-than-required packets over the socket.  Given that this
// code is only used for unit-tests, that approach seems simpler.
#if defined(__APPLE__)
	saun.sun_len = sizeof(struct sockaddr_un);
#endif //  defined(__APPLE__)

	if ((m_socket = socket(AF_UNIX, SOCK_STREAM, 0)) < 0)
		ETH_FAIL("Error creating IPC socket object");

	if (connect(m_socket, reinterpret_cast<struct sockaddr const*>(&saun), sizeof(struct sockaddr_un)) < 0)
	{
		close(m_socket);
		ETH_FAIL("Error connecting to IPC socket: " + _path);
	}
#endif
}

string IPCSocket::sendRequest(string const& _req)
{
#if defined(_WIN32)
	// Write to the pipe.
	DWORD cbWritten;
	BOOL fSuccess = WriteFile(
		m_socket,               // pipe handle
		_req.c_str(),           // message
		_req.size(),            // message length
		&cbWritten,             // bytes written
		NULL);                  // not overlapped

	if (!fSuccess || (_req.size() != cbWritten))
		ETH_FAIL("WriteFile to pipe failed");

	// Read from the pipe.
	DWORD cbRead;
	fSuccess = ReadFile(
		m_socket,          // pipe handle
		m_readBuf,         // buffer to receive reply
		sizeof(m_readBuf), // size of buffer
		&cbRead,           // number of bytes read
		NULL);             // not overlapped

	if (!fSuccess)
		ETH_FAIL("ReadFile from pipe failed");

	return string(m_readBuf, m_readBuf + cbRead);
#else
	if (send(m_socket, _req.c_str(), _req.length(), 0) != (ssize_t)_req.length())
		ETH_FAIL("Writing on IPC failed.");

	auto start = chrono::steady_clock::now();
	ssize_t ret;
	string reply;
	do
	{
		ret = recv(m_socket, m_readBuf, sizeof(m_readBuf), 0);

		// Also consider closed socket an error.
		if (ret < 0)
			ETH_FAIL("Reading on IPC failed.");
		if (ExitHandler::shouldExit())
			return "";

		// check for a long message
		if (ret != 0)
		{
			ssize_t ret2 = ret;
			do
			{
				reply += string(m_readBuf, m_readBuf + ret2);
				ret2 = recv(m_socket, m_readBuf, sizeof(m_readBuf), MSG_DONTWAIT);
				if (ret2 > 0)
					std::this_thread::sleep_for(std::chrono::milliseconds(100)); //wait for socket update
			}
			while (ret2 > 0);
		}
	}
	while (
		ret == 0 &&
		chrono::duration_cast<chrono::milliseconds>(chrono::steady_clock::now() - start).count() < m_readTimeOutMS
	);

	if (ret == 0)
		ETH_FAIL("Timeout reading on IPC.");

	return reply;
#endif
}

struct sessionInfo
{
    sessionInfo(FILE* _pipe, RPCSession* _session, std::string const& _tmpDir, int _pid)
    {
        session.reset(_session);
        filePipe.reset(_pipe);
        tmpDir = _tmpDir;
        pipePid = _pid;
        isUsed = RPCSession::NotExist;
    }
    std::unique_ptr<RPCSession> session;
    std::unique_ptr<FILE> filePipe;
    int pipePid;
    RPCSession::SessionStatus isUsed;
    std::string tmpDir;
};

std::mutex g_socketMapMutex;
static std::map<std::string, sessionInfo> socketMap;
void RPCSession::runNewInstanceOfAClient(string const& _threadID)
{
    fs::path tmpDir = test::createUniqueTmpDirectory();
    string ipcPath = tmpDir.string() + "/geth.ipc";

    string command = "eth";
    std::vector<string> args;
    args.push_back("--test");           // 1
    args.push_back("--db-path");        // 2
    args.push_back(tmpDir.string());    // 3
    args.push_back("--ipcpath");        // 4
    args.push_back(ipcPath);            // 5
    args.push_back("--log-verbosity");  // 6
    args.push_back("5");                // 7

    int pid = 0;
    test::popenOutput mode = (Options::get().enableClientsOutput) ? test::popenOutput::EnableALL :
                                                                    test::popenOutput::DisableAll;
    FILE* fp = test::popen2(command, args, "r", pid, mode);
    if (!fp)
    {
        ETH_ERROR("Failed to start the client: '" + command + "'");
        ExitHandler::setFinishExecution(true);
        std::raise(SIGABRT);
    }
    else
    {
        int maxSeconds = 25;
        while (!boost::filesystem::exists(ipcPath) && maxSeconds-- > 0)
            std::this_thread::sleep_for(std::chrono::seconds(1));
        ETH_REQUIRE_MESSAGE(maxSeconds > 0, "Client took too long to start ipc!");
        // Client has opened ipc socket. wait for it to initialize
        std::this_thread::sleep_for(std::chrono::seconds(4));
    }

    sessionInfo info(fp, new RPCSession(ipcPath), tmpDir.string(), pid);
    // sessionInfo info(fp, new RPCSession("/home/wins/.ethereum/geth.ipc"), dir, pid);
    {
        std::lock_guard<std::mutex> lock(g_socketMapMutex);  // function must be called from lock
        socketMap.insert(std::pair<string, sessionInfo>(_threadID, std::move(info)));
    }
}

RPCSession& RPCSession::instance(const string& _threadID)
{
    bool needToCreateNew = false;
    {
        std::lock_guard<std::mutex> lock(g_socketMapMutex);
        if (!socketMap.count(_threadID))
        {
            // look for free clients that already instantiated
            for (auto& socket : socketMap)
            {
                if (socket.second.isUsed == SessionStatus::Available)
                {
                    socket.second.isUsed = SessionStatus::Working;
                    socketMap.insert(
                        std::pair<string, sessionInfo>(_threadID, std::move(socket.second)));
                    socketMap.erase(socketMap.find(socket.first));
                    return *(socketMap.at(_threadID).session.get());
                }
            }
            needToCreateNew = true;
        }
    }
    if (needToCreateNew)
        runNewInstanceOfAClient(_threadID);
    std::lock_guard<std::mutex> lock(g_socketMapMutex);
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
    ETH_REQUIRE(socketMap.count(_threadID));
    sessionInfo& element = socketMap.at(_threadID);
    test::pclose2(element.filePipe.get(), element.pipePid);
    std::this_thread::sleep_for(std::chrono::seconds(4));
    boost::filesystem::remove_all(boost::filesystem::path(element.tmpDir));
    element.filePipe.release();
    element.session.release();
}

void RPCSession::clear()
{
    std::lock_guard<std::mutex> lock(g_socketMapMutex);
    std::vector<thread> closingThreads;
    for (auto& element : socketMap)
        closingThreads.push_back(std::move(thread(closeSession, element.first)));
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
	return test::scheme_block(test::convertJsonCPPtoData(response));
}

test::scheme_transactionReceipt RPCSession::eth_getTransactionReceipt(string const& _transactionHash)
{
	Json::Value const result = rpcCall("eth_getTransactionReceipt", { quote(_transactionHash) });
	return test::scheme_transactionReceipt(test::convertJsonCPPtoData(result));
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
	return rpcCall("eth_sendRawTransaction", { quote(_rlp) }).asString();
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
    ETH_REQUIRE(Json::Reader().parse(c_configString, config));
	for (auto const& account: _accounts)
		config["accounts"][account]["wei"] = "0x100000000000000000000000000000000000000000";
	test_setChainParams(Json::FastWriter().write(config));
}

string RPCSession::test_getLogHash(std::string const& _txHash)
{
	return rpcCall("test_getLogHash", { quote(_txHash) }).asString();
}

void RPCSession::test_setChainParams(string const& _config)
{
    ETH_REQUIRE(rpcCall("test_setChainParams", { _config }) == true);
}

void RPCSession::test_rewindToBlock(size_t _blockNr)
{
    ETH_REQUIRE(rpcCall("test_rewindToBlock", { to_string(_blockNr) }) == true);
}

void RPCSession::test_mineBlocks(int _number)
{
	u256 startBlock = fromBigEndian<u256>(fromHex(rpcCall("eth_blockNumber").asString()));
    ETH_REQUIRE(rpcCall("test_mineBlocks", { to_string(_number) }, true) == true);

	// We auto-calibrate the time it takes to mine the transaction.
	// It would be better to go without polling, but that would probably need a change to the test client

	auto startTime = std::chrono::steady_clock::now();
	unsigned sleepTime = m_sleepTime;
	size_t tries = 0;
	for (; ; ++tries)
	{
		if (ExitHandler::shouldExit())
			break;

		std::this_thread::sleep_for(chrono::milliseconds(sleepTime));
		auto endTime = std::chrono::steady_clock::now();
		unsigned timeSpent = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();
		if (timeSpent > m_maxMiningTime)
			break; // could be that some blocks are invalid.
			//ETH_FAIL("Error in test_mineBlocks: block mining timeout! " + test::TestOutputHelper::get().testName());

		bigint number = fromBigEndian<u256>(fromHex(rpcCall("eth_blockNumber").asString()));
		if (number >= startBlock + _number)
			break;
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
    ETH_REQUIRE(rpcCall("test_modifyTimestamp", { to_string(_timestamp) }) == true);
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
	string reply = m_ipcSocket.sendRequest(request);
    ETH_TEST_MESSAGE("Reply: " + reply);

	Json::Value result;
    ETH_REQUIRE(Json::Reader().parse(reply, result, false));

	if (result.isMember("error"))
	{
		if (_canFail)
			return Json::Value();

		ETH_FAIL("Error on JSON-RPC call (" + test::TestOutputHelper::get().testName() + "): "
		 + result["error"]["message"].asString()
		 + " Request: " + request);
	}
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

RPCSession::RPCSession(const string& _path):
	m_ipcSocket(_path)
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

