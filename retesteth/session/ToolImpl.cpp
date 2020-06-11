#include <chrono>
#include <cstdio>
#include <thread>

#include <dataObject/ConvertFile.h>
#include <retesteth/Options.h>
#include <retesteth/session/ToolImpl.h>
#include <retesteth/testStructures/types/BlockchainTests/Filler/BlockchainTestFillerEnv.h>

#include "ToolBackend/ToolImplHelper.h"

using namespace test;
using namespace toolimpl;

DataObject ToolImpl::web3_clientVersion()
{
    rpcCall("", {});
    ETH_TEST_MESSAGE("\nRequest: web3_clientVersion");
    string const cmd = m_toolPath.string() + " -v";
    DataObject res(test::executeCmd(cmd));
    ETH_TEST_MESSAGE("\nResponse: web3_clientVersion " + res.asString());
    return res;
}

// ETH Methods
FH32 ToolImpl::eth_sendRawTransaction(BYTES const& _rlp)
{
    rpcCall("", {});
    ETH_TEST_MESSAGE("\nRequest: eth_sendRawTransaction \n" + _rlp.asString());
    Transaction tr(_rlp);
    m_toolChainManager.getContent().addPendingTransaction(tr);
    FH32 trHash = tr.hash();
    ETH_TEST_MESSAGE("Response: " + trHash.asString());
    return trHash;
}

size_t ToolImpl::eth_getTransactionCount(FH20 const& _address, VALUE const& _blockNumber)
{
    rpcCall("", {});
    ETH_TEST_MESSAGE("\nRequest: eth_getTransactionCount " + _blockNumber.asString() + " " + _address.asString());
    VALUE const& nonce = blockchain().blockByNumber(_blockNumber).state().getAccount(_address).nonce();
    ETH_TEST_MESSAGE("Response: eth_getTransactionCount " + nonce.asDecString());
    return (size_t)nonce.asU256();
}

VALUE ToolImpl::eth_blockNumber()
{
    rpcCall("", {});
    ETH_TEST_MESSAGE("\nRequest: eth_blockNumber");
    string const snumber = m_toolChainManager.getContent().lastBlock().header().number().asDecString();
    ETH_TEST_MESSAGE("Response: eth_blockNumber {" + snumber + "}");
    return m_toolChainManager.getContent().lastBlock().header().number();
}


EthGetBlockBy ToolImpl::eth_getBlockByHash(FH32 const& _hash, Request _fullObjects)
{
    rpcCall("", {});
    (void)_fullObjects;
    ETH_TEST_MESSAGE("\nRequest: eth_getBlockByHash `" + _hash.asString());
    DataObject res = constructEthGetBlockBy(blockchain().blockByHash(_hash));
    ETH_LOG("Response: eth_getBlockByHash `" + res.asJson(), 7);
    return EthGetBlockBy(res);
}

EthGetBlockBy ToolImpl::eth_getBlockByNumber(VALUE const& _blockNumber, Request _fullObjects)
{
    rpcCall("", {});
    (void)_fullObjects;
    ETH_TEST_MESSAGE("\nRequest: eth_getBlockByNumber `" + _blockNumber.asDecString());
    DataObject res = constructEthGetBlockBy(blockchain().blockByNumber(_blockNumber));
    ETH_LOG("Response: eth_getBlockByNumber `" + res.asJson(), 7);
    return EthGetBlockBy(res);
}

BYTES ToolImpl::eth_getCode(FH20 const& _address, VALUE const& _blockNumber)
{
    rpcCall("", {});
    ETH_TEST_MESSAGE("\nRequest: eth_getCode " + _blockNumber.asString() + " " + _address.asString());
    BYTES const& code = blockchain().blockByNumber(_blockNumber).state().getAccount(_address).code();
    ETH_TEST_MESSAGE("Response: eth_getCode " + code.asString());
    return code;
}

