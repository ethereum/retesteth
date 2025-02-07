#include "BlockchainTestFiller.h"
#include <retesteth/EthChecks.h>
#include <retesteth/helpers/TestOutputHelper.h>
#include <retesteth/testStructures/Common.h>
#include <Options.h>

using namespace std;
using namespace test::teststruct;

namespace test::teststruct
{
BlockchainTestInFiller::BlockchainTestInFiller(spDataObject& _data)
{
    try
    {
        REQUIRE_JSONFIELDS(_data, "BlockchainTestInFiller " + _data->getKey(),
            {{"_info", {{DataType::Object}, jsonField::Optional}},
                {"sealEngine", {{DataType::String}, jsonField::Optional}},
                {"genesisBlockHeader", {{DataType::Object}, jsonField::Required}},
                {"expect", {{DataType::Array}, jsonField::Required}},
                {"exceptions", {{DataType::Array}, jsonField::Optional}},
                {"verify", {{DataType::Object}, jsonField::Optional}},
                {"pre", {{DataType::Object}, jsonField::Required}},
                {"blocks", {{DataType::Array}, jsonField::Required}}});

        m_hasAtLeastOneUncle = false;
        m_name = _data->getKey();
        if (_data->count("_info"))
            m_info = spInfoIncomplete(new InfoIncomplete(MOVE(_data, "_info")));
        convertDecStateToHex((*_data).atKeyPointerUnsafe("pre"));
        m_pre = spState(new State(MOVE(_data, "pre")));
        m_hasEmptyAccounts = checkEmptyAccounts(m_pre);

        // Prepare nonce map for transaction 'auto' nonce parsing
        NonceMap nonceMap;
        for (auto const& acc : m_pre->accounts())
            nonceMap.emplace(acc.first.asString(), spVALUE(new VALUE(acc.second->nonce().asBigInt())));
        // nonce map

        m_sealEngine = SealEngine::NoProof;
        if (_data->count("sealEngine"))
        {
            string const sEngine = _data->atKey("sealEngine").asString();
            if (sEngine == sealEngineToStr(SealEngine::Ethash))
                m_sealEngine = SealEngine::Ethash;
            else if (sEngine == sealEngineToStr(SealEngine::NoProof))
                m_sealEngine = SealEngine::NoProof;
            else
                ETH_ERROR_MESSAGE("BlockchainTestInFiller: Unknown sealEngine: " + sEngine);
        }

        m_env = spBlockchainTestFillerEnv(readBlockchainFillerTestEnv(MOVE(_data, "genesisBlockHeader"), m_sealEngine));

        // Process expect section
        std::set<FORK> knownForks;
        for (auto& el2 : (*_data).atKeyUnsafe("expect").getSubObjectsUnsafe())
        {
            m_expects.emplace_back(el2);
            BlockchainTestFillerExpectSection const& expect = m_expects.at(m_expects.size() - 1);
            for (auto const& fork : expect.forks())
            {
                if (knownForks.count(fork))
                    ETH_ERROR_MESSAGE("Blockchain test filler expect section contains multiple records of the same fork");
                //if (compareFork(fork, CMP::lt, FORK("Cancun")))
                //    ETH_ERROR_MESSAGE("Test has fork <Cancun " + TestOutputHelper::get().testInfo().errorDebug());
                knownForks.insert(fork);
            }
        }

        ETH_ERROR_REQUIRE_MESSAGE(m_expects.size() > 0, "BlockchainTestFiller require expect section!");

        // UnitTests
        if (_data->count("exceptions"))
        {
            for (size_t i = _data->atKey("exceptions").getSubObjects().size(); i > 0; i--)
                m_exceptions.emplace_back(_data->atKey("exceptions").getSubObjects().at(i - 1)->asString());
        }

        if (_data->count("verify"))
        {
            spDataObjectMove m = MOVE(_data, "verify");
            m_verify = m.getPointer();
        }

        m_hasAtLeastOneUncle = false;

        string const c_blocks = "blocks";
        m_blocks.reserve(_data->atKey(c_blocks).getSubObjects().size());
        for (auto& el : (*_data).atKeyUnsafe(c_blocks).getSubObjectsUnsafe())
        {
            m_blocks.emplace_back(BlockchainTestFillerBlock(el, nonceMap));
            if (m_blocks.at(m_blocks.size() - 1).uncles().size() > 0)
                m_hasAtLeastOneUncle = true;
        }

        auto const& opt = Options::get();
        if (opt.isLegacyConstantinople() || opt.isLegacy() || opt.isEIPTest() || opt.isEOFTest())
        {}
        else
        {
            auto const forks = getAllForksFromExpectSections();
            m_config = GCP_SPointer<BlockchainTestFillerConfig>(new BlockchainTestFillerConfig(knownForks));
        }
    }
    catch (std::exception const& _ex)
    {
        ETH_ERROR_MESSAGE(string("BlockchainTestInFiller convertion error: ") + _ex.what());
    }
}

BlockchainTestFiller::BlockchainTestFiller(spDataObject& _data)
{
    try
    {
        TestOutputHelper::get().setCurrentTestInfo(TestInfo("TestFillerInit"));
        ETH_ERROR_REQUIRE_MESSAGE(_data->type() == DataType::Object,
            TestOutputHelper::get().get().testFile().string() + " A test file must contain an object value (json/yaml).");
        ETH_ERROR_REQUIRE_MESSAGE(_data->getSubObjects().size() >= 1,
            TestOutputHelper::get().get().testFile().string() + " A test file must contain at least one test!");
        for (auto& el : _data.getContent().getSubObjectsUnsafe())
        {
            TestOutputHelper::get().setCurrentTestInfo(TestInfo("BlockchainTestFiller", el->getKey()));
            m_tests.emplace_back(BlockchainTestInFiller(el));
        }
    }
    catch (DataObjectException const& _ex)
    {
        ETH_ERROR_MESSAGE(_ex.what());
    }
}


bool BlockchainTestInFiller::hasExpectForNetwork(FORK const& _net) const
{
    for (auto const& el : m_expects)
    {
        for (auto const& el : el.forks())
            if (el == _net)
                return true;
    }
    return false;
}

// Gather all networks from all the expect sections
std::set<FORK> BlockchainTestInFiller::getAllForksFromExpectSections() const
{
    std::set<FORK> out;
    for (auto const& ex : m_expects)
        for (auto const& el : ex.forks())
            out.emplace(el);
    return out;
}

}  // namespace teststruct
