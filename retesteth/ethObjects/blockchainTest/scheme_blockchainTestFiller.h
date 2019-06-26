#pragma once
#include "../expectSection/scheme_expectSection.h"
#include "../object.h"
#include "../stateTest/scheme_state.h"
#include "../stateTest/scheme_transaction.h"
#include "scheme_blockHeader.h"
#include "scheme_blockchainTest.h"
#include <retesteth/Options.h>
#include <retesteth/TestHelper.h>

using namespace test;

namespace test
{
class scheme_blockchainTestFiller : public scheme_blockchainTestBase
{
public:
    class blockSection
    {
    public:
        blockSection(DataObject const& _data);
        std::vector<scheme_transaction> const& getTransactions() const { return m_transactons; }

    private:
        std::vector<scheme_transaction> m_transactons;
    };

public:
    scheme_blockchainTestFiller(DataObject const& _test);
    scheme_expectSection const& getExpectSection() const { return m_expectSection; }
    std::vector<blockSection> const& getBlocks() const { return m_blocks; }
    string getSealEngine() const;

private:
    class fieldChecker
    {
    public:
        fieldChecker(DataObject const& _test);
    };
    fieldChecker m_checker;
    scheme_expectSection m_expectSection;
    std::vector<blockSection> m_blocks;
};
}
