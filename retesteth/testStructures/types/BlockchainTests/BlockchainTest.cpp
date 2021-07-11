#include "BlockchainTest.h"
#include <retesteth/EthChecks.h>
#include <retesteth/Options.h>
#include <retesteth/testStructures/Common.h>

using namespace test::teststruct;
BlockchainTestInFilled::BlockchainTestInFilled(DataObject const& _data)
{
    try
    {
        m_name = _data.getKey();
        m_info = GCP_SPointer<Info>(new Info(_data.atKey("_info")));
        m_env = spBlockchainTestEnv(new BlockchainTestEnv(_data.atKey("genesisBlockHeader")));

        m_genesisRLP = spBYTES(new BYTES(_data.atKey("genesisRLP")));
        m_pre = spState(new State(_data.atKey("pre")));
        m_fork = spFORK(new FORK(_data.atKey("network")));
        m_sealEngine = SealEngine::NoProof;
        if (_data.count("sealEngine") && _data.atKey("sealEngine").asString() == sealEngineToStr(SealEngine::Ethash))
            m_sealEngine = SealEngine::Ethash;

        if (_data.count("postState"))
            m_post = spState(new State(_data.atKey("postState")));
        else
            m_postHash = spFH32(new FH32(_data.atKey("postStateHash")));

        for (auto const& el : _data.atKey("blocks").getSubObjects())
            m_blocks.push_back(BlockchainTestBlock(el));
        m_lastBlockHash = spFH32(new FH32(_data.atKey("lastblockhash")));

        if (_data.count("exceptions"))
        {
            for (size_t i = _data.atKey("exceptions").getSubObjects().size(); i > 0; i--)
                m_exceptions.push_back(_data.atKey("exceptions").getSubObjects().at(i - 1)->asString());
        }
        requireJsonFields(_data, "BlockchainTestInFilled " + _data.getKey(),
            {{"_info", {{DataType::Object}, jsonField::Required}},
             {"sealEngine", {{DataType::String}, jsonField::Optional}},
             {"genesisBlockHeader", {{DataType::Object}, jsonField::Required}},
             {"postState", {{DataType::Object}, jsonField::Optional}},
             {"postStateHash", {{DataType::String}, jsonField::Optional}},
             {"exceptions", {{DataType::Array}, jsonField::Optional}},
             {"genesisRLP", {{DataType::String}, jsonField::Required}},
             {"lastblockhash", {{DataType::String}, jsonField::Required}},
             {"network", {{DataType::String}, jsonField::Required}},
             {"pre", {{DataType::Object}, jsonField::Required}},
             {"blocks", {{DataType::Array}, jsonField::Required}}});
    }
    catch (std::exception const& _ex)
    {
        ETH_ERROR_MESSAGE(string("BlockchainTestInFilled convertion error: ") + _ex.what());
    }
}

BlockchainTest::BlockchainTest(DataObject const& _data)
{
    try
    {
        ETH_ERROR_REQUIRE_MESSAGE(_data.type() == DataType::Object,
            TestOutputHelper::get().get().testFile().string() + " A test file must contain an object value (json/yaml).");
        ETH_ERROR_REQUIRE_MESSAGE(_data.getSubObjects().size() >= 1,
            TestOutputHelper::get().get().testFile().string() + " A test file must contain at least one test!");
        for (auto const& el : _data.getSubObjects())
        {
            TestOutputHelper::get().setCurrentTestInfo(TestInfo("BlockchainTest", el->getKey()));
            m_tests.push_back(BlockchainTestInFilled(el));
        }
    }
    catch (DataObjectException const& _ex)
    {
        ETH_ERROR_MESSAGE(_ex.what());
    }
}