VALUE ToolImpl::eth_getBalance(FH20 const& _address, VALUE const& _blockNumber)
{
    rpcCall("", {});
    ETH_TEST_MESSAGE("\nRequest: eth_getBalance " + _blockNumber.asString() + " " + _address.asString());
    VALUE const& balance = blockchain().blockByNumber(_blockNumber).state().getAccount(_address).balance();
    ETH_TEST_MESSAGE("Response: eth_getBalance " + balance.asDecString());
    return balance;
}

// Debug
DebugAccountRange ToolImpl::debug_accountRange(
    VALUE const& _blockNumber, VALUE const& _txIndex, FH32 const& _addressHash, size_t _maxResults)
{
    rpcCall("", {});
    ETH_TEST_MESSAGE("\nRequest: debug_accountRange `" + _blockNumber.asDecString() + " " + _addressHash.asString());

    (void)_txIndex;
    (void)_addressHash;
    DataObject constructResponse = constructAccountRange(blockchain().blockByNumber(_blockNumber), _addressHash, _maxResults);
    ETH_TEST_MESSAGE("Response: debug_accountRange " + constructResponse.asJson());
    return DebugAccountRange(constructResponse);
}

DebugAccountRange ToolImpl::debug_accountRange(
    FH32 const& _blockHash, VALUE const& _txIndex, FH32 const& _addressHash, size_t _maxResults)
{
    rpcCall("", {});
    ETH_TEST_MESSAGE("\nRequest: debug_accountRange " + _blockHash.asString() + " " + _addressHash.asString());

    (void)_txIndex;
    (void)_addressHash;
    DataObject constructResponse = constructAccountRange(blockchain().blockByHash(_blockHash), _addressHash, _maxResults);
    ETH_TEST_MESSAGE("Response: debug_accountRange " + constructResponse.asJson());
    return DebugAccountRange(constructResponse);
}

DebugStorageRangeAt ToolImpl::debug_storageRangeAt(
    VALUE const& _blockNumber, VALUE const& _txIndex, FH20 const& _address, FH32 const& _begin, int _maxResults)
{
    rpcCall("", {});
    ETH_TEST_MESSAGE("\nRequest: debug_storageRangeAt bl:" + _blockNumber.asDecString() + " ind:" + _begin.asString() +
                     " addr:" + _address.asString());
    DataObject constructResponse =
        constructStorageRangeAt(blockchain().blockByNumber(_blockNumber), _address, _begin, _maxResults);
    (void)_txIndex;
    (void)_maxResults;
    ETH_TEST_MESSAGE("Response: debug_storageRangeAt " + constructResponse.asJson());
    return DebugStorageRangeAt(constructResponse);
}

DebugStorageRangeAt ToolImpl::debug_storageRangeAt(
    FH32 const& _blockHash, VALUE const& _txIndex, FH20 const& _address, FH32 const& _begin, int _maxResults)
{
    rpcCall("", {});
    ETH_TEST_MESSAGE("\nRequest: debug_storageRangeAt bl:" + _blockHash.asString() + " ind:" + _begin.asString() +
                     " addr:" + _address.asString());
    DataObject constructResponse = constructStorageRangeAt(blockchain().blockByHash(_blockHash), _address, _begin, _maxResults);
    (void)_txIndex;
    (void)_maxResults;
    ETH_TEST_MESSAGE("Response: debug_storageRangeAt " + constructResponse.asJson());
    return DebugStorageRangeAt(constructResponse);
}

DebugTraceTransaction ToolImpl::debug_traceTransaction(FH32 const& _trHash)
{
    rpcCall("", {});
    (void)_trHash;
    ETH_FAIL_MESSAGE("\nRequest: debug_traceTransaction");
    return DebugTraceTransaction(DataObject());
}

// Test
void ToolImpl::test_setChainParams(SetChainParamsArgs const& _config)
{
    rpcCall("", {});
    ETH_TEST_MESSAGE("\nRequest: test_setChainParams \n" + _config.asDataObject().asJson());

    // Ask tool to calculate genesis header stateRoot for genesisHeader
    EthereumBlock genesis(BlockHeader(_config.genesis()));
    m_toolChainManager = GCP_SPointer<ToolChainManager>(new ToolChainManager(_config, m_toolPath));

    ETH_TEST_MESSAGE("Response test_setChainParams: {true}");
}

