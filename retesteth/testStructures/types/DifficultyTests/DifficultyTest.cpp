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
            TestOutputHelper::get().setCurrentTestInfo(TestInfo("TransactionTestFiller", el->getKey()));
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
        REQUIRE_JSONFIELDS(_data, "DifficultyTestInFilled " + _data->getKey(),
            {{"_info", {{DataType::Object}, jsonField::Required}},
             {"result", {{DataType::Object}, jsonField::Required}},
             {"txbytes", {{DataType::String}, jsonField::Required}}});

        m_name = _data->getKey();


    }
    catch (std::exception const& _ex)
    {
        ETH_ERROR_MESSAGE(string("DifficultyTestFilled convertion error: ") + _ex.what());
    }
}
