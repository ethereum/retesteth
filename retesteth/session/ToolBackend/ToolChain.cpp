#include "BlockMining.h"
#include "ToolChainHelper.h"
#include "ToolChainManager.h"
#include <Options.h>
#include <dataObject/ConvertFile.h>
#include <libdevcore/CommonIO.h>
#include <testStructures/Common.h>
#include <testStructures/types/BlockchainTests/Filler/BlockchainTestFillerEnv.h>
#include <testStructures/types/RPC/DebugVMTrace.h>
#include <testStructures/types/RPC/ToolResponse.h>

using namespace dev;
using namespace test;
using namespace teststruct;
using namespace dataobject;

namespace  {
void correctHeaderByToolResponse(BlockHeader& _header, ToolResponse const& _res)
{
    // Update a block header with information that we have and what we get from t8ntool
    _header.setStateRoot(_res.stateRoot());
    _header.setGasUsed(_res.totalGasUsed());
    _header.setTransactionHash(_res.txRoot());
    _header.setTrReceiptsHash(_res.receiptRoot());
    _header.setLogsBloom(_res.logsBloom());
    _header.setStateRoot(_res.stateRoot());
    _header.setDifficulty(_res.currentDifficulty());
}
}

namespace toolimpl
{
ToolChain::ToolChain(
    EthereumBlockState const& _genesis, spSetChainParamsArgs const& _config, fs::path const& _toolPath, fs::path const& _tmpDir)
  : m_initialParams(_config),
    m_engine(_config->sealEngine()),
    m_fork(new FORK(_config->params().atKey("fork"))),
    m_toolPath(_toolPath),
    m_tmpDir(_tmpDir)
{
    m_toolParams = GCP_SPointer<ToolParams>(new ToolParams(_config->params()));

    auto const& additionalForks = Options::getCurrentConfig().cfgFile().additionalForks();
    if (!inArray(additionalForks, m_fork.getCContent()))
    {
        if (compareFork(m_fork, CMP::lt, FORK("London"))
            && _genesis.header()->type() == BlockType::BlockHeader1559)
            throw test::UpwardsException("Constructing 1559 genesis on network which is lower London!");
        if (compareFork(m_fork, CMP::ge, FORK("London"))
            && _genesis.header()->type() != BlockType::BlockHeader1559)
            throw test::UpwardsException("Constructing legacy genesis on network which is higher London!");
    }

    // We yet don't know the state root of genesis. Ask the tool to calculate it
    ToolResponse const res = mineBlockOnTool(_genesis, _genesis, SealEngine::NoReward);

    EthereumBlockState genesisFixed(_genesis.header(), _genesis.state(), FH32::zero());
    genesisFixed.headerUnsafe().getContent().setStateRoot(res.stateRoot());
    genesisFixed.headerUnsafe().getContent().recalculateHash();
    genesisFixed.setTotalDifficulty(genesisFixed.header()->difficulty());
    m_blocks.push_back(genesisFixed);
}

ToolChain::ToolChain(
    EthereumBlockState const& _parentBlock, EthereumBlockState const& _currentBlock,
    FORK const& _fork, fs::path const& _toolPath, fs::path const& _tmpDir)
  : m_initialParams(0),
    m_engine(SealEngine::NoProof),
    m_fork(new FORK(_fork.asString())),
    m_toolPath(_toolPath),
    m_tmpDir(_tmpDir)
{
    // Calculate the difficutly of _currentBlock given _parentBlock
    ToolResponse res = mineBlockOnTool(_currentBlock, _parentBlock, SealEngine::NoReward);
    m_blocks.push_back(_currentBlock);
    m_blocks.back().headerUnsafe().getContent().setDifficulty(res.currentDifficulty());
}


spDataObject const ToolChain::mineBlock(EthereumBlockState const& _pendingBlock, EthereumBlockState const& _parentBlock, Mining _req)
{
    // Ask the tool to calculate post state and block header
    // With current chain information, txs from pending block
    ToolResponse const res = mineBlockOnTool(_pendingBlock, _parentBlock, m_engine);

    // Pending fixed is pending header corrected by the information returned by tool
    // The tool can reject transactions changing the stateHash, TxRoot, TxReceipts, HeaderHash, GasUsed
    EthereumBlockState pendingFixed(_pendingBlock.header(), res.state(), res.logsHash());

    BlockHeader& pendingFixedHeader = pendingFixed.headerUnsafe().getContent();
    pendingFixedHeader.setNumber(m_blocks.size());
    correctHeaderByToolResponse(pendingFixedHeader, res);
    checkDifficultyAgainstRetesteth(res.currentDifficulty(), pendingFixed.header());
    calculateAndSetBaseFee(pendingFixed.headerUnsafe(), lastBlock().header());

    // Add only those transactions which tool returned a receipt for
    // Some transactions are expected to fail. That should be detected by tests
    size_t index = 0;
    spDataObject miningResult;
    (*miningResult)["result"] = true;

    for (auto const& tr : _pendingBlock.transactions())
    {
        bool found = false;
        FH32 const trHash = tr->hash();
        for (auto const& trReceipt : res.receipts())
        {
            if (trReceipt.trHash() == trHash)
            {
                found = true;
                pendingFixed.addTransaction(tr);
                break;
            }
        }
        if (!found)
        {
            string const message = "t8ntool didn't return a transaction with hash: " + trHash.asString();

            // Find the rejected transaction information
            bool rejectedInfoFound = false;
            for (auto const& el : res.rejected())
            {
                if (el.index() == index)
                {
                    rejectedInfoFound = true;
                    spDataObject rejectInfo;
                    (*rejectInfo)["hash"] = trHash.asString();
                    (*rejectInfo)["error"] = el.error();
                    (*miningResult)["rejectedTransactions"].addArrayObject(rejectInfo);
                    break;
                }
            }
            if (!rejectedInfoFound)
                ETH_ERROR_MESSAGE("tool didn't provide information about rejected transaction");
            if (_req == Mining::AllowFailTransactions)
            {
                ETH_WARNING_TEST(message, 6);
            }
            else
                throw test::UpwardsException(message);
        }
        index++;
    }


    // Treat all uncles as valid as t8ntool does not calculate uncles
    // Uncle header validity as well as RLP logic is checked before
    for (auto const& un : _pendingBlock.uncles())
    {
        verifyEthereumBlockHeader(un, *this);
        pendingFixed.addUncle(un);
    }
    pendingFixed.recalculateUncleHash(); // Rely that only uncle hash is recalculated (simulate t8ntool unclehash)

    // Calculate header hash from header fields (do not recalc tx, un hashes)
    pendingFixed.headerUnsafe().getContent().recalculateHash();

    // Blockchain rules
    verifyEthereumBlockHeader(pendingFixed.header(), *this);

    // Require number from pending block to be equal to actual block number that is imported
    if (_pendingBlock.header()->number() != pendingFixed.header()->number().asBigInt())
        throw test::UpwardsException(string("Block Number from pending block != actual chain height! (") +
                                     _pendingBlock.header()->number().asString() +
                                     " != " + pendingFixed.header()->number().asString() + ")");

    // Require new block timestamp to be > than the previous block timestamp
    if (lastBlock().header()->timestamp().asBigInt() >= pendingFixed.header()->timestamp().asBigInt())
        throw test::UpwardsException("Block Timestamp from pending block <= previous block timestamp!");

    if (_req == Mining::RequireValid)  // called on rawRLP import
    {
        if (m_fork.getContent().asString() == "HomesteadToDaoAt5" && pendingFixed.header()->number() > 4 &&
            pendingFixed.header()->number() < 19 &&
            pendingFixed.header()->extraData().asString() != "0x64616f2d686172642d666f726b")
            throw test::UpwardsException("Dao Extra Data required!");

        spDataObject const pendingH = _pendingBlock.header()->asDataObject();
        spDataObject const pendingFixedH = pendingFixed.header()->asDataObject();
        if (pendingH->asJson(0, false) != pendingFixedH->asJson(0, false))
        {
            string errField;
            string const compare = compareBlockHeaders(pendingH, pendingFixedH, errField);
            throw test::UpwardsException(string("Block from pending block != t8ntool constructed block!\n") +
                                         "Error in field: " + errField + "\n" +
                                         "rawRLP/Pending header  vs  t8ntool header \n" + compare);
        }
    }

    if (pendingFixed.header()->transactionRoot() != res.txRoot())
    {
        ETH_ERROR_MESSAGE(string("ToolChain::mineBlock txRootHash is different to one ruturned by tool \n") +
                          "constructedBlockHash: " + pendingFixed.header()->transactionRoot().asString() +
                          "\n toolTransactionRoot: " + res.txRoot().asString());
    }

    VALUE totalDifficulty(0);
    if (m_blocks.size() > 0)
        totalDifficulty = m_blocks.at(m_blocks.size() - 1).totalDifficulty();
    pendingFixed.setTotalDifficulty(totalDifficulty + pendingFixed.header()->difficulty());

    ETH_LOG("New block N: " + to_string(m_blocks.size()), 6);
    ETH_LOG("New block TD: " + totalDifficulty.asDecString() + " + " +
                pendingFixed.header()->difficulty().asDecString() + " = " +
                pendingFixed.totalDifficulty().asDecString(), 6);

    pendingFixed.setTrsTrace(res.debugTrace());
    m_blocks.push_back(pendingFixed);

    return miningResult;
}

ToolResponse ToolChain::mineBlockOnTool(
    EthereumBlockState const& _currentBlock, EthereumBlockState const& _parentBlock, SealEngine _engine)
{
    BlockMining toolMiner(m_toolPath, m_tmpDir, _currentBlock, _parentBlock, _engine, m_fork, m_blocks);
    toolMiner.prepareEnvFile();
    toolMiner.prepareAllocFile();
    toolMiner.prepareTxnFile();
    toolMiner.executeTransition();
    return toolMiner.readResult();
}

void ToolChain::rewindToBlock(size_t _number)
{
    while (m_blocks.size() > _number + 1)
        m_blocks.pop_back();
}

// Helper functions
void ToolChain::checkDifficultyAgainstRetesteth(VALUE const& _toolDifficulty, spBlockHeader const& _pendingHeader)
{
    // Calculate difficulty for tool (tool does not calculate difficulty)
    ChainOperationParams params = ChainOperationParams::defaultParams(toolParams());
    VALUE retestethDifficulty = calculateEthashDifficulty(params, _pendingHeader, lastBlock().header());
    if (_toolDifficulty != retestethDifficulty)
        ETH_ERROR_MESSAGE("tool vs retesteth difficulty disagree: " + _toolDifficulty.asDecString() + " vs " + retestethDifficulty.asDecString());
}

void ToolChain::calculateAndSetBaseFee(spBlockHeader& _pendingHeader, spBlockHeader const& _parentHeader)
{
    // Calculate new baseFee
    if (_pendingHeader.getCContent().type() == BlockType::BlockHeader1559 &&
        _parentHeader->type() == BlockType::BlockHeader1559)
    {
        BlockHeader1559& pendingFixed1559Header = BlockHeader1559::castFrom(_pendingHeader.getContent());
        ChainOperationParams params = ChainOperationParams::defaultParams(toolParams());
        VALUE baseFee = calculateEIP1559BaseFee(params, _pendingHeader, _parentHeader);
        pendingFixed1559Header.setBaseFee(baseFee);
    }
}
}  // namespace toolimpl
