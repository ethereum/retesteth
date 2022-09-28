#include "GeneralStateTestFiller.h"
#include <retesteth/EthChecks.h>
#include <retesteth/TestOutputHelper.h>
#include <retesteth/testStructures/Common.h>

using namespace std;
using namespace test::teststruct;
using namespace test::compiler;

GeneralStateTestFiller::GeneralStateTestFiller(spDataObject& _data)
{
    try
    {
        ETH_ERROR_REQUIRE_MESSAGE(_data->type() == DataType::Object,
            TestOutputHelper::get().get().testFile().string() + " A test file must contain an object value (json/yaml).");
        ETH_ERROR_REQUIRE_MESSAGE(_data->getSubObjects().size() == 1,
            TestOutputHelper::get().get().testFile().string() + " A test file must contain exactly one test!");
        for (auto& el : _data.getContent().getSubObjectsUnsafe())
        {
            TestOutputHelper::get().setCurrentTestInfo(TestInfo("GeneralStateTestFiller", el->getKey()));
            m_tests.push_back(StateTestInFiller(el));
        }
    }
    catch (DataObjectException const& _ex)
    {
        ETH_ERROR_MESSAGE(_ex.what());
    }
}

StateTestInFiller::StateTestInFiller(spDataObject& _data)
{
    try
    {
        REQUIRE_JSONFIELDS(_data, "StateTestInFiller " + _data->getKey(),
            {{"_info", {{DataType::Object}, jsonField::Optional}},
                {"env", {{DataType::Object}, jsonField::Required}},
                {"expect", {{DataType::Array}, jsonField::Required}},
                {"pre", {{DataType::Object}, jsonField::Required}},
                {"solidity", {{DataType::String}, jsonField::Optional}},
                {"verify", {{DataType::Object}, jsonField::Optional}},
                {"verifyBC", {{DataType::Object}, jsonField::Optional}},
                {"transaction", {{DataType::Object}, jsonField::Required}}});

        if (_data->count("_info"))
            m_info = GCP_SPointer<InfoIncomplete>(new InfoIncomplete(MOVE(_data, "_info")));
        m_env = GCP_SPointer<StateTestFillerEnv>(new StateTestFillerEnv(MOVE(_data, "env")));

        // Compile solidity contracts from separate field
        // Because one solidity contract may depend on another during the compilation
        solContracts solidityCode;
        if (_data->count("solidity"))
            solidityCode = test::compiler::compileSolidity(_data->atKey("solidity").asString());

        convertDecStateToHex((*_data).atKeyPointerUnsafe("pre"), solidityCode); // "Pre" section
        m_pre = spState(new State(MOVE(_data, "pre")));
        m_transaction = spStateTestFillerTransaction(new StateTestFillerTransaction(MOVE(_data, "transaction")));

        for (auto& el : (*_data).atKeyUnsafe("expect").getSubObjectsUnsafe())
            m_expectSections.push_back(StateTestFillerExpectSection(dataobject::move(el), m_transaction));
        ETH_ERROR_REQUIRE_MESSAGE(m_expectSections.size() > 0, "StateTestFiller require expect sections!");

        m_name = _data->getKey();
        if (_data->count("verify"))
        {
            spDataObjectMove m = MOVE(_data, "verify");
            m_verify = m.getPointer();
        }
        if (_data->count("verifyBC"))
        {
            spDataObjectMove m = MOVE(_data, "verifyBC");
            m_verifyBC = m.getPointer();
        }
    }
    catch (std::exception const& _ex)
    {
        ETH_ERROR_MESSAGE(string("StateTestFiller convertion error: ") + _ex.what());
    }
}

// Gather all networks from all the expect sections
std::set<FORK> StateTestInFiller::getAllForksFromExpectSections() const
{
    std::set<FORK> allForksMentionedInExpectSections;
    for (auto const& expectSection : m_expectSections)
        for (auto const& fork : expectSection.forks())
            allForksMentionedInExpectSections.emplace(fork);
    return allForksMentionedInExpectSections;
}
