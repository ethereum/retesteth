#pragma once
#include "../object.h"
#include "../stateTest/scheme_state.h"
#include "../stateTest/scheme_transaction.h"
#include "scheme_blockHeader.h"
#include <retesteth/Options.h>
#include <retesteth/TestHelper.h>
#include <vector>

using namespace test;

namespace test
{
class scheme_blockchainTestBase : public object
{
public:
    scheme_blockchainTestBase(DataObject const& _test);
    DataObject getGenesisForRPC(
        string const& _network = string(), string const& _sealEngine = "NoProof") const;
    scheme_state const& getPre() const { return m_pre; };

private:
    class fieldChecker
    {
    public:
        fieldChecker(DataObject const& _test);
    };
    fieldChecker m_checker;
    scheme_state m_pre;
    scheme_blockHeader m_genesisHeader;
    std::string m_sealEngine;
};

class scheme_blockchainTest : public scheme_blockchainTestBase
{
public:
    scheme_blockchainTest(DataObject const& _test);
    std::vector<std::string> const& getBlockRlps() const { return m_blockRLPs; }
    scheme_state const& getPost() const { return m_post; }
    string const& getNetwork() const { return m_data.atKey("network").asString(); }
    string const& getEngine() const
    {
        if (m_data.count("sealEngine"))
            return m_data.atKey("sealEngine").asString();
        return m_sNoProof;
    }
    string const& getLastBlockHash() const { return m_data.atKey("lastblockhash").asString(); }

private:
    class fieldChecker
    {
    public:
        fieldChecker(DataObject const& _test);
    };
    fieldChecker m_checker;
    scheme_state m_post;
    std::vector<std::string> m_blockRLPs;
    std::vector<scheme_transaction> m_transactions;
    static const string m_sNoProof;
};
}