void ToolImpl::test_rewindToBlock(VALUE const& _blockNr)
{
    rpcCall("", {});
    ETH_TEST_MESSAGE("\nRequest: test_rewindToBlock " + _blockNr.asDecString());
    blockchain().rewindToBlock(_blockNr);
    ETH_TEST_MESSAGE("Response: test_rewindToBlock: " + blockchain().lastBlock().header().number().asDecString());
}

void ToolImpl::test_modifyTimestamp(VALUE const& _timestamp)
{
    rpcCall("", {});
    ETH_TEST_MESSAGE("\nRequest: test_modifyTimestamp " + _timestamp.asDecString());
    blockchain().modifyTimestamp(_timestamp);
    ETH_TEST_MESSAGE("\nResponse: test_modifyTimestamp " + _timestamp.asDecString());
}

void ToolImpl::test_mineBlocks(size_t _number)
{
    rpcCall("", {});
    ETH_TEST_MESSAGE("\nRequest: test_mineBlocks");
    m_toolChainManager.getContent().mineBlocks(_number);

    ETH_TEST_MESSAGE("Response test_mineBlocks {" +
                     toString(m_toolChainManager.getContent().lastBlock().header().number().asDecString()) + "}");

    /*
    ETH_ERROR_REQUIRE_MESSAGE(_number == 1, "Make sure test_mineBlocks mine 1 block");

    // env.json file
    // if mining a new block (not from import RawBlock)
    if (!m_currentBlockHeader.isImportRawBlock && !m_currentBlockHeader.isMiningGenesis)
    {
        // Information about the block that currently mined
        // if m_blockchain.size == 0 then we mine [1] first block
        // if m_blockchain.size == 1 then we mine [2] second block
        m_currentBlockHeader.currentBlockNumber = getCurrChain().size() + 1;  // future block
        size_t const _blockNr = m_currentBlockHeader.currentBlockNumber;
        string const* pHash;
        if (_blockNr == 1)  // if we are mining blockN = 1
            pHash = &getGenesis().getHash();
        else
            pHash = &getLastBlock().getHash();
        m_currentBlockHeader.header["parentHash"] = *pHash;
    }
    fs::path envPath = m_tmpDir / "env.json";
    writeFile(envPath.string(), prepareEnvForTool());

    // output file
    fs::path outPath = m_tmpDir / "out.json";
    fs::path outAllocPath = m_tmpDir / "outAlloc.json";

    // If calculating geneis block, disable rewards so to see the state root hash

    // Construct block rpc response
    DataObject const toolResponse = ConvertJsoncppStringToData(contentsString(outPath));
    scheme_RPCBlock blockRPC = internalConstructResponseGetBlockByHashOrNumber(toolResponse);

    ToolBlock block(blockRPC, m_chainParams,                        // Env, alloc info
        ConvertJsoncppStringToData(contentsString(outAllocPath)));  // Result state
    if (toolResponse.count("rejected"))
        block.markInvalidTransactions();

    m_transactions.clear();  // comment to trigger transaction rejection

    if (!m_currentBlockHeader.isMiningGenesis)
        m_currentBlockHeader.reset();

    // TODO:: verify tx from our list and from tool response

    m_blockchainMap[m_current_chain_ind].push_back(block);
    fs::remove_all(m_tmpDir);
    ETH_TEST_MESSAGE("Response test_mineBlocks {" + toString(getCurrChain().size()) + "}");
    ETH_TEST_MESSAGE(blockRPC.getData().asJson());
    return toString(getCurrChain().size());
    */
}

