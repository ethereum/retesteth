#pragma once
#include "../expectSection/scheme_expectSection.h"
#include "../object.h"
#include "../stateTest/scheme_state.h"
#include "../stateTest/scheme_transaction.h"
#include "scheme_blockHeader.h"
#include "scheme_blockchainTest.h"
#include "scheme_uncleHeader.h"
#include <retesteth/Options.h>
#include <retesteth/TestHelper.h>

using namespace test;

namespace test
{
class scheme_blockchainTestFiller : public scheme_blockchainTestBase
{
public:
    class blockSection : public object
    {
    public:
        blockSection(DataObject const& _data);
        std::vector<scheme_transaction> const& getTransactions() const { return m_transactons; }
        std::vector<scheme_uncleHeader> const& getUncles() const { return m_uncles; }
        std::string const& getException(string const& _network) const
        {
            if (m_expectException.count(_network))
                return m_expectException.at(_network);
            return m_noExceptionString;
        }
        std::string const& getChainName() const
        {
            if (m_data.count("chainname"))
                return m_data.atKey("chainname").asString();
            return object::emptyString;
        }
        size_t getNumber() const
        {
            if (m_data.count("blocknumber"))
                return (size_t)atoi(m_data.atKey("blocknumber").asString().c_str());
            return 0;
        }

    private:
        std::vector<scheme_transaction> m_transactons;
        std::vector<scheme_uncleHeader> m_uncles;
        std::map<string, string> m_expectException;
        std::string m_noExceptionString;
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
