#include "ToolChainManager.h"
#include <libdevcore/CommonIO.h>
#include <retesteth/Options.h>
#include <retesteth/TestHelper.h>
#include <retesteth/dataObject/ConvertFile.h>
#include <retesteth/dataObject/DataObject.h>
#include <retesteth/testStructures/types/BlockchainTests/Filler/BlockchainTestFillerEnv.h>
#include <retesteth/testStructures/types/RPC/ToolResponse.h>
using namespace dev;
using namespace test;
using namespace dataobject;

namespace
{
// We simulate the client backend side here, so thats why number5 is hardcoded
// Map rewards from non standard forks into standard
static std::map<FORK, FORK> RewardMapForToolBefore5 = {{"FrontierToHomesteadAt5", "Frontier"},
    {"HomesteadToEIP150At5", "Homestead"}, {"EIP158ToByzantiumAt5", "EIP158"}, {"HomesteadToDaoAt5", "Homestead"},
    {"ByzantiumToConstantinopleFixAt5", "Byzantium"}};
static std::map<FORK, FORK> RewardMapForToolAfter5 = {{"FrontierToHomesteadAt5", "Homestead"},
    {"HomesteadToEIP150At5", "EIP150"}, {"EIP158ToByzantiumAt5", "Byzantium"}, {"HomesteadToDaoAt5", "Homestead"},
    {"ByzantiumToConstantinopleFixAt5", "ConstantinopleFix"}};

VALUE prepareReward(SealEngine _engine, FORK const& _fork, VALUE const& _blockNumber = VALUE(0))
{
    if (_engine == SealEngine::Ethash)
        ETH_WARNING_TEST("t8ntool backend treat Ethash as NoProof!", 6);

    // Setup mining rewards
    std::map<FORK, spVALUE> const& rewards = Options::get().getDynamicOptions().getCurrentConfig().getRewardMap();
    if (rewards.count(_fork))
        return rewards.at(_fork).getCContent();
    else
    {
        if (_blockNumber < 5)
            return rewards.at(RewardMapForToolBefore5.at(_fork)).getCContent();
        else
            return rewards.at(RewardMapForToolAfter5.at(_fork)).getCContent();
    }
}
}  // namespace


namespace toolimpl
{
ToolChain::ToolChain(EthereumBlock const& _genesis, State const& _genesisState, SealEngine _sealEngine, FORK const& _fork,
    fs::path const& _toolPath)
  : m_genesisState(new State(_genesisState.asDataObject())),
    m_engine(_sealEngine),
    m_fork(new FORK(_fork)),
    m_toolPath(_toolPath)
{
    // We yet don't know the state root. ask the tool to calculate it
    ToolResponse res = mineBlock(_genesis, _genesisState);

    EthereumBlock genesisFixed(_genesis.header().asDataObject());
    genesisFixed.headerUnsafe().setStateHash(res.stateRoot());
    genesisFixed.addState(_genesisState);
    m_blocks.push_back(genesisFixed);
}

ToolResponse ToolChain::mineBlock(EthereumBlock const& _block, State const& _state)
{
    fs::path tmpDir = test::createUniqueTmpDirectory();

    // env.json file
    fs::path envPath = tmpDir / "env.json";
    BlockchainTestFillerEnv env(_block.header().asDataObject(), m_engine);
    writeFile(envPath.string(), env.asDataObject().asJson());

    // alloc.json file
    fs::path allocPath = tmpDir / "alloc.json";
    writeFile(allocPath.string(), _state.asDataObject().asJsonNoFirstKey());

    // txs.json file
    fs::path txsPath = tmpDir / "txs.json";
    DataObject txs(DataType::Array);
    for (auto const& tr : _block.transactions())
        txs.addArrayObject(tr.asDataObject());
    writeFile(txsPath.string(), txs.asJson());

    // output file
    fs::path outPath = tmpDir / "out.json";
    fs::path outAllocPath = tmpDir / "outAlloc.json";

    string cmd = m_toolPath.string();
    cmd += " --input.alloc " + allocPath.string();
    cmd += " --input.txs " + txsPath.string();
    cmd += " --input.env " + envPath.string();
    cmd += " --state.fork " + m_fork.getContent().asString();
    cmd += " --output.result " + outPath.string();
    cmd += " --output.alloc " + outAllocPath.string();
    cmd += " --state.reward " + prepareReward(m_engine, m_fork.getContent(), _block.header().number()).asDecString();

    ETH_TEST_MESSAGE("Alloc:\n" + contentsString(allocPath.string()));
    if (_block.transactions().size())
        ETH_TEST_MESSAGE("Txs:\n" + contentsString(txsPath.string()));
    ETH_TEST_MESSAGE("Env:\n" + contentsString(envPath.string()));
    test::executeCmd(cmd, ExecCMDWarning::NoWarning);
    ETH_TEST_MESSAGE("Res:\n" + contentsString(outPath.string()));
    ETH_TEST_MESSAGE("RAlloc:\n" + contentsString(outAllocPath.string()));
    ETH_TEST_MESSAGE(cmd);

    // Construct block rpc response
    ToolResponse toolResponse(ConvertJsoncppStringToData(contentsString(outPath)));
    spStateIncomplete const toolState(new StateIncomplete(ConvertJsoncppStringToData(contentsString(outAllocPath))));
    toolResponse.attachState(toolState.getCContent());
    return toolResponse;
}

}  // namespace toolimpl
