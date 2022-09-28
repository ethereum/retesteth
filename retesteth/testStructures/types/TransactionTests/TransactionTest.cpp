#include <TestOutputHelper.h>
#include <retesteth/testStructures/Common.h>
#include "EthChecks.h"
#include "TransactionTest.h"

using namespace std;
using namespace test;
using namespace test::teststruct;

TransactionTest::TransactionTest(spDataObject& _data)
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
            m_tests.push_back(TransactionTestInFilled(el));
        }
    }
    catch (DataObjectException const& _ex)
    {
        ETH_ERROR_MESSAGE(_ex.what());
    }
}


TransactionTestInFilled::TransactionTestInFilled(spDataObject& _data)
{
    try
    {
        REQUIRE_JSONFIELDS(_data, "TransactionTestInFilled " + _data->getKey(),
            {{"_info", {{DataType::Object}, jsonField::Required}},
             {"result", {{DataType::Object}, jsonField::Required}},
             {"txbytes", {{DataType::String}, jsonField::Required}}});

        m_name = _data->getKey();
        m_rlp = spBYTES(new BYTES(_data->atKey("txbytes").asString()));

        try {
            m_readTransaction = readTransaction(m_rlp);
        }
        catch (...)
        {
            ETH_WARNING("Unable to read transaction from 'txbytes'");
            m_readTransaction = spTransaction(0);
        }

        for (auto const& el : _data->atKey("result").getSubObjects())
        {
            FORK fork(el->getKey());
            m_forks.emplace(fork);
            if (el->count("exception"))
            {
                REQUIRE_JSONFIELDS(el, "TransactionTestInFilled::result ",
                    {{"exception", {{DataType::String}, jsonField::Required}},
                        {"intrinsicGas", {{DataType::String}, jsonField::Required}}});
                m_expectExceptions.emplace(fork, el->atKey("exception").asString());
            }
            else
            {
                REQUIRE_JSONFIELDS(el, "TransactionTestInFilled::result ",
                    {{"hash", {{DataType::String}, jsonField::Required}},
                        {"sender", {{DataType::String}, jsonField::Required}},
                        {"intrinsicGas", {{DataType::String}, jsonField::Required}}});
                spFH32 hash(new FH32(el->atKey("hash")));
                spFH20 sender(new FH20(el->atKey("sender")));
                spVALUE intrGas(new VALUE(el->atKey("intrinsicGas")));
                m_acceptedTransactions.emplace(fork, TransactionResult(hash, sender, intrGas));
            }
        }
    }
    catch (std::exception const& _ex)
    {
        ETH_ERROR_MESSAGE(string("TransactionTestFilled convertion error: ") + _ex.what());
    }
}
