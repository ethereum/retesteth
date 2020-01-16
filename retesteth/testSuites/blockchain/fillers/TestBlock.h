#pragma once
#include <ethObjects/common.h>

typedef std::vector<test::scheme_block> vectorOfSchemeBlock;
class TestBlock
{
public:
    void setUnpreparedUncle(test::scheme_block const& _uncle) { m_unpreparedUncle = _uncle; }
    DataObject& getDataForTest() { return m_dataForTest; }

private:
    // a block mined in paralel representing an uncle (it has the same blocknumber)
    test::scheme_block m_unpreparedUncle;
    DataObject m_dataForTest;
};
