#include "GeneralStateTest.h"
#include "../../Common.h"
#include <retesteth/EthChecks.h>
#include <retesteth/Options.h>

using namespace test::teststruct;
GeneralStateTest::GeneralStateTest(DataObject const& _data)
{
    try
    {
        ETH_ERROR_REQUIRE_MESSAGE(_data.type() == DataType::Object,
            TestOutputHelper::get().get().testFile().string() + " A test file must contain an object value (json/yaml).");
        for (auto const& el : _data.getSubObjects())
        {
            TestOutputHelper::get().setCurrentTestInfo(TestInfo("GeneralStateTest", el.getKey()));
            m_tests.push_back(StateTestInFilled(el));
        }
    }
    catch (DataObjectException const& _ex)
    {
        ETH_ERROR_MESSAGE(_ex.what());
    }
}

StateTestInFilled::StateTestInFilled(DataObject const& _data)
{
    m_info = GCP_SPointer<Info>(new Info(_data.atKey("_info")));
    m_env = GCP_SPointer<StateTestEnv>(new StateTestEnv(_data.atKey("env")));
    m_pre = spState(new State(_data.atKey("pre")));
    m_transaction = GCP_SPointer<StateTestTransaction>(new StateTestTransaction(_data.atKey("transaction")));
    // for (auto const& el : _data.atKey("expect").getSubObjects())
    //    m_expectSections.push_back(StateTestFillerExpectSection(el));
    m_name = _data.getKey();
}
