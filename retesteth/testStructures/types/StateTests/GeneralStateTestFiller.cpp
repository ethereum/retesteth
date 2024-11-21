#include "GeneralStateTestFiller.h"
#include <retesteth/EthChecks.h>
#include <retesteth/helpers/TestOutputHelper.h>
#include <retesteth/testStructures/Common.h>
#include <retesteth/helpers/TestHelper.h>
#include <retesteth/testStructures/configs/FORK.h>
#include <retesteth/Constants.h>
#include <retesteth/Options.h>

using namespace std;
using namespace test::teststruct;
using namespace test::compiler;
using namespace test;

namespace  {

void checkCoinbaseInExpectSection(StateTestFillerExpectSection const& _expect, GCP_SPointer<StateTestFillerEnv> const& _env)
{
    for (auto const& acc : _expect.result().accounts())
    {
        if (acc.first == _env.getCContent().currentCoinbase())
        {
            if (acc.second.getCContent().hasBalance())
            {
                auto const& debug = TestOutputHelper::get().testInfo().errorDebug();
                ETH_WARNING("Expect section checking coinbase balance! " + acc.first.asString() + debug);
            }
            break;
        }
        else if (acc.first == FH20("0xa94f5374fce5edbc8e2a8697c15331677e6ebf0b"))
        {
            auto const& accAddress = acc.second.getCContent();
            if (accAddress.hasBalance() && accAddress.balance() != 0)
            {
                auto const& debug = TestOutputHelper::get().testInfo().errorDebug();
                ETH_WARNING("Expect section checking sender balance! " + acc.first.asString() + debug);
            }
            break;
        }
    }
}
void checkRedundantExpectSection(std::vector<StateTestFillerExpectSection> const& _readExpectSections, StateTestFillerExpectSection const& _newExpectSection)
{
    for (auto const& readExpect : _readExpectSections)
    {
        bool forkMatch = false;
        for (auto const& newFork : _newExpectSection.forks())
        {
            if (test::inArray(readExpect.forks(), newFork))
            {
                forkMatch = true;
                break;
            }
        }

        if (forkMatch)
        {
            for (int d : _newExpectSection.getDataInd())
                for (int g : _newExpectSection.getGasInd())
                    for (int v : _newExpectSection.getValInd())
                    {
                        if (readExpect.checkIndexes(d, g, v))
                            ETH_ERROR_MESSAGE("Test filler read redundant expect section: \n" + _newExpectSection.initialData().asJson());
                    }
        }
    }
}
}

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
            m_tests.emplace_back(StateTestInFiller(el));
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
                {"exceptions", {{DataType::Array}, jsonField::Optional}},
                {"env", {{DataType::Object}, jsonField::Required}},
                {"expect", {{DataType::Array}, jsonField::Required}},
                {"pre", {{DataType::Object}, jsonField::Required}},
                {"solidity", {{DataType::String}, jsonField::Optional}},
                {"verify", {{DataType::Object}, jsonField::Optional}},
                {"verifyBC", {{DataType::Object}, jsonField::Optional}},
                {"transaction", {{DataType::Object}, jsonField::Required}}});

        // UnitTests
        if (_data->count("exceptions"))
        {
            for (size_t i = _data->atKey("exceptions").getSubObjects().size(); i > 0; i--)
                m_exceptions.emplace_back(_data->atKey("exceptions").getSubObjects().at(i - 1)->asString());
        }
        TestOutputHelper::get().setUnitTestExceptions(m_exceptions);

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
        m_hasEmptyAccounts = checkEmptyAccounts(m_pre);

        m_hasBigInt = _data->atKey("transaction").performSearch(src_findBigInt);
        m_transaction = spStateTestFillerTransaction(new StateTestFillerTransaction(MOVE(_data, "transaction")));

        string const c_expect = "expect";
        m_expectSections.reserve((*_data).atKey(c_expect).getSubObjects().size());
        for (auto& el : (*_data).atKeyUnsafe(c_expect).getSubObjectsUnsafe())
        {
            StateTestFillerExpectSection newSection(dataobject::move(el), m_transaction);
            checkRedundantExpectSection(m_expectSections, newSection);
            checkCoinbaseInExpectSection(newSection, m_env);
            m_expectSections.emplace_back(newSection);
        }
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
    {
        for (auto const& fork : expectSection.forks())
        {
            //if (compareFork(fork, CMP::lt, FORK("Cancun")))
            //    ETH_ERROR_MESSAGE("Test has fork <Cancun " + TestOutputHelper::get().testInfo().errorDebug());
            allForksMentionedInExpectSections.emplace(fork);
        }
    }
    return allForksMentionedInExpectSections;
}

