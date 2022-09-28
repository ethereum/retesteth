#include <TestOutputHelper.h>
#include <retesteth/testStructures/Common.h>
#include "EthChecks.h"
#include "TransactionTestFiller.h"

using namespace std;
using namespace test;
using namespace test::teststruct;

TransactionTestFiller::TransactionTestFiller(spDataObject& _data)
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
            m_tests.push_back(TransactionTestInFiller(el));
        }
    }
    catch (DataObjectException const& _ex)
    {
        ETH_ERROR_MESSAGE(_ex.what());
    }
}


TransactionTestInFiller::TransactionTestInFiller(spDataObject& _data)
{
    try
    {
        REQUIRE_JSONFIELDS(_data, "TransactionTestInFiller " + _data->getKey(),
            {{"_info", {{DataType::Object}, jsonField::Optional}},
             {"additionalForks", {{DataType::Array}, jsonField::Optional}},
             {"expectException", {{DataType::Object}, jsonField::Required}},
             {"transaction", {{DataType::Object}, jsonField::Required}}});

        m_name = _data->getKey();
        if (_data->count("_info"))
            m_info = GCP_SPointer<InfoIncomplete>(new InfoIncomplete(MOVE(_data, "_info")));

        if (_data->count("additionalForks"))
        {
            for (auto const& additionalFork : _data->atKey("additionalForks").getSubObjects())
                m_additionalForks.push_back(FORK(additionalFork));
        }

        readExpectExceptions(_data->atKey("expectException"), m_expectExceptions);

        convertDecTransactionToHex(_data.getContent().atKeyPointerUnsafe("transaction"));
        m_transaction = readTransaction(MOVE(_data, "transaction"));
    }
    catch (std::exception const& _ex)
    {
        ETH_ERROR_MESSAGE(string("TransactionTestFiller convertion error: ") + _ex.what());
    }
}
