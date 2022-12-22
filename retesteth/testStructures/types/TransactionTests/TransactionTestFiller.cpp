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
        m_tests.reserve(_data->getSubObjects().size());
        for (auto& el : _data.getContent().getSubObjectsUnsafe())
        {
            TestOutputHelper::get().setCurrentTestInfo(TestInfo("TransactionTestFiller", el->getKey()));
            TransactionTestInFiller const test(el);
            m_tests.push_back(std::move(test));
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

        string const c_additionalForks = "additionalForks";
        if (_data->count(c_additionalForks))
        {
            auto const& forkObjects = _data->atKey(c_additionalForks).getSubObjects();
            m_additionalForks.reserve(forkObjects.size());
            for (auto const& additionalFork : forkObjects)
            {
                FORK const fork(additionalFork);
                m_additionalForks.push_back(std::move(fork));
            }
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
