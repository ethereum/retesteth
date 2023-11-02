#include "../ToolChainManager.h"
#include "../ToolImplHelper.h"
#include "../Verification.h"
#include <retesteth/helpers/TestHelper.h>
#include <retesteth/testStructures/types/Ethereum/Transactions/TransactionBlob.h>
using namespace std;
using namespace test;
using namespace test::debug;
using namespace toolimpl;
using namespace test::teststruct;

spBlockHeader ToolChainManager::_irb_verifyAndSetHeader(dev::RLP const& _headerRLP)
{
    spBlockHeader header = readBlockHeader(_headerRLP);
    ETH_DC_MESSAGE(DC::RPC, header->asDataObject()->asJson());
    for (auto const& chain : m_chains)
        for (auto const& bl : chain.second->blocks())
            if (bl.header()->hash() == header->hash())
                ETH_WARNING("Block with hash: `" + header->hash().asString() + "` already in chain!");

    // Check that we know the parent and prepare head to be the parentHeader of _rlp block
    reorganizeChainForParent(header->parentHash());
    verifyEthereumBlockHeader(header, currentChain());
    m_pendingBlock = spEthereumBlockState(new EthereumBlockState(header, lastBlock().state(), FH32::zero()));
    m_pendingBlock.getContent().setTotalDifficulty(lastBlock().totalDifficulty());
    return header;
}

void ToolChainManager::_irb_verifyAndSetTransactions(dev::RLP const& _trsRLP)
{
    ETH_DC_MESSAGE(DC::RPC, "RLP transaction number: " + test::fto_string(_trsRLP.toList().size()));
    size_t blobCount = 0;
    for (auto const& trRLP : _trsRLP.toList())
    {
        spTransaction spTr = readTransaction(trRLP);
        if (spTr->type() == TransactionType::BLOB)
        {
            TransactionBlob const& blobtx = dynamic_cast<TransactionBlob const&>(spTr.getContent());
            blobCount += blobtx.blobs().size();
            if (blobCount >= 7)
                throw test::UpwardsException("versioned hashes len exceeds, Block has invalid number of blobs in txs >=7! would exceed maximum");
        }
        ETH_DC_MESSAGE(DC::RPC, spTr->asDataObject()->asJson());
        addPendingTransaction(spTr);
    }
}

void ToolChainManager::_irb_verifyAndSetUncles(dev::RLP const& _unclsRLP, spBlockHeader const& _header)
{
    if (_unclsRLP.toList().size() > 2)
        throw test::UpwardsException("Too many uncles!");

    for (auto const& unRLP : _unclsRLP.toList())
    {
        spBlockHeader un = readBlockHeader(unRLP);
        verifyEthereumBlockHeader(un, currentChain());
        if (un->number() >= _header->number() ||
            un->number() + 7 <= _header->number())
            throw test::UpwardsException("Uncle number is wrong!");
        for (auto const& pun : m_pendingBlock->uncles())
            if (pun->hash() == un->hash())
                throw test::UpwardsException("Uncle is brother!");
        m_pendingBlock.getContent().addUncle(un);
    }
}


void ToolChainManager::_irb_verifyAndSetWithdrawals(dev::RLP const& _blockRLP, spBlockHeader const& _header)
{
    if (_blockRLP.itemCount() > 3 || isBlockExportWithdrawals(_header))
    {
        verifyWithdrawalsRLP(_blockRLP[3]);
        for (auto const& wtRLP : _blockRLP[3].toList())
        {
            if (wtRLP.itemCount() != 4)
                throw dev::RLPException("Rlp structure is wrong: Withdrawals RLP does not have 4 elements!");
            spWithdrawal wt(new Withdrawal(wtRLP));
            verifyWithdrawalRecord(wt);
            m_pendingBlock.getContent().addWithdrawal(wt);
        }
    }
}

FH32 ToolChainManager::_irb_compareT8NBlockToRawRLP(spBlockHeader const& _header)
{
    FH32 const importedHash = lastBlock().header()->hash();
    if (importedHash != _header->hash())
    {
        string errField;
        string message = "t8ntool constructed HEADER vs rawRLP HEADER: \n";
        message += compareBlockHeaders(
            lastBlock().header()->asDataObject(), _header->asDataObject(), errField);
        ETH_ERROR_MESSAGE(string("Imported block hash != rawRLP hash ") + "(" + importedHash.asString() +
                          " != " + _header->hash().asString() + ")" + "\n " + message);
    }
    return importedHash;
}


// Import Raw Block via t8ntool
FH32 ToolChainManager::importRawBlock(BYTES const& _rlp)
{
    size_t const currentChainINDX = m_currentChain;
    try
    {
        dev::bytes const decodeRLP = sfromHex(_rlp.asString());
        dev::RLP const rlp(decodeRLP, dev::RLP::VeryStrict);
        toolimpl::verifyBlockRLP(rlp);

        auto const header = _irb_verifyAndSetHeader(rlp[0]);
        _irb_verifyAndSetTransactions(rlp[1]);
        _irb_verifyAndSetUncles(rlp[2], header);
        _irb_verifyAndSetWithdrawals(rlp, header);

        mineBlocks(1, ToolChain::Mining::RequireValid);
        auto const importedHash = _irb_compareT8NBlockToRawRLP(header);

        if (!isMergeChain())
            reorganizeChainForTotalDifficulty();
        reorganizePendingBlock();
        return importedHash;
    }
    catch (std::exception const& _ex)
    {
        if (!isMergeChain())
            reorganizeChainForTotalDifficulty();
        m_currentChain = currentChainINDX;
        m_pendingBlock.getContent().clear();
        static const string exception = "Error importing raw rlp block: ";
        throw test::UpwardsException(exception + _ex.what());
    }
}
