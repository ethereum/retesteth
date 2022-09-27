#pragma once
#include "../../configs/FORK.h"
#include "../../configs/SealEngine.h"
#include "../Ethereum/State.h"
#include "../StateTests/Filler/InfoIncomplete.h"
#include "Filler/BlockchainTestFillerBlock.h"
#include "Filler/BlockchainTestFillerEnv.h"
#include "Filler/BlockchainTestFillerExpectSection.h"

#include <libdataobj/DataObject.h>

namespace test
{
namespace teststruct
{
struct BlockchainTestInFiller : GCP_SPointerBase
{
    BlockchainTestInFiller(spDataObject&);
    std::string const& testName() const { return m_name; }
    bool hasInfo() const { return !m_info.isEmpty(); }
    InfoIncomplete const& Info() const { return m_info; }
    SealEngine sealEngine() const { return m_sealEngine; }
    BlockchainTestFillerEnv const& Env() const { return m_env; }
    State const& Pre() const { return m_pre; }
    std::vector<BlockchainTestFillerBlock> const& blocks() const { return m_blocks; }
    std::vector<BlockchainTestFillerExpectSection> const& expects() const { return m_expects; }

    bool hasExpectForNetwork(FORK const& _net) const;
    std::set<FORK> getAllForksFromExpectSections() const;
    bool hasUnclesInTest() const { return m_hasAtLeastOneUncle; }

    // UnitTest features
    std::vector<std::string> const& unitTestExceptions() const { return m_exceptions; }
    DataObject const& unitTestVerify() const { return m_verify; }


private:
    BlockchainTestInFiller() {}
    std::string m_name;
    spInfoIncomplete m_info;
    SealEngine m_sealEngine;
    spBlockchainTestFillerEnv m_env;
    spState m_pre;
    std::vector<BlockchainTestFillerBlock> m_blocks;
    std::vector<BlockchainTestFillerExpectSection> m_expects;
    std::vector<std::string> m_exceptions;
    bool m_hasAtLeastOneUncle;

    spDataObject m_verify;
};

struct BlockchainTestFiller
{
    BlockchainTestFiller(spDataObject&);
    std::vector<BlockchainTestInFiller> const& tests() const { return m_tests; }

private:
    BlockchainTestFiller() {}
    std::vector<BlockchainTestInFiller> m_tests;
};


}  // namespace teststruct
}  // namespace test
