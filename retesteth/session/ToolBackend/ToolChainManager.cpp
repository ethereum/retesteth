#include "ToolChainManager.h"
#include <retesteth/EthChecks.h>

namespace toolimpl
{
ToolChainManager::ToolChainManager(SetChainParamsArgs const& _config, fs::path const& _toolPath)
{
    m_currentChain = 0;
    m_chains[m_currentChain] =
        spToolChain(new ToolChain(_config.genesis(), _config.state(), _config.sealEngine(), _config.fork(), _toolPath));
    m_pendingBlock = spEthereumBlock(new EthereumBlock(currentChain().lastBlock().header()));
    m_pendingBlock.getContent().addState(_config.state());
}

void ToolChainManager::mineBlocks(size_t _number)
{
    assert(_number < 2 && _number > 0);
    latestChain().mineBlock(m_pendingBlock.getCContent());
}


EthGetBlockBy ToolChainManager::getBlockByNumber(VALUE const& _blockNumber) const
{
    auto const& blocks = currentChain().blocks();
    if (_blockNumber < blocks.size())
    {
        EthereumBlock const& block = blocks.at((size_t)_blockNumber.asU256());
        return internalConstructResponseBlock(block);
    }
    ETH_ERROR_MESSAGE("ToolChainManager::getBlockByNumber block not found: " + _blockNumber.asDecString());
    return EthGetBlockBy(DataObject());
}


// Construct RPC style response
EthGetBlockBy ToolChainManager::internalConstructResponseBlock(EthereumBlock const& _block) const
{
    DataObject constructResponse = _block.header().asDataObject();

    constructResponse["transactions"] = DataObject(DataType::Array);
    for (auto const& tr : _block.transactions())
    {
        DataObject fullTransaction = tr.asDataObject();
        fullTransaction["blockHash"] = FH32::zero().asString();  // We don't know the hash its in tool response
        fullTransaction["blockNumber"] = _block.header().number().asString();
        fullTransaction["from"] = FH20::zero().asString();  // Can be recovered from vrs
        fullTransaction["transactionIndex"] = "0x00";       // Its in tool response
        fullTransaction["hash"] = tr.hash().asString();
        constructResponse["transactions"].addArrayObject(fullTransaction);
    }

    constructResponse["uncles"] = DataObject(DataType::Array);
    for (auto const& un : _block.uncles())
        constructResponse["uncles"].addArrayObject(un.hash().asString());

    constructResponse["size"] = "0x00";
    constructResponse["totalDifficulty"] = "0x00";
    constructResponse.renameKey("bloom", "logsBloom");
    constructResponse.renameKey("coinbase", "miner");
    constructResponse.renameKey("receiptTrie", "receiptsRoot");
    constructResponse.renameKey("transactionsTrie", "transactionsRoot");
    constructResponse.renameKey("uncleHash", "sha3Uncles");

    ETH_TEST_MESSAGE("Response: eth_getBlockByNumber " + constructResponse.asJson());
    return EthGetBlockBy(constructResponse);
}

}  // namespace toolimpl
