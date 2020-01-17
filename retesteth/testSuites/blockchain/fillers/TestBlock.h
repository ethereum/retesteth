#pragma once
#include <ethObjects/common.h>

typedef std::vector<test::scheme_block> vectorOfSchemeBlock;
class TestBlock
{
public:
    void setNextBlockForked(test::scheme_block const& _next) { m_nextBlockForked.push_back(_next); }
    test::scheme_block const& getNextBlockForked() const
    {
        assert(m_nextBlockForked.size() > 0);
        return m_nextBlockForked.at(0);
    }
    DataObject& getDataForTestUnsafe() { return m_dataForTest; }
    DataObject const& getDataForTest() const { return m_dataForTest; }
    string const& getRLP() const { return m_dataForTest.atKey("rlp").asString(); }
    void addUncle(test::scheme_block const& _uncle) { m_uncles.push_back(_uncle); }
    std::vector<test::scheme_block> const& getUncles() const { return m_uncles; }

private:
    // a block mined in paralel representing an uncle (it has the same blocknumber)
    DataObject m_dataForTest;
    std::vector<test::scheme_block> m_nextBlockForked;  // Hypothetical next block for uncles
    std::vector<test::scheme_block> m_uncles;
};
