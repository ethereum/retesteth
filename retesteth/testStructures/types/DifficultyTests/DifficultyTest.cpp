#include <Options.h>
#include <TestOutputHelper.h>
#include <retesteth/testStructures/Common.h>

#include "EthChecks.h"
#include "DifficultyTest.h"

using namespace test;
using namespace test::teststruct;

DifficultyTest::DifficultyTest(spDataObject& _data)
{
    try
    {
        ETH_ERROR_REQUIRE_MESSAGE(_data->type() == DataType::Object,
            TestOutputHelper::get().get().testFile().string() + " A test file must contain an object value (json/yaml).");
        ETH_ERROR_REQUIRE_MESSAGE(_data->getSubObjects().size() >= 1,
            TestOutputHelper::get().get().testFile().string() + " A test file must contain at least one test!");
        for (auto& el : _data.getContent().getSubObjectsUnsafe())
        {
            TestOutputHelper::get().setCurrentTestInfo(TestInfo("DifficultyTestFiller", el->getKey()));
            m_tests.push_back(DifficultyTestInFilled(el));
        }
    }
    catch (DataObjectException const& _ex)
    {
        ETH_ERROR_MESSAGE(_ex.what());
    }
}


DifficultyTestInFilled::DifficultyTestInFilled(spDataObject& _data)
{
    try
    {
        auto const& subObjects = _data->getSubObjects();
        if (subObjects.size() < 2)
            throw test::UpwardsException("test missing subobjects (_info + tests)!");

        if (subObjects.at(0)->getKey() != "_info")
            throw test::UpwardsException(_data->getKey() + " missing `_info` section as first subobject!");

        m_name = _data->getKey();
        for (size_t i = 1; i < subObjects.size(); i++)
            m_testVectors.push_back(DifficultyTestVector(subObjects.at(i)));
    }
    catch (std::exception const& _ex)
    {
        ETH_ERROR_MESSAGE(string("DifficultyTestFilled convertion error: ") + _ex.what());
    }
}

DifficultyTestVector::DifficultyTestVector(spDataObject const& _data)
{
    REQUIRE_JSONFIELDS(_data, "DifficultyTestVector " + _data->getKey(),
        {
            {"currentBlockNumber", {{DataType::String}, jsonField::Required}},
            {"currentDifficulty", {{DataType::String}, jsonField::Required}},
            {"currentTimestamp", {{DataType::String}, jsonField::Required}},
            {"network", {{DataType::String}, jsonField::Required}},
            {"parentDifficulty", {{DataType::String}, jsonField::Required}},
            {"parentTimestamp", {{DataType::String}, jsonField::Required}},
            {"parentUncles", {{DataType::String}, jsonField::Required}},
        });

    currentBlockNumber = spVALUE(new VALUE(_data->atKey("currentBlockNumber")));
    currentDifficulty = spVALUE(new VALUE(_data->atKey("currentDifficulty")));
    currentTimestamp = spVALUE(new VALUE(_data->atKey("currentTimestamp")));
    network = spFORK(new FORK(_data->atKey("network")));
    parentDifficulty = spVALUE(new VALUE(_data->atKey("parentDifficulty")));
    parentTimestamp = spVALUE(new VALUE(_data->atKey("parentTimestamp")));
    parentUncles = VALUE(_data->atKey("parentUncles")) == 1;

    testName = _data->getKey();
}
