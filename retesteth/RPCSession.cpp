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

#include <string>
#include <stdio.h>
#include <thread>
#include <chrono>
#include <cstdio>
#include <mutex>
#include <csignal>

#include <dataObject/ConvertFile.h>
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
string const RPCSession::c_errorMiningString = "remote test_mineBlocks = false";

std::mutex g_socketMapMutex;
static std::map<std::string, sessionInfo> socketMap;
void RPCSession::runNewInstanceOfAClient(string const& _threadID, ClientConfig const& _config)
{
    if (_config.getSocketType() == Socket::IPC)
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
    else if (_config.getSocketType() == Socket::TCP)
    {
        std::lock_guard<std::mutex> lock(g_socketMapMutex);  // function must be called from lock

        // Create sessionInfo for a tcp address that is still not present in socketMap
        for (auto const& addr : _config.getAddressObject().getSubObjects())
        {
            bool unused = true;
            for (auto const& socket : socketMap)
            {
                sessionInfo const& sInfo = socket.second;
                if (sInfo.session.get()->getSocketPath() == addr.asString())
                {
                    unused = false;
                    break;
                }
            }
            if (unused)
            {
                sessionInfo info(NULL, new RPCSession(Socket::SocketType::TCP, addr.asString()), "",
                    0, _config.getId());
                socketMap.insert(std::pair<string, sessionInfo>(_threadID, std::move(info)));
                return;
            }
        }
    }
    else if (_config.getSocketType() == Socket::IPCDebug)
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
        "Something went wrong. Retesteth connect to more instances than needed!");
    ETH_FAIL_REQUIRE_MESSAGE(socketMap.size() != 0,
        "Something went wrong. Retesteth failed to create socket connection!");
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

scheme_debugAccountRange RPCSession::debug_accountRange(
    std::string const& _blockHashOrNumber, int _txIndex, string const& _address, int _maxResults)
{
    return scheme_debugAccountRange(rpcCall(
        "debug_accountRange", {quote(toString(u256(dev::fromHex(_blockHashOrNumber)))),
                                  to_string(_txIndex), quote(_address), to_string(_maxResults)}));
}

DataObject RPCSession::debug_storageRangeAt(std::string const& _blockHashOrNumber, int _txIndex,
    string const& _address, string const& _begin, int _maxResults)
{
    return rpcCall("debug_storageRangeAt",
        {quote(toString(u256(dev::fromHex(_blockHashOrNumber)))), to_string(_txIndex),
            quote(_address), quote(_begin), to_string(_maxResults)});
}

scheme_debugTraceTransaction RPCSession::debug_traceTransaction(std::string const& _trHash)
{
    return scheme_debugTraceTransaction(rpcCall("debug_traceTransaction", {quote(_trHash), "{}"}));
}

string RPCSession::web3_clientVersion()
{
	return rpcCall("web3_clientVersion", { }).asString();
}

string RPCSession::eth_getCode(string const& _address, string const& _blockNumber)
{
	return rpcCall("eth_getCode", { quote(_address), quote(_blockNumber) }).asString();
}

test::scheme_block RPCSession::eth_getBlockByNumber(
    BlockNumber const& _blockNumber, bool _fullObjects)
{
    return test::scheme_block(rpcCall("eth_getBlockByNumber",
        {quote(_blockNumber.getBlockNumberAsString()), _fullObjects ? "true" : "false"}));
}

test::scheme_block RPCSession::eth_getBlockByHash(string const& _blockHash, bool _fullObjects)
{
    return test::scheme_block(
        rpcCall("eth_getBlockByHash", {quote(_blockHash), _fullObjects ? "true" : "false"}));
}

test::scheme_transactionReceipt RPCSession::eth_getTransactionReceipt(string const& _transactionHash)
{
    return test::scheme_transactionReceipt(
        rpcCall("eth_getTransactionReceipt", {quote(_transactionHash)}));
}

string RPCSession::eth_blockNumber()
{
    DataObject res = rpcCall("eth_blockNumber", {});
    return res.type() == DataType::String ? res.asString() : toString(res.asInt());
}

string RPCSession::eth_sendTransaction(string const& _transaction)
{
	return rpcCall("eth_sendTransaction", { _transaction }).asString();
}

string RPCSession::eth_sendRawTransaction(std::string const& _rlp)
{
    DataObject result = rpcCall("eth_sendRawTransaction", {quote(_rlp)}, true);

    DataObject const& lastError = getLastRPCError();
    if (lastError.type() != DataType::Null)
        ETH_ERROR_MESSAGE(lastError.atKey("message").asString());
    if (!isHash<h256>(result.asString()))
        ETH_ERROR_MESSAGE("eth_sendRawTransaction return invalid hash: '" + result.asString() + "'");
    if (result.type() == DataType::Null)  // if the method failed
        return "";
    return result.asString();
}

