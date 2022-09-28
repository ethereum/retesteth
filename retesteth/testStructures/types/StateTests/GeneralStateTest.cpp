#include "GeneralStateTest.h"
#include <retesteth/EthChecks.h>
#include <retesteth/TestOutputHelper.h>
#include <retesteth/testStructures/Common.h>

namespace test
{
namespace teststruct
{
GeneralStateTest::GeneralStateTest(spDataObject& _data)
{
    try
    {
        ETH_ERROR_REQUIRE_MESSAGE(_data->type() == DataType::Object,
            TestOutputHelper::get().get().testFile().string() + " A test file must contain an object value (json/yaml).");
        ETH_ERROR_REQUIRE_MESSAGE(_data->getSubObjects().size() == 1,
            TestOutputHelper::get().get().testFile().string() + " A test file must contain exactly one test!");
        for (auto& el : _data.getContent().getSubObjectsUnsafe())
        {
            TestOutputHelper::get().setCurrentTestInfo(TestInfo("GeneralStateTest", el->getKey()));
            m_tests.push_back(StateTestInFilled(el));
        }
    }
    catch (DataObjectException const& _ex)
    {
        ETH_ERROR_MESSAGE(_ex.what());
    }
}

StateTestInFilled::StateTestInFilled(spDataObject& _data)
{
    REQUIRE_JSONFIELDS(_data, "StateTestInFilled " + _data->getKey(),
        {{"_info", {{DataType::Object}, jsonField::Required}},
            {"env", {{DataType::Object}, jsonField::Required}},
            {"post", {{DataType::Object}, jsonField::Required}},
            {"pre", {{DataType::Object}, jsonField::Required}},
            {"transaction", {{DataType::Object}, jsonField::Required}}});

    m_info = GCP_SPointer<Info>(new Info(_data->atKey("_info")));
    m_env = GCP_SPointer<StateTestEnv>(new StateTestEnv(_data->atKey("env")));

    // -- Some tests has storage keys/values with leading zeros. Convert it to hex value
    for (auto& spAcc : _data.getContent().atKeyUnsafe("pre").getSubObjectsUnsafe())
    {
        DataObject& acc = spAcc.getContent();
        for (auto& rec : acc["storage"].getSubObjectsUnsafe())
        {
            rec.getContent().performModifier(mod_keyToCompactEvenHexPrefixed);
            rec.getContent().performModifier(mod_valueToCompactEvenHexPrefixed);
        }
    }
    // -- REMOVE THIS, FIX THE TESTS
    m_pre = spState(new State(MOVE(_data, "pre")));

    m_transaction = GCP_SPointer<StateTestTransaction>(new StateTestTransaction(_data->atKey("transaction")));
    for (auto const& elFork : _data->atKey("post").getSubObjects())
    {
        StateTestPostResults res;
        for (auto const& elForkResults : elFork->getSubObjects())
            res.push_back(StateTestPostResult(elForkResults));
        if (m_post.count(FORK(elFork->getKey())))
            ETH_ERROR_MESSAGE("StateTest post section has multiple results for the same fork!");
        m_post[FORK(elFork->getKey())] = res;
    }
    m_name = _data->getKey();
}

}  // namespace teststruct
}  // namespace test
