#include "GeneralStateTest.h"
#include "../../Common.h"
#include <retesteth/EthChecks.h>
#include <retesteth/Options.h>
#include <retesteth/ethObjects/object.h>

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

    // -- Some tests has storage keys/values with leading zeros. Convert it to hex value
    DataObject tmpD = _data.atKey("pre");
    for (auto& acc : tmpD.getSubObjectsUnsafe())
    {
        for (auto& rec : acc["storage"].getSubObjectsUnsafe())
        {
            rec.performModifier(mod_keyToCompactEvenHexPrefixed);
            rec.performModifier(mod_valueToCompactEvenHexPrefixed);
        }
    }
    // -- REMOVE THIS, FIX THE TESTS
    m_pre = spState(new State(tmpD));

    m_transaction = GCP_SPointer<StateTestTransaction>(new StateTestTransaction(_data.atKey("transaction")));
    for (auto const& elFork : _data.atKey("post").getSubObjects())
    {
        StateTestPostResults res;
        for (auto const& elForkResults : elFork.getSubObjects())
            res.push_back(StateTestPostResult(elForkResults));
        m_post[FORK(elFork.getKey())] = res;
    }
    m_name = _data.getKey();
}
