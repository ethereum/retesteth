#pragma once
#include <testStructures/structures.h>

namespace test::blockchainfiller
{
typedef std::vector<spBlockHeader> vectorOfSchemeBlock;
typedef std::tuple<BYTES, std::string> TransactBytesException;
class TestBlock
{
public:

    // _rlp of a block that has been imported on remote clinet
    TestBlock(BYTES const& _rlp, std::string const& _chainName, FORK const& _chainNet, VALUE const& _number);

    // Attach block header to the test block
    void registerTestHeader(spBlockHeader const& _header) { m_block = spEthereumBlock(new EthereumBlock(_header)); }
    spBlockHeader const& getTestHeader() const { return m_block->header(); }

    // Attach Transaction header to EthereumBlock (the one described in tests)
    void registerTestTransaction(spTransaction const& _tr) { m_block.getContent().addTransaction(_tr); }

    // Attach Transaction to transaction Execution Order section
    void registerTransactionSequence(TransactBytesException const& _trTuple) { m_transactionExecOrder.emplace_back(_trTuple); }
    void nullTransactionSequence() { m_transactionExecOrder.clear(); }

    // Attach Uncle header to EthereumBlock (the one described in tests)
    void registerTestUncle(spBlockHeader const& _uncle) { m_block.getContent().addUncle(_uncle); }

    void registerTestWithdrawal(spWithdrawal const& _w) { m_block.getContent().addWithdrawal(_w); }

    bool isThereTestHeader() const { return !m_block.isEmpty(); }
    std::vector<spBlockHeader> const& getUncles() const { return m_block->uncles(); }

    // Attach test exception to the test block
    void registerTestExceptios(std::string const& _exception) { m_expectException = _exception; }

    // Attach uncle header of potential fork to this block. If test has no uncles this will not be called
    void setNextBlockForked(spBlockHeader const& _next)
    {
        m_nextBlockForked = readBlockHeader(_next->asDataObject());
    }
    spBlockHeader const& getNextBlockForked() const { return m_nextBlockForked; }

    // Actual RLP of a block that has been impoted on remote client
    BYTES const getRawRLP() const { return m_rawRLP; }

    // Block might be flagged not to be exported to the final test
    void setDoNotExport(bool _flag) { m_doNotExport = _flag; }
    void setHasBigInt(bool _flag) { m_hasBigInt = _flag; }
    bool isDoNotExport() const { return m_doNotExport; }
    bool hasBigInt() const { return m_hasBigInt; }

    spDataObject asDataObject() const;
    static void exportRLPDecodedToData(BYTES const& _rawRLP, DataObject& _res);

private:
    TestBlock() {}
    spEthereumBlock m_block;        // Container that stores header, transactions, uncleheaders

    // Transaction Execution order for invalid transaction check
    std::vector<TransactBytesException> m_transactionExecOrder;

    std::string m_chainName;
    spFORK m_chainNet;
    std::string m_expectException;
    spVALUE m_blockNumber;
    spBYTES m_rawRLP;
    bool m_doNotExport;
    bool m_hasBigInt = false;

    // A block mined in paralel representing an uncle (it has the same blocknumber)
    spBlockHeader m_nextBlockForked;  // Hypothetical next block for uncles
};
}  // namespace test::blockchainfiller