int RPCSession::eth_getTransactionCount(
    std::string const& _address, std::string const& _blockNumber)
{
    DataObject res = rpcCall("eth_getTransactionCount", {quote(_address), quote(_blockNumber)});
    return (res.type() == DataType::String) ? test::hexOrDecStringToInt(res.asString()) :
                                              res.asInt();
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

string RPCSession::test_getBlockStatus(std::string const& _blockHash)
{
    return rpcCall("test_getBlockStatus", {quote(_blockHash)}).asString();
}

string RPCSession::test_getLogHash(std::string const& _txHash)
{
	return rpcCall("test_getLogHash", { quote(_txHash) }).asString();
}

string RPCSession::test_importRawBlock(std::string const& _blockRLP)
{
    DataObject res = rpcCall("test_importRawBlock", {quote(_blockRLP)}, true);
    if (res.type() != DataType::Null)
        return res.asString();
    return string();
}

void RPCSession::test_setChainParams(string const& _config)
{
    ETH_FAIL_REQUIRE_MESSAGE(rpcCall("test_setChainParams", { _config }) == true, "remote test_setChainParams = false");
}

void RPCSession::test_rewindToBlock(size_t _blockNr)
{
    ETH_FAIL_REQUIRE_MESSAGE(rpcCall("test_rewindToBlock", { to_string(_blockNr) }) == true, "remote test_rewintToBlock = false");
}

string RPCSession::test_mineBlocks(int _number, bool _canFail)
{
    DataObject blockNumber = rpcCall("eth_blockNumber");
    u256 startBlock = (blockNumber.type() == DataType::String) ? u256(blockNumber.asString()) :
                                                                 blockNumber.asInt();

    if (!_canFail)
        ETH_ERROR_REQUIRE_MESSAGE(rpcCall("test_mineBlocks", { to_string(_number) }, true) == true, c_errorMiningString);
    else
        return c_errorMiningString;

    // We auto-calibrate the time it takes to mine the transaction.
    // It would be better to go without polling, but that would probably need a change to the test
    // client

    auto startTime = std::chrono::steady_clock::now();
    unsigned sleepTime = m_sleepTime;
    size_t tries = 0;
    for (;; ++tries)
    {
        std::this_thread::sleep_for(chrono::milliseconds(sleepTime));
        auto endTime = std::chrono::steady_clock::now();
        unsigned timeSpent =
            std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();
        if (timeSpent > m_maxMiningTime)
            break;  // could be that some blocks are invalid.
        // ETH_FAIL_MESSAGE("Error in test_mineBlocks: block mining timeout! " +
        // test::TestOutputHelper::get().testName());

        blockNumber = rpcCall("eth_blockNumber");
        u256 number = (blockNumber.type() == DataType::String) ? u256(blockNumber.asString()) :
                                                                 blockNumber.asInt();
        if (number >= startBlock + _number)
            return toString(number);
        else
            sleepTime *= 2;

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

    // Better keep it int everywhere in codebase. !!!
    return toString(startBlock);
}

void RPCSession::test_modifyTimestamp(unsigned long long _timestamp)
{
    ETH_FAIL_REQUIRE_MESSAGE(rpcCall("test_modifyTimestamp", { to_string(_timestamp) }) == true, "test_modifyTimestamp was not successfull");
}

std::string RPCSession::sendRawRequest(string const& _request)
{
    JsonObjectValidator validator;
    return m_socket.sendRequest(_request, validator);
}

DataObject RPCSession::rpcCall(
    string const& _methodName, vector<string> const& _args, bool _canFail)
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
    JsonObjectValidator validator;  // read response while counting `{}`
    string reply = m_socket.sendRequest(request, validator);
    ETH_TEST_MESSAGE("Reply: " + reply);

    DataObject result = ConvertJsoncppStringToData(reply, string(), true);
    if (result.count("error"))
        result["result"] = "";
    requireJsonFields(result, "rpcCall_response ('" + request.substr(0, 70) + "')",
        {{"jsonrpc", {{DataType::String}, jsonField::Required}},
            {"id", {{DataType::Integer}, jsonField::Required}},
            {"result", {{DataType::String, DataType::Integer, DataType::Bool, DataType::Object,
                            DataType::Array},
                           jsonField::Required}},
            {"error", {{DataType::String, DataType::Object}, jsonField::Optional}}});

    if (result.count("error"))
    {
        test::TestOutputHelper const& helper = test::TestOutputHelper::get();
        m_lastRPCError["message"] = "Error on JSON-RPC call (" + helper.testInfo().getMessage() +
                                    "):\nRequest: '" + request + "'" + "\nResult: '" +
                                    result["error"]["message"].asString() + "'\n";
        m_lastRPCError["error"] = result["error"]["message"].asString();
        if (_canFail)
            return DataObject(DataType::Null);
        ETH_FAIL_MESSAGE(m_lastRPCError.atKey("message").asString());
    }
    m_lastRPCError.clear();    //null the error as last RPC call was success.
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

RPCSession::RPCSession(Socket::SocketType _type, const string& _path) : m_socket(_type, _path) {}
