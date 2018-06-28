#pragma once
#include "../object.h"
#include "../stateTest/scheme_state.h"
#include "scheme_blockHeader.h"
#include <retesteth/Options.h>
#include <retesteth/TestHelper.h>

using namespace test;

namespace test
{
class scheme_blockchainTest : public object
{
public:
    scheme_blockchainTest(DataObject const& _test);
    DataObject getGenesisForRPC(std::string const& _sealEngine = "NoProof");
    std::vector<std::string> const& getBlockRlps() const { return m_blockRLPs; }
    scheme_state const& getPost() const { return m_post; }

private:
    class fieldChecker
    {
    public:
        fieldChecker(DataObject const& _test);
    };
    fieldChecker m_checker;
    scheme_state m_pre;
    scheme_state m_post;
    scheme_blockHeader m_genesisHeader;
    std::vector<std::string> m_blockRLPs;
};
}
