#include "ToolChainManager.h"
#include <retesteth/EthChecks.h>
#include <retesteth/TestHelper.h>
using namespace test;

namespace toolimpl
{
ToolChainManager::ToolChainManager(SetChainParamsArgs const& _config, fs::path const& _toolPath)
{
    m_currentChain = 0;
    EthereumBlockState genesis(_config.genesis(), _config.state(), FH32::zero());
    m_chains[m_currentChain] = spToolChain(new ToolChain(genesis, _config.sealEngine(), _config.fork(), _toolPath));
    m_pendingBlock =
        spEthereumBlockState(new EthereumBlockState(currentChain().lastBlock().header(), _config.state(), FH32::zero()));
    m_pendingBlock.getContent().headerUnsafe().setNumber(1);
    m_pendingBlock.getContent().headerUnsafe().setParentHash(currentChain().lastBlock().header().hash());
}

void ToolChainManager::mineBlocks(size_t _number)
{
    if (_number > 1)
        throw test::UpwardsException("ToolChainManager::mineBlocks number arg invalid: " + fto_string(_number));
    currentChainUnsafe().mineBlock(m_pendingBlock.getCContent());
    EthereumBlockState const& bl = currentChain().lastBlock();
    m_pendingBlock = spEthereumBlockState(new EthereumBlockState(bl.header(), bl.state(), FH32::zero()));
}

void ToolChainManager::rewindToBlock(VALUE const& _number)
{
    size_t number = (size_t)_number.asU256();
    assert(_number.asU256() >= 0 && _number < currentChainUnsafe().blocks().size());
    currentChainUnsafe().rewindToBlock(number);
    EthereumBlockState const& bl = currentChain().lastBlock();
    m_pendingBlock = spEthereumBlockState(new EthereumBlockState(bl.header(), bl.state(), bl.logHash()));
}

EthereumBlockState const& ToolChainManager::blockByNumber(VALUE const& _number) const
{
    if ((size_t)_number.asU256() >= currentChain().blocks().size())
        throw UpwardsException(string("ToolChainManager::blockByNumer block number not found: " + _number.asDecString()));
    return currentChain().blocks().at((size_t)_number.asU256());
}

EthereumBlockState const& ToolChainManager::blockByHash(FH32 const& _hash) const
{
    for (auto const& chain : m_chains)
    {
        for (auto const& block : chain.second.getCContent().blocks())
            if (block.header().hash() == _hash)
                return block;
    }
    throw UpwardsException(string("ToolChainManager::blockByHash block hash not found: " + _hash.asString()));
}

void ToolChainManager::modifyTimestamp(VALUE const& _time)
{
    m_pendingBlock.getContent().headerUnsafe().setTimestamp(_time);
}

// Import Raw Block via t8ntool
FH32 ToolChainManager::importRawBlock(BYTES const& _rlp)
{
    dev::bytes decodeRLP = sfromHex(_rlp.asString());
    dev::RLP rlp(decodeRLP, dev::RLP::VeryStrict);

    BlockHeader header(rlp[0]);
    m_pendingBlock = spEthereumBlockState(new EthereumBlockState(header, lastBlock().state(), FH32::zero()));
    for (auto const& trRLP : rlp[1].toList())
    {
        Transaction tr(trRLP);
        addPendingTransaction(tr);
    }
    for (auto const& unRLP : rlp[2].toList())
    {
        BlockHeader un(unRLP);
        m_pendingBlock.getContent().addUncle(un);
    }
    m_pendingBlock.getContent().recalculateHeaderHash();
    ETH_TEST_MESSAGE(header.asDataObject().asJson());
    mineBlocks(1);
    return lastBlock().header().hash();
}


}  // namespace toolimpl
