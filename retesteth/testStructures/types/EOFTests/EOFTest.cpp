#include "EOFTest.h"
#include <retesteth/TestOutputHelper.h>
#include <retesteth/EthChecks.h>
#include <retesteth/testSuites/Common.h>
using namespace std;
using namespace test;
using namespace test::teststruct;

EOFTestVectorInFilled::EOFTestVectorInFilled(DataObject const& _data)
{
    REQUIRE_JSONFIELDS(_data, "EOFTestVector " + _data.getKey(),
        {{"code", {{DataType::String}, jsonField::Required}},
            {"results", {{DataType::Object}, jsonField::Required}}});

    m_data = spBYTES(new BYTES(DataObject(_data.atKey("code").asString())));
    for (auto const& res : _data.atKey("results").getSubObjects())
    {
        if (res->count("exception"))
            m_expectExceptions.emplace(FORK(res->getKey()), res->atKey("exception").asString());
        else
            m_expectExceptions.emplace(FORK(res->getKey()), string());
    }
}

EOFTestInFilled::EOFTestInFilled(spDataObject& _data)
{
    REQUIRE_JSONFIELDS(_data, "EOFTestInFilled " + _data->getKey(),
        {{"_info", {{DataType::Object}, jsonField::Required}},
         {"vectors", {{DataType::Object}, jsonField::Required}}});

    m_name = _data->getKey();
    for (auto const& obj : _data->atKey("vectors").getSubObjects())
        m_vectors.emplace(obj->getKey(), EOFTestVectorInFilled(obj));
}

EOFTest::EOFTest(spDataObject& _data)
{
    try
    {
        ETH_ERROR_REQUIRE_MESSAGE(_data->type() == DataType::Object,
            TestOutputHelper::get().testFile().string() + " A test file must contain an object value (json/yaml).");
        ETH_ERROR_REQUIRE_MESSAGE(_data->getSubObjects().size() == 1,
            TestOutputHelper::get().testFile().string() + " A test file must contain exactly one test!");

        m_tests.reserve(_data.getContent().getSubObjects().size());
        for (auto& el : _data.getContent().getSubObjectsUnsafe())
        {
            TestOutputHelper::get().setCurrentTestInfo(TestInfo("EOFTest", el->getKey()));
            m_tests.emplace_back(EOFTestInFilled(el));
        }
    }
    catch (DataObjectException const& _ex)
    {
        ETH_ERROR_MESSAGE(_ex.what());
    }
}
