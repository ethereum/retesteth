#include "TestBlockchain.h"
#include <retesteth/testSuites/Common.h>
#include <retesteth/Options.h>
#include <retesteth/testSuites/blockchain/Common.h>
#include <retesteth/TestHelper.h>
using namespace std;
using namespace test::debug;
using namespace test::session;

namespace test::blockchainfiller {

void TestBlockchain::generateBlock(
    BlockchainTestFillerBlock const& _block, vectorOfSchemeBlock const& _uncles, bool _generateUncles)
{
    if (_block.isRawRLP())
    {
        _generateBlock_RawBlock(_block);
        return;
    }

    _generateBlock_ImportTransactionsOnRemoteClient(_block, _uncles);

    // Remote client generate block with transactions
    // And if it has uncles or blockheader overwrite we perform manual overwrite and reimport block again
    // Then construct this RLP of a modifed block (with new header and uncles)
    // if block mining failed this will be the rawRLP of import
    BYTES rawRLP(DataObject("0x"));
    spEthGetBlockBy minedBlock = mineBlock(_block, _uncles, rawRLP);

    if (minedBlock.isEmpty())
        _generateBlock_RegisterInvalidBlock(_block, rawRLP);
    else
    {
        if (!Options::get().statediff.isTransSelected)
            _performStatediff((size_t)minedBlock->header()->number().asBigInt(), 0);

        // if block mining succeed. the block is valid.
        FORK const& newBlockNet = _block.hasChainNet() ? _block.chainNet() : m_network;
        TestBlock newBlock(rawRLP, _block.chainName(), newBlockNet, m_blocks.size());
        newBlock.registerTestExceptios(_block.getExpectException(m_network));
        newBlock.registerTestHeader(minedBlock->header());
        newBlock.setDoNotExport(_block.isDoNotImportOnClient());

        _generateBlock_RegisterTestTransactions(_block, newBlock, minedBlock);

        // Register all test uncles (the one described in test)
        for (auto const& uncle : _uncles)
            newBlock.registerTestUncle(uncle);

        for (auto const& with : _block.withdrawals())
            newBlock.registerTestWithdrawal(with.withdrawal());

        // Ask remote client to generate a parallel blockheader that will later be used for uncles
        if (_generateUncles)
            newBlock.setNextBlockForked(mineNextBlockAndRevert());

        m_blocks.emplace_back(newBlock);
    }
}

void TestBlockchain::_generateBlock_RawBlock(BlockchainTestFillerBlock const& _block)
{
    FH32 const blHash(m_session.test_importRawBlock(_block.rawRLP()));
    checkBlockException(m_session, _block.getExpectException(m_network));
    if (!blHash.isZero())
        ETH_ERROR_MESSAGE("rawBlock rlp appered to be valid. Unable to contruct objects from RLP!");
    else
    {
        TestBlock newBlock(_block.rawRLP(), m_chainName, m_network, m_blocks.size());
        newBlock.registerTestExceptios(_block.getExpectException(m_network));
        m_blocks.emplace_back(newBlock);
    }
}

void TestBlockchain::_generateBlock_ImportTransactionsOnRemoteClient(BlockchainTestFillerBlock const& _block,
    vectorOfSchemeBlock const& _uncles)
{
    // Import known transactions to remote client
    ETH_DC_MESSAGEC(DC::RPC, "Import transactions: " + m_sDebugString, LogColor::YELLOW);
    _tryIntermidiatePostState(_block, _uncles);
    for (auto const& tr : _block.transactions())
    {
        modifyTransactionChainIDByNetwork(tr.tr(), m_network);
        m_session.eth_sendRawTransaction(tr.tr().getRawBytes(), tr.tr().getSecret());
    }
}

void TestBlockchain::_generateBlock_RegisterInvalidBlock(BlockchainTestFillerBlock const& _block, BYTES const& _rawRLP)
{
    // if block mining failed, the block is invalid
    FORK const& newBlockNet = _block.hasChainNet() ? _block.chainNet() : m_network;
    TestBlock newBlock(_rawRLP, _block.chainName(), newBlockNet, m_blocks.size());
    newBlock.registerTestExceptios(_block.getExpectException(m_network));
    newBlock.setDoNotExport(_block.isDoNotImportOnClient());
    m_blocks.emplace_back(newBlock);
}

void TestBlockchain::_generateBlock_RegisterTestTransactions(BlockchainTestFillerBlock const& _block,
    TestBlock& _newBlock, spEthGetBlockBy _minedBlock)
{
    // Register all test transactions (the one described in test)
    // In the same order as on remote block has returned after mining
    typedef std::tuple<spTransaction, bool> testTrInfo;
    std::map<FH32, testTrInfo> testTransactionMap;
    bool hasAtLeastOneInvalid = false;
    for (auto const& tr : _block.transactions())
    {
        string const& exception = tr.getExpectException(m_network);
        bool hasException = !exception.empty();
        testTransactionMap[tr.tr().hash()] = {tr.trPointer(), hasException};
        hasAtLeastOneInvalid = hasAtLeastOneInvalid || hasException;
        _newBlock.registerTransactionSequence({tr.tr().getRawBytes(), exception});
    }

    // Only export the order if we have rejected transactions. to save space in tests
    if (!hasAtLeastOneInvalid)
        _newBlock.nullTransactionSequence();

    size_t txIndex = 0;
    for (auto const& remoteTr : _minedBlock->transactions())
    {
        TxContext const ctx(m_session, TestOutputHelper::get().testName(), remoteTr->transaction(), _minedBlock->header(),
            m_network, (size_t)_minedBlock->header()->number().asBigInt(), txIndex);
        performVMTrace(ctx);
        txIndex++;

        if (testTransactionMap.count(remoteTr->hash()))
        {
            bool isMarkedInvalid = std::get<1>(testTransactionMap.at(remoteTr->hash()));
            spTransaction const& spTr = std::get<0>(testTransactionMap.at(remoteTr->hash()));

            if (!isMarkedInvalid)
                _newBlock.registerTestTransaction(spTr);

            testTransactionMap.erase(remoteTr->hash());
        }
        else
            ETH_ERROR_MESSAGE("Remote client returned block with transaction that is not registered in test!");
    }

    // Register all the transactions that have not been repoted by remote client
    // Perhaps it dropped some of the valid transactions..
    for (auto const& leftoverTr : testTransactionMap)
    {
        bool isMarkedInvalid = std::get<1>(leftoverTr.second);
        spTransaction const& spTr = std::get<0>(leftoverTr.second);
        if (!isMarkedInvalid)
            _newBlock.registerTestTransaction(spTr);
    }
}


void TestBlockchain::_tryIntermidiatePostState(BlockchainTestFillerBlock const& _block, vectorOfSchemeBlock const& _uncles)
{
    auto runForOption = [this, &_block, &_uncles]
        (auto const& _option, size_t _blockNumber, size_t _transNumber)
    {
        bool const optionInitialized = _option.initialized() && _option.isBlockSelected;
        auto const optionBlockNumber = _blockNumber;
        auto const optionTransNumber = _transNumber;

        if (optionInitialized)
        {
            auto const currentBlockNumber = m_session.eth_blockNumber();
            if (optionBlockNumber != (size_t)currentBlockNumber.asBigInt() + 1)
                return;

            bool thereisTransactionIndex = optionTransNumber < _block.transactions().size();
            if (!thereisTransactionIndex)
                return;

            size_t txIndex = 0;
            for (auto const& tr : _block.transactions())
            {
                modifyTransactionChainIDByNetwork(tr.tr(), m_network);
                m_session.eth_sendRawTransaction(tr.tr().getRawBytes(), tr.tr().getSecret());

                if (txIndex == optionTransNumber)
                {
                    BYTES rawRLP(DataObject("0x"));
                    GCP_SPointer<EthGetBlockBy> minedBlock = mineBlock(_block, _uncles, rawRLP, true);
                    if (!minedBlock.isEmpty())
                    {
                        TxContext const ctx(m_session, TestOutputHelper::get().testName(), tr.trPointer(), minedBlock->header(),
                            m_network, (size_t)minedBlock->header()->number().asBigInt(), txIndex);
                        performPostState(ctx);
                        _performStatediff((size_t)(currentBlockNumber.asBigInt()+1), txIndex);
                    }
                    else
                        ETH_WARNING("Tying to get intermidiate tx state for block#" + test::fto_string(optionBlockNumber) + ", but block mining failed!");

                    m_session.test_rewindToBlock(currentBlockNumber);
                    {
                        VALUE latestBlockNumber(m_session.eth_blockNumber());
                        EthGetBlockBy const latestBlock(m_session.eth_getBlockByNumber(latestBlockNumber, Request::LESSOBJECTS));
                        m_session.test_modifyTimestamp(latestBlock.header()->timestamp() + 1000);
                    }
                    return;
                }
                txIndex++;
            }
        }
    };

    auto const& opt = Options::get();
    runForOption(opt.poststate, opt.poststate.blockNumber, opt.poststate.transactionNumber);

    auto runForStatediff = [this, &opt, &runForOption](){
        if (m_stateDiffStateA.isEmpty())
            runForOption(opt.statediff, opt.statediff.firstBlock, opt.statediff.firstTrnsx);
        else if (m_stateDiffStateB.isEmpty())
            runForOption(opt.statediff, opt.statediff.seconBlock, opt.statediff.seconTrnsx);
    };

    if (opt.statediff.firstBlock == opt.statediff.seconBlock)
    {
        // attempt to capture the intermidiate state from different txs
        runForStatediff();
        runForStatediff();
    }
    else
        runForStatediff();

}

void TestBlockchain::_performStatediff(size_t _blockNumber, size_t _txNumber)
{
    m_triedStateDiff = true;
    auto const& statediff = Options::get().statediff;
    if (statediff.initialized() && statediff.isBlockSelected && m_stateDiffStateB.isEmpty())
    {
        bool selector = statediff.firstBlock == _blockNumber;
        if (statediff.isTransSelected)
            selector = selector && statediff.firstTrnsx == _txNumber;

        if (selector && m_stateDiffStateA.isEmpty())
        {
            m_stateDiffStateA = getRemoteState(m_session);
        }
        else
        {
            bool selector = statediff.seconBlock == _blockNumber;
            if (statediff.isTransSelected)
                selector = selector && statediff.seconTrnsx == _txNumber;

            if (selector && m_stateDiffStateB.isEmpty() && !m_stateDiffStateA.isEmpty())
            {
                m_stateDiffStateB = getRemoteState(m_session);
                auto const diff = test::stateDiff(m_stateDiffStateA, m_stateDiffStateB)->asJson();
                ETH_DC_MESSAGE(DC::STATE,
                    "\nFilling BC test State Diff:" + TestOutputHelper::get().testInfo().errorDebug() + cDefault + " \n" + diff);
            }
        }
    }
}

}
