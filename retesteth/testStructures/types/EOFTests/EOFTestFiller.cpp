#include "EOFTestFiller.h"
#include <retesteth/EthChecks.h>
#include <retesteth/TestOutputHelper.h>
#include <retesteth/Constants.h>
#include <retesteth/testStructures/Common.h>
#include <retesteth/Options.h>
#include <retesteth/compiler/Compiler.h>

using namespace std;
using namespace test;
using namespace test::teststruct;


EOFTestInFiller::EOFTestInFiller(spDataObject& _data)
{
    try
    {
        REQUIRE_JSONFIELDS(_data, "EOFTestInFiller " + _data->getKey(),
            {{"_info", {{DataType::Object}, jsonField::Optional}},
                {"forks", {{DataType::Array}, jsonField::Required}},
                {"vectors", {{DataType::Array}, jsonField::Required}}});

        m_name = _data->getKey();

        if (_data->count("_info"))
            m_info = GCP_SPointer<InfoIncomplete>(new InfoIncomplete(MOVE(_data, "_info")));

        std::set<string> forks;
        for (auto const& fork: _data->atKey("forks").getSubObjects())
            forks.emplace(fork->asString());
        m_forks = Options::getCurrentConfig().translateNetworks(forks);

        for (auto& el : _data.getContent().atKeyUnsafe("vectors").getSubObjectsUnsafe())
            m_vectors.emplace_back(EOFTestVectorInFiller(el));
    }
    catch (std::exception const& _ex)
    {
        ETH_ERROR_MESSAGE(string("EOFTestFiller convertion error: ") + _ex.what());
    }
}

EOFTestVectorInFiller::EOFTestVectorInFiller(spDataObject& _data)
{
    REQUIRE_JSONFIELDS(_data, "EOFTestVector " + _data->getKey(),
        {{"data", {{DataType::String}, jsonField::Required}},
         {"expectException", {{DataType::Object}, jsonField::Optional}}});
    m_dataPreview = _data->atKey("data").asString().substr(0, 17);
    m_data = spBYTES(new BYTES(DataObject(compiler::replaceCode(_data->atKey("data").asString()))));
    if (_data->count("expectException"))
        readExpectExceptions(_data->atKey("expectException"), m_expectExceptions);
}


EOFTestFiller::EOFTestFiller(spDataObject& _data)
{
    try
    {
        ETH_ERROR_REQUIRE_MESSAGE(_data->type() == DataType::Object,
            TestOutputHelper::get().get().testFile().string() + " A test file must contain an object value (json/yaml).");
        ETH_ERROR_REQUIRE_MESSAGE(_data->getSubObjects().size() == 1,
            TestOutputHelper::get().get().testFile().string() + " A test file must contain exactly one test!");
        for (auto& el : _data.getContent().getSubObjectsUnsafe())
        {
            TestOutputHelper::get().setCurrentTestInfo(TestInfo("EOFTestFiller", el->getKey()));
            m_tests.emplace_back(EOFTestInFiller(el));
        }
    }
    catch (DataObjectException const& _ex)
    {
        ETH_ERROR_MESSAGE(_ex.what());
    }
}


std::string const& EOFTestVectorInFiller::getExpectException(FORK const& _net) const
{
    if (m_expectExceptions.count(_net))
        return m_expectExceptions.at(_net);
    return C_EMPTY_STR;
}
