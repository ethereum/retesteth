#pragma once
#include "../../configs/FORK.h"
#include "../../configs/SealEngine.h"
#include "../Ethereum/State.h"
#include "../StateTests/Filled/Info.h"

#include "Filled/BlockchainTestBlock.h"
#include "Filled/BlockchainTestEnv.h"
#include <libdataobj/DataObject.h>
#include <libdataobj/SPointer.h>

namespace test
{
namespace teststruct
{
struct BlockchainTestInFilled : GCP_SPointerBase
{
    BlockchainTestInFilled(spDataObject&);
    std::string const& testName() const { return m_name; }
    Info const& testInfo() const { return m_info; }
    BlockchainTestEnv const& Env() const { return m_env; }
    BYTES const& genesisRLP() const { return m_genesisRLP; }

    SealEngine sealEngine() const { return m_sealEngine; }
    FORK const& network() const { return m_fork; }
    State const& Pre() const { return m_pre; }

    bool isFullState() const { return !m_post.isEmpty(); }
    State const& Post() const { return m_post; }
    FH32 const& PostHash() const { return m_postHash; }
    FH32 const& lastBlockHash() const { return m_lastBlockHash; }

    std::vector<BlockchainTestBlock> const& blocks() const { return m_blocks; }
    std::vector<std::string> const& unitTestExceptions() const { return m_exceptions; }

private:
    BlockchainTestInFilled() {}
    std::string m_name;
    spInfo m_info;
    SealEngine m_sealEngine;
    spFORK m_fork;
    spState m_pre;
    spBlockchainTestEnv m_env;
    // Genesis block header
    spBYTES m_genesisRLP;
    std::vector<BlockchainTestBlock> m_blocks;
    spState m_post;
    spFH32 m_postHash;
    spFH32 m_lastBlockHash;

    std::vector<std::string> m_exceptions;
};

struct BlockchainTest
{
    BlockchainTest(spDataObject&);
    std::vector<BlockchainTestInFilled> const& tests() const { return m_tests; }

private:
    BlockchainTest() {}
    std::vector<BlockchainTestInFilled> m_tests;
};


}  // namespace teststruct
}  // namespace test
