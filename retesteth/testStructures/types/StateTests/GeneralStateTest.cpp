#include "GeneralStateTest.h"
#include <retesteth/EthChecks.h>
#include <retesteth/helpers/TestHelper.h>
#include <retesteth/helpers/TestOutputHelper.h>
#include <retesteth/testStructures/Common.h>

using namespace std;
namespace test::teststruct
{
void GeneralStateTest::registerAllVectors() const
{
    string execTotal;
    for (auto const& test : m_tests)
    {
        auto const& genTr = test.GeneralTr();

        size_t k = 0;
        size_t vectorSize = genTr.databoxVector().size() * genTr.gasLimitVector().size() * genTr.valueVector().size();
        vectorSize = std::max(vectorSize, (size_t)1);

        string* execs = new string[vectorSize];
        for (size_t d = 0; d < genTr.databoxVector().size(); d++)
            for (size_t g = 0; g < genTr.gasLimitVector().size(); g++)
                for (size_t v = 0; v < genTr.valueVector().size(); v++)
                {
                    string exec;
                    exec += "-t GeneralStateTests/" + TestOutputHelper::get().testInfo().caseName() + " --";
                    exec += " --singletest " + test.testName();
                    exec += " -d " + test::fto_string(d);
                    exec += " -g " + test::fto_string(g);
                    exec += " -v " + test::fto_string(v);
                    execs[k++] = std::move(exec);
                }

        for (auto const& fork : test.Post())
            for (size_t k = 0; k < vectorSize; k++)
                execTotal += execs[k] + " --singlenet " + fork.first.asString() + "\n";

        delete[] execs;
    }
    TestOutputHelper::get().addTestVector(std::move(execTotal));
}

GeneralStateTest::GeneralStateTest(spDataObject& _data)
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
            TestOutputHelper::get().setCurrentTestInfo(TestInfo("GeneralStateTest", el->getKey()));
            m_tests.emplace_back(StateTestInFilled(el));
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
            {"exceptions", {{DataType::Array}, jsonField::Optional}},
            {"env", {{DataType::Object}, jsonField::Required}},
            {"post", {{DataType::Object}, jsonField::Required}},
            {"pre", {{DataType::Object}, jsonField::Required}},
            {"transaction", {{DataType::Object}, jsonField::Required}}});

    // UnitTests
    if (_data->count("exceptions"))
    {
        for (size_t i = _data->atKey("exceptions").getSubObjects().size(); i > 0; i--)
            m_exceptions.emplace_back(_data->atKey("exceptions").getSubObjects().at(i - 1)->asString());
    }

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
        res.reserve(elFork->getSubObjects().size());
        for (auto const& elForkResults : elFork->getSubObjects())
            res.emplace_back(StateTestPostResult(elForkResults));
        if (m_post.count(FORK(elFork->getKey())))
            ETH_ERROR_MESSAGE("StateTest post section has multiple results for the same fork!");
        m_post[FORK(elFork->getKey())] = res;
    }
    m_name = _data->getKey();
}

}  // namespace teststruct
