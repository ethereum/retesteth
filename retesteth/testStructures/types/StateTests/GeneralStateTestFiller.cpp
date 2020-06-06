#include "GeneralStateTestFiller.h"
#include <retesteth/EthChecks.h>
#include <retesteth/Options.h>
#include <retesteth/testStructures/Common.h>

using namespace test::teststruct;
GeneralStateTestFiller::GeneralStateTestFiller(DataObject const& _data)
{
    try
    {
        ETH_ERROR_REQUIRE_MESSAGE(_data.type() == DataType::Object,
            TestOutputHelper::get().get().testFile().string() + " A test file must contain an object value (json/yaml).");
        ETH_ERROR_REQUIRE_MESSAGE(_data.getSubObjects().size() == 1,
            TestOutputHelper::get().get().testFile().string() + " A test file must contain exactly one test!");
        for (auto const& el : _data.getSubObjects())
        {
            TestOutputHelper::get().setCurrentTestInfo(TestInfo("GeneralStateTestFiller", el.getKey()));
            m_tests.push_back(StateTestInFiller(el));
        }
    }
    catch (DataObjectException const& _ex)
    {
        ETH_ERROR_MESSAGE(_ex.what());
    }
}

StateTestInFiller::StateTestInFiller(DataObject const& _data)
{
    try
    {
        if (_data.count("_info"))
            m_info = GCP_SPointer<InfoIncomplete>(new InfoIncomplete(_data.atKey("_info")));
        m_env = GCP_SPointer<StateTestFillerEnv>(new StateTestFillerEnv(_data.atKey("env")));
        m_pre = spState(new State(convertDecStateToHex(_data.atKey("pre"))));
        m_transaction = GCP_SPointer<StateTestFillerTransaction>(new StateTestFillerTransaction(_data.atKey("transaction")));
        for (auto const& el : _data.atKey("expect").getSubObjects())
            m_expectSections.push_back(StateTestFillerExpectSection(el));
        ETH_ERROR_REQUIRE_MESSAGE(m_expectSections.size() > 0, "StateTestFiller require expect sections!");
        m_name = _data.getKey();

        requireJsonFields(_data, "StateTestInFiller " + _data.getKey(),
            {{"_info", {{DataType::Object}, jsonField::Optional}},
             {"env", {{DataType::Object}, jsonField::Required}},
             {"expect", {{DataType::Array}, jsonField::Required}},
             {"pre", {{DataType::Object}, jsonField::Required}},
             {"transaction", {{DataType::Object}, jsonField::Required}}});

    }
    catch (std::exception const& _ex)
    {
        ETH_ERROR_MESSAGE(string("StateTestFiller convertion error: ") + _ex.what());
    }
}

// Gather all networks from all the expect sections
std::set<FORK> StateTestInFiller::getAllForksFromExpectSections() const
{
    std::set<FORK> out;
    for (auto const& ex : m_expectSections)
        for (auto const& el : ex.forks())
            out.emplace(el);
    return out;
}
