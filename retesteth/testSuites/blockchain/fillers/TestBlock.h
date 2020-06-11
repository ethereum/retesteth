#pragma once
#include <testStructures/structures.h>

typedef std::vector<BlockHeader> vectorOfSchemeBlock;
class TestBlock
{
public:

    // _rlp of a block that has been imported on remote clinet
    TestBlock(BYTES const& _rlp, string const& _chainName, FORK const& _chainNet, VALUE const& _number);

    // Attach block header to the test block
    void registerTestHeader(BlockHeader const& _header) { m_block = spEthereumBlock(new EthereumBlock(_header)); }
    BlockHeader const& getTestHeader() const { return m_block.getCContent().header(); }

    // Attach Transaction header to EthereumBlock (the one described in tests)
    void registerTestTransaction(Transaction const& _tr) { m_block.getContent().addTransaction(_tr); }

    // Attach Uncle header to EthereumBlock (the one described in tests)
    void registerTestUncle(BlockHeader const& _uncle) { m_block.getContent().addUncle(_uncle); }
    std::vector<BlockHeader> const& getUncles() const { return m_block.getCContent().uncles(); }

    // Attach test exception to the test block
    void registerTestExceptios(string const& _exception) { m_expectException = _exception; }

    // Attach uncle header of potential fork to this block. If test has no uncles this will not be called
    void setNextBlockForked(BlockHeader const& _next) { m_nextBlockForked = spBlockHeader(new BlockHeader(_next)); }
    BlockHeader const& getNextBlockForked() const { return m_nextBlockForked.getCContent(); }

    // Actual RLP of a block that has been impoted on remote client
    BYTES const getRawRLP() const { return m_rawRLP.getCContent(); }

    // Block might be flagged not to be exported to the final test
    void setDoNotExport(bool _flag) { m_doNotExport = _flag; }
    bool isDoNotExport() const { return m_doNotExport; }

    DataObject asDataObject() const;

private:
    TestBlock() {}
    spEthereumBlock m_block;        // Container that stores header, transactions, uncleheaders

    string m_chainName;
    spFORK m_chainNet;
    string m_expectException;
    spVALUE m_blockNumber;
    spBYTES m_rawRLP;
    bool m_doNotExport;

    // A block mined in paralel representing an uncle (it has the same blocknumber)
    spBlockHeader m_nextBlockForked;  // Hypothetical next block for uncles
};
