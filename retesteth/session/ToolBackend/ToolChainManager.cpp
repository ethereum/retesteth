#include "ToolChainManager.h"
#include "ToolChainHelper.h"
#include "ToolImplHelper.h"
#include <retesteth/EthChecks.h>
#include <retesteth/TestHelper.h>
#include <retesteth/testStructures/Common.h>
using namespace test;

namespace toolimpl
{
ToolChainManager::ToolChainManager(SetChainParamsArgs const& _config, fs::path const& _toolPath, fs::path const& _tmpDir)
{
    m_currentChain = 0;
    m_maxChains = 0;
    EthereumBlockState genesis(_config.genesis(), _config.state(), FH32::zero());
    m_chains[m_currentChain] = spToolChain(new ToolChain(genesis, _config, _toolPath, _tmpDir));
    m_pendingBlock =
        spEthereumBlockState(new EthereumBlockState(currentChain().lastBlock().header(), _config.state(), FH32::zero()));
    reorganizePendingBlock();
}

void ToolChainManager::mineBlocks(size_t _number, ToolChain::Mining _req)
{
    if (_number > 1)
        throw test::UpwardsException("ToolChainManager::mineBlocks number arg invalid: " + fto_string(_number));
    currentChainUnsafe().mineBlock(m_pendingBlock.getCContent(), _req);
    reorganizePendingBlock();
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
    m_pendingBlock.getContent().headerUnsafe().getContent().setNumber(bl.header().getCContent().number() + 1);

    // Because aleth and geth+retesteth does this, but better be empty extraData
    m_pendingBlock.getContent().headerUnsafe().getContent().setExtraData(bl.header().getCContent().extraData());
    if (currentChain().fork() == "HomesteadToDaoAt5" && m_pendingBlock.getContent().header().getCContent().number() == 5)
        m_pendingBlock.getContent().headerUnsafe().getContent().setExtraData(BYTES(DataObject("0x64616f2d686172642d666f726b")));
    m_pendingBlock.getContent().headerUnsafe().getContent().setParentHash(
        currentChain().lastBlock().header().getCContent().hash());
}

EthereumBlockState const& ToolChainManager::blockByNumber(VALUE const& _number) const
{
    size_t blockN = (size_t)_number.asU256();
    if (blockN >= currentChain().blocks().size())
        throw UpwardsException(string("ToolChainManager::blockByNumer block number not found: " + _number.asDecString()));
    return currentChain().blocks().at(blockN);
}

EthereumBlockState const& ToolChainManager::blockByHash(FH32 const& _hash) const
{
    for (auto const& chain : m_chains)
    {
        for (auto const& block : chain.second.getCContent().blocks())
            if (block.header().getCContent().hash() == _hash)
                return block;
    }
    throw UpwardsException(string("ToolChainManager::blockByHash block hash not found: " + _hash.asString()));
}

void ToolChainManager::modifyTimestamp(VALUE const& _time)
{
    dev::u256 prevTime = lastBlock().header().getCContent().timestamp().asU256();
    m_pendingBlock.getContent().headerUnsafe().getContent().setTimestamp(prevTime + _time.asU256());
}

// Import Raw Block via t8ntool
FH32 ToolChainManager::importRawBlock(BYTES const& _rlp)
{
    try
    {
        dev::bytes decodeRLP = sfromHex(_rlp.asString());
        dev::RLP rlp(decodeRLP, dev::RLP::VeryStrict);
        toolimpl::verifyBlockRLP(rlp);

        spBlockHeader header = readBlockHeader(rlp[0]);
        ETH_TEST_MESSAGE(header.getCContent().asDataObject().asJson());
        for (auto const& chain : m_chains)
            for (auto const& bl : chain.second.getCContent().blocks())
                if (bl.header().getCContent().hash() == header.getCContent().hash())
                    ETH_WARNING("Block with hash: `" + header.getCContent().hash().asString() + "` already in chain!");

        // Check that we know the parent and prepare head to be the parentHeader of _rlp block
        reorganizeChainForParent(header.getCContent().parentHash());
        verifyEthereumBlockHeader(header, currentChain());

        m_pendingBlock = spEthereumBlockState(new EthereumBlockState(header, lastBlock().state(), FH32::zero()));
        for (auto const& trRLP : rlp[1].toList())
        {
            spTransaction spTr = readTransaction(trRLP);
            ETH_TEST_MESSAGE(spTr.getCContent().asDataObject().asJson());
            addPendingTransaction(spTr);
        }

        if (rlp[2].toList().size() > 2)
            throw test::UpwardsException("Too many uncles!");

        for (auto const& unRLP : rlp[2].toList())
        {
            spBlockHeader un = readBlockHeader(unRLP);
            verifyEthereumBlockHeader(un, currentChain());
            if (un.getCContent().number() >= header.getCContent().number() ||
                un.getCContent().number() + 7 <= header.getCContent().number())
                throw test::UpwardsException("Uncle number is wrong!");
            for (auto const& pun : m_pendingBlock.getCContent().uncles())
                if (pun.getCContent().hash() == un.getCContent().hash())
                    throw test::UpwardsException("Uncle is brother!");
            m_pendingBlock.getContent().addUncle(un);
        }

        mineBlocks(1, ToolChain::Mining::RequireValid);
        FH32 const& importedHash = lastBlock().header().getCContent().hash();
        if (importedHash != header.getCContent().hash())
        {
            string errField;
            string message = "t8ntool constructed HEADER vs rawRLP HEADER: \n";
            message += compareBlockHeaders(
                lastBlock().header().getCContent().asDataObject(), header.getCContent().asDataObject(), errField);
            ETH_ERROR_MESSAGE(string("Imported block hash != rawRLP hash ") + "(" + importedHash.asString() +
                              " != " + header.getCContent().hash().asString() + ")" + "\n " + message);
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
            if (blocks.at(i).header().getCContent().hash() == _parentHash)
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
                        spToolChain(new ToolChain(blocks.at(0), rchain.params(), rchain.toolPath(), rchain.tmpDir()));
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
