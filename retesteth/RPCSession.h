#pragma once

#include <json/value.h>
#include <boost/noncopyable.hpp>
#include <boost/test/unit_test.hpp>

#include <string>
#include <stdio.h>
#include <map>

#include <libdevcore/CommonData.h>
#include <libdevcore/Common.h>
#include <retesteth/ethObjects/common.h>
#include <retesteth/Socket.h>

class RPCSession: public boost::noncopyable
{
public:
    struct TransactionData  // remove this
    {
		std::string from;
		std::string to;
		std::string gas;
		std::string gasPrice;
		std::string value;
		std::string data;

		std::string toJson() const;
	};

    enum SessionStatus
    {
        Working,      // test execution in progress
        Available,    // test execution has finished
        HasFinished,  // has just finished execution
        NotExist      // socket yet not initialized
    };

    static RPCSession& instance(std::string const& _threadID);
    static void sessionStart(std::string const &_threadID);
    static void sessionEnd(std::string const& _threadID, SessionStatus _status);
    static SessionStatus sessionStatus(std::string const& _threadID);
    static void clear();

	std::string web3_clientVersion();
	std::string eth_call(TransactionData const& _td, std::string const& _blockNumber);
	std::string eth_sendTransaction(TransactionData const& _td);
	std::string eth_sendTransaction(std::string const& _transaction);
	std::string eth_sendRawTransaction(std::string const& _rlp);

	std::string eth_blockNumber();
	test::scheme_transactionReceipt eth_getTransactionReceipt(std::string const& _transactionHash);
	std::string eth_getTransactionCount(std::string const& _address, std::string const& _blockNumber);
	std::string eth_getCode(std::string const& _address, std::string const& _blockNumber);
	test::scheme_block eth_getBlockByNumber(std::string const& _blockNumber, bool _fullObjects);
	std::string eth_getBalance(std::string const& _address, std::string const& _blockNumber);
	std::string eth_getStorageRoot(std::string const& _address, std::string const& _blockNumber);
	std::string eth_getStorageAt(std::string const& _address, std::string const& _position, std::string const& _blockNumber);

	std::string personal_newAccount(std::string const& _password);
	void personal_unlockAccount(std::string const& _address, std::string const& _password, int _duration);
	Json::Value debug_accountRangeAt(std::string const& _blockHashOrNumber, int _txIndex, std::string const& _address, int _maxResults);
	Json::Value debug_storageRangeAt(std::string const& _blockHashOrNumber, int _txIndex, std::string const& _address, std::string const& _begin, int _maxResults);

    std::string test_getBlockStatus(std::string const& _blockHash);
    std::string test_getLogHash(std::string const& _txHash);
	void test_setChainParams(std::vector<std::string> const& _genesis);
	void test_setChainParams(std::string const& _config);
	void test_rewindToBlock(size_t _blockNr);
	void test_modifyTimestamp(size_t _timestamp);
    void test_mineBlocks(int _number, std::string const& _hash = "");
    void test_importRawBlock(std::string const& _blockRLP);

    std::string sendRawRequest(std::string const& _request);
    Json::Value rpcCall(std::string const& _methodName, std::vector<std::string> const& _args = std::vector<std::string>(), bool _canFail = false);

	std::string const& account(size_t _id) const { return m_accounts.at(_id); }
	std::string const& accountCreate();
	std::string const& accountCreateIfNotExists(size_t _id);

    /// Returns empty string if last RPC call had no errors, error string if there was an error
    std::string const& getLastRPCError() const { return m_lastRPCErrorString; }
    Socket::SocketType getSocketType() const { return m_socket.type(); }

private:
    explicit RPCSession(Socket::SocketType _type, std::string const& _path);
    static void runNewInstanceOfAClient(std::string const& _threadID, ClientConfig const& _config);

    inline std::string quote(std::string const& _arg) { return "\"" + _arg + "\""; }
	/// Parse std::string replacing keywords to values
	void parseString(std::string& _string, std::map<std::string, std::string> const& _varMap);

    Socket m_socket;
	size_t m_rpcSequence = 1;
    unsigned m_maxMiningTime = 250000;    // should be instant with --test (1 sec)
    unsigned m_sleepTime = 10;            // 10 milliseconds
	unsigned m_successfulMineRuns = 0;
    string m_lastRPCErrorString;          // last RPC error string

	std::vector<std::string> m_accounts;
};


