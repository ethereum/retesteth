#pragma once
#include <testStructures/structures.h>

typedef std::vector<spBlockHeader> vectorOfSchemeBlock;
typedef std::tuple<BYTES, string> TransactBytesException;
class TestBlock
{
public:

    // _rlp of a block that has been imported on remote clinet
    TestBlock(BYTES const& _rlp, string const& _chainName, FORK const& _chainNet, VALUE const& _number);

    // Attach block header to the test block
    void registerTestHeader(spBlockHeader const& _header) { m_block = spEthereumBlock(new EthereumBlock(_header)); }
    spBlockHeader const& getTestHeader() const { return m_block.getCContent().header(); }

    // Attach Transaction header to EthereumBlock (the one described in tests)
    void registerTestTransaction(spTransaction const& _tr) { m_block.getContent().addTransaction(_tr); }

    // Attach Transaction to transaction Execution Order section
    void registerTransactionSequence(TransactBytesException const& _trTuple) { m_transactionExecOrder.push_back(_trTuple); }
    void nullTransactionSequence() { m_transactionExecOrder.clear(); }

    // Attach Uncle header to EthereumBlock (the one described in tests)
    void registerTestUncle(spBlockHeader const& _uncle) { m_block.getContent().addUncle(_uncle); }
    std::vector<spBlockHeader> const& getUncles() const { return m_block.getCContent().uncles(); }

    // Attach test exception to the test block
    void registerTestExceptios(string const& _exception) { m_expectException = _exception; }

    // Attach uncle header of potential fork to this block. If test has no uncles this will not be called
    void setNextBlockForked(spBlockHeader const& _next)
    {
        m_nextBlockForked = readBlockHeader(_next.getCContent().asDataObject());
    }
    spBlockHeader const& getNextBlockForked() const { return m_nextBlockForked; }

    // Actual RLP of a block that has been impoted on remote client
    BYTES const getRawRLP() const { return m_rawRLP.getCContent(); }

    // Block might be flagged not to be exported to the final test
    void setDoNotExport(bool _flag) { m_doNotExport = _flag; }
    bool isDoNotExport() const { return m_doNotExport; }

    DataObject asDataObject() const;

private:
    TestBlock() {}
    spEthereumBlock m_block;        // Container that stores header, transactions, uncleheaders

    // Transaction Execution order for invalid transaction check
    std::vector<TransactBytesException> m_transactionExecOrder;

    string m_chainName;
    spFORK m_chainNet;
    string m_expectException;
    spVALUE m_blockNumber;
    spBYTES m_rawRLP;
    bool m_doNotExport;

    // A block mined in paralel representing an uncle (it has the same blocknumber)
    spBlockHeader m_nextBlockForked;  // Hypothetical next block for uncles
};