// Import block from RAW rlp and validate it according to ethereum rules
// Very logic heavy function. Must be on the client side. Its a clien logic.
FH32 ToolImpl::test_importRawBlock(BYTES const& _blockRLP)
{
    rpcCall("", {});
    ETH_TEST_MESSAGE("\nRequest: test_importRawBlock, following transaction import are internal");
    (void)_blockRLP;

    /*
    static vector<string> exceptions = {"BlockWrongStoreClears"};
    for (auto const& el : exceptions)
    {
        if (TestOutputHelper::get().testName().find(el) != string::npos)
        {
            ETH_WARNING("t8ntool backend can't verify this block!");
            onError("TooImpl can't verify this block!", "Test exception");
            return string();
        }
    }

    string rawBlockHash;
    try
    {
        bytes blockBytes = dev::fromHex(_blockRLP);
        dev::RLP blockRLP(blockBytes);
        ostringstream out;
        out << blockRLP;
        ETH_TEST_MESSAGE(out.str());
        rawBlockHash = dev::toHexPrefixed(dev::sha3(blockRLP[0].data()));
        if (!blockRLP.isList())
            throw dev::RLPException("Block RLP is expected to be list");

        BlockHeadFromRLP sanHeader(blockRLP[0]);
        verifyRawBlock(sanHeader, blockRLP);

        test_mineBlocks(1);

        ToolBlock const& cbl = getLastBlock();

        auto revertLastBlockWithException = [&](string const& _what) {
            m_blockchainMap.at(m_current_chain_ind).pop_back();
            throw dev::RLPException(_what);
        };

        // Evm return different field
        const scheme_blockHeader t8nHeader = cbl.getRPCResponse().getBlockHeader2();
        if (t8nHeader.bloom() != sanHeader.header.bloom())
            revertLastBlockWithException(
                "Raw block bloom is different to one returned by t8ntool!");
        if (t8nHeader.stateRoot() != sanHeader.header.stateRoot())
            revertLastBlockWithException(
                "Raw block stateRoot is different to one returned by t8ntool!");


        // When import from raw block, no invalid transactions (tool::rejected) allowed
        if (cbl.wereInvalidTransactions())
            revertLastBlockWithException("Raw block transaction execution failed!");

        if (rawBlockHash != cbl.getHash())
            throw dev::RLPException(
                string("RAW import hash != test_mineBlocks imitating rawblock. ") +
                "RAW: " + rawBlockHash + " vs " + cbl.getHash());
    }
    catch (test::BaseEthException const& _ex)
    {
        // do not treat this exception as retesteth error
        TestOutputHelper::get().unmarkLastError();
        onError(_ex.what(), "Import raw block failed");
        ETH_TEST_MESSAGE("Response test_importRawBlock: " + rawBlockHash);
        return string();
    }
    catch (RLPException const& _ex)
    {
        onError(_ex.what(), "Import raw block failed");
        ETH_TEST_MESSAGE("Response test_importRawBlock: " + rawBlockHash);
        return string();
    }

    doChainReorg();  // update head
    ETH_TEST_MESSAGE("Response test_importRawBlock: " + rawBlockHash);
    return rawBlockHash;
    */
    return FH32::zero();
}

FH32 ToolImpl::test_getLogHash(FH32 const& _txHash)
{
    rpcCall("", {});
    (void)_txHash;
    ETH_TEST_MESSAGE("\nRequest: test_getLogHash " + _txHash.asString());
    FH32 const& res = blockchain().lastBlock().logHash();
    ETH_TEST_MESSAGE("Response: test_getLogHash " + res.asString());
    return res;
}


// Internal
DataObject ToolImpl::rpcCall(
    std::string const& _methodName, std::vector<std::string> const& _args, bool _canFail)
{
    m_lastInterfaceError.clear();
    if (Options::get().rpcLimit != 0 && Options::get().rpcLimit < m_totalCalls++)
        ETH_FAIL_MESSAGE("\nRequest: rpcCall of test limit");
    (void)_methodName;
    (void)_args;
    (void)_canFail;
    return DataObject();
}

Socket::SocketType ToolImpl::getSocketType() const
{
    return m_sockType;
}

std::string const& ToolImpl::getSocketPath() const
{
    return m_toolPath.string();
}
