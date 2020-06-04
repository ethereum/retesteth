#include "BlockchainTestFiller.h"
#include <retesteth/EthChecks.h>
#include <retesteth/Options.h>
#include <retesteth/testStructures/Common.h>

using namespace test::teststruct;

BlockchainTestInFiller::BlockchainTestInFiller(DataObject const& _data)
{
    try
    {
        m_hasAtLeastOneUncle = false;
        m_name = _data.getKey();
        if (_data.count("_info"))
            m_info = spInfoIncomplete(new InfoIncomplete(_data.atKey("_info")));
        m_env = spBlockchainTestFillerEnv(new BlockchainTestFillerEnv(_data.atKey("genesisBlockHeader")));
        m_pre = spState(new State(convertDecStateToHex(_data.atKey("pre"))));

        m_sealEngine = SealEngine::NoProof;
        if (_data.count("sealEngine"))
        {
            string const sEngine = _data.atKey("sealEngine").asString();
            if (sEngine == "Ethash")
                m_sealEngine = SealEngine::Ethash;
            else if (sEngine == "NoProof")
                m_sealEngine = SealEngine::NoProof;
            else
                ETH_ERROR_MESSAGE("BlockchainTestInFiller: Unknown sealEngine: " + sEngine);
        }

        // Process expect section
        std::set<FORK> knownForks;
        for (auto const& el : _data.atKey("expect").getSubObjects())
        {
            m_expects.push_back(el);
            BlockchainTestFillerExpectSection const& expect = m_expects.at(m_expects.size() - 1);
            for (auto const& fork : expect.forks())
            {
                if (knownForks.count(fork))
                    ETH_ERROR_MESSAGE("Blockchain test filler expect section contains multiple records of the same fork");
                knownForks.insert(fork);
            }
        }

        ETH_ERROR_REQUIRE_MESSAGE(m_expects.size() > 0, "BlockchainTestFiller require expect section!");

        if (_data.count("exceptions"))
        {
            for (size_t i = _data.atKey("exceptions").getSubObjects().size(); i > 0; i--)
                m_exceptions.push_back(_data.atKey("exceptions").getSubObjects().at(i - 1).asString());
        }

        for (auto const& el : _data.atKey("blocks").getSubObjects())
        {
            m_blocks.push_back(BlockchainTestFillerBlock(el));
            if (m_blocks.at(m_blocks.size() - 1).uncles().size() > 0)
                m_hasAtLeastOneUncle = true;
        }

        requireJsonFields(_data, "BlockchainTestInFiller " + _data.getKey(),
            {{"_info", {{DataType::Object}, jsonField::Optional}},
                {"sealEngine", {{DataType::String}, jsonField::Optional}},
                {"genesisBlockHeader", {{DataType::Object}, jsonField::Required}},
                {"expect", {{DataType::Array}, jsonField::Required}},
                {"exceptions", {{DataType::Array}, jsonField::Optional}},
                {"pre", {{DataType::Object}, jsonField::Required}},
                {"blocks", {{DataType::Array}, jsonField::Required}}});
    }
    catch (std::exception const& _ex)
    {
        ETH_ERROR_MESSAGE(string("BlockchainTestInFiller convertion error: ") + _ex.what());
    }
}


BlockchainTestFiller::BlockchainTestFiller(DataObject const& _data)
{
    try
    {
        TestOutputHelper::get().setCurrentTestInfo(TestInfo("TestFillerInit"));
        ETH_ERROR_REQUIRE_MESSAGE(_data.type() == DataType::Object,
            TestOutputHelper::get().get().testFile().string() + " A test file must contain an object value (json/yaml).");
        ETH_ERROR_REQUIRE_MESSAGE(_data.getSubObjects().size() >= 1,
            TestOutputHelper::get().get().testFile().string() + " A test file must contain at least one test!");
        for (auto const& el : _data.getSubObjects())
        {
            TestOutputHelper::get().setCurrentTestInfo(TestInfo("BlockchainTestFiller", el.getKey()));
            m_tests.push_back(BlockchainTestInFiller(el));
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
        if (el.forks().count(_net))
            return true;
    }
    return false;
}

// Gather all networks from all the expect sections
std::list<FORK> BlockchainTestInFiller::getAllForksFromExpectSections() const
{
    std::list<FORK> out;
    for (auto const& ex : m_expects)
        for (auto const& el : ex.forks())
            out.push_back(el);
    return out;
}
