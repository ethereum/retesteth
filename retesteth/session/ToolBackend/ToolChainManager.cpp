#include "ToolChainManager.h"
#include "ToolImplHelper.h"
#include <retesteth/EthChecks.h>
#include <retesteth/TestHelper.h>
#include <retesteth/testStructures/Common.h>
using namespace test;

namespace toolimpl
{
ToolChainManager::ToolChainManager(SetChainParamsArgs const& _config, fs::path const& _toolPath)
{
    m_currentChain = 0;
    m_maxChains = 0;
    EthereumBlockState genesis(_config.genesis(), _config.state(), FH32::zero());
    m_chains[m_currentChain] = spToolChain(new ToolChain(genesis, _config.sealEngine(), _config.fork(), _toolPath));
    m_pendingBlock =
        spEthereumBlockState(new EthereumBlockState(currentChain().lastBlock().header(), _config.state(), FH32::zero()));
    reorganizePendingBlock();
}

void ToolChainManager::mineBlocks(size_t _number, ToolChain::Mining _req)
{
    if (_number > 1)
        throw test::UpwardsException("ToolChainManager::mineBlocks number arg invalid: " + fto_string(_number));
    currentChainUnsafe().mineBlock(m_pendingBlock.getCContent(), _req);
    EthereumBlockState const& bl = currentChain().lastBlock();
    m_pendingBlock = spEthereumBlockState(new EthereumBlockState(bl.header(), bl.state(), FH32::zero()));
}

void ToolChainManager::rewindToBlock(VALUE const& _number)
{
    size_t number = (size_t)_number.asU256();
    assert(_number.asU256() >= 0 && _number < currentChainUnsafe().blocks().size());
    currentChainUnsafe().rewindToBlock(number);
    reorganizePendingBlock();
}

void ToolChainManager::reorganizePendingBlock()
{
    EthereumBlockState const& bl = currentChain().lastBlock();
    m_pendingBlock = spEthereumBlockState(new EthereumBlockState(bl.header(), bl.state(), bl.logHash()));
    m_pendingBlock.getContent().headerUnsafe().setNumber(1);
    m_pendingBlock.getContent().headerUnsafe().setExtraData(DataObject("0x"));
    m_pendingBlock.getContent().headerUnsafe().setParentHash(currentChain().lastBlock().header().hash());
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
    dev::u256 prevTime = lastBlock().header().timestamp().asU256();
    m_pendingBlock.getContent().headerUnsafe().setTimestamp(prevTime + _time.asU256());
}

// Import Raw Block via t8ntool
FH32 ToolChainManager::importRawBlock(BYTES const& _rlp)
{
    try
    {
        dev::bytes decodeRLP = sfromHex(_rlp.asString());
        dev::RLP rlp(decodeRLP, dev::RLP::VeryStrict);
        toolimpl::verifyBlockRLP(rlp);

        BlockHeader header(rlp[0]);
        toolimpl::verifyEthereumBlockHeader(header);

        // Check that we know the parent and prepare head to be the parentHeader of _rlp block
        reorganizeChainForParent(header.parentHash());

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

        ETH_TEST_MESSAGE(header.asDataObject().asJson());
        mineBlocks(1, ToolChain::Mining::RequireValid);
        FH32 const& importedHash = lastBlock().header().hash();
        if (importedHash != header.hash())
        {
            string message = "t8ntool constructed HEADER vs rawRLP HEADER: \n";
            message += compareBlockHeaders(lastBlock().header().asDataObject(), header.asDataObject());
            ETH_ERROR_MESSAGE(string("Imported block hash != rawRLP hash ") + "(" + importedHash.asString() +
                              " != " + header.hash().asString() + ")" + "\n " + message);
        }

        reorganizeChainForTotalDifficulty();
        return importedHash;
    }
    catch (std::exception const& _ex)
    {
        reorganizeChainForTotalDifficulty();
        throw test::UpwardsException(string("Error importing raw rlp block: ") + _ex.what());
    }
}

void ToolChainManager::reorganizeChainForParent(FH32 const& _parentHash)
{
    for (auto const& chain : m_chains)
    {
        auto const& rchain = chain.second.getCContent();
        auto const& blocks = chain.second.getCContent().blocks();
        for (size_t i = 0; i < blocks.size(); i++)
        {
            if (blocks.at(i).header().hash() == _parentHash)
            {
                if (i + 1 == blocks.size())  // last known block
                {                            // stay on this chain
                    m_currentChain = chain.first;
                    return;
                }
                else
                {
                    // clone existing chain up to this block
                    m_chains[++m_maxChains] =
                        spToolChain(new ToolChain(blocks.at(0), rchain.engine(), rchain.fork(), rchain.toolPath()));
                    m_currentChain = m_maxChains;
                    for (size_t j = 1; j <= i; j++)
                        m_chains[m_currentChain].getContent().insertBlock(blocks.at(j));
                    return;
                }
            }
        }
    }
    throw test::UpwardsException(string("ToolChainManager:: unknown parent hash ") + _parentHash.asString());
}

void ToolChainManager::reorganizeChainForTotalDifficulty()
{
    VALUE maxTotalDifficulty(0);
    for (auto const& chain : m_chains)
    {
        auto const& blocks = chain.second.getCContent().blocks();
        auto const& lastBlock = blocks.at(blocks.size() - 1);

        if (lastBlock.totalDifficulty() > maxTotalDifficulty)
        {
            maxTotalDifficulty = lastBlock.totalDifficulty();
            m_currentChain = chain.first;
        }
    }
}


}  // namespace toolimpl
