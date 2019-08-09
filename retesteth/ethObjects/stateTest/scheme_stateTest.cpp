#include "scheme_stateTest.h"
using namespace test;
using namespace std;

scheme_stateTestBase::scheme_stateTestBase(DataObject const& _test)
  : object(_test),
    m_checker(_test),
    m_env(_test.atKey("env")),
    m_pre(_test.atKey("pre")),
    m_transaction(_test.atKey("transaction"))
{}

DataObject scheme_stateTestBase::getGenesisForRPC(
    const string& _network, const string& _sealEngine) const
{
    DataObject genesis = prepareGenesisParams(_network, _sealEngine);
    genesis["genesis"] = getEnv().getDataForRPC();
    for (auto const& acc : getPre().getData().getSubObjects())
        genesis["accounts"].addSubObject(acc);
    return genesis;
}

scheme_stateTestBase::fieldChecker::fieldChecker(DataObject const& _test)
{
    ETH_ERROR_REQUIRE_MESSAGE(_test.count("env"), "State test must have 'env' section");
    ETH_ERROR_REQUIRE_MESSAGE(_test.count("pre"), "State test must have 'pre' section");
    ETH_ERROR_REQUIRE_MESSAGE(
        _test.count("transaction"), "State test must have 'transaction' section");
}

void scheme_stateTestBase::checkUnexecutedTransactions()
{
    bool atLeastOneExecuted = false;
    bool atLeastOneWithoutExpectSection = false;
    for (auto const& tr: m_transaction.getTransactions())
    {
        if (tr.executed)
            atLeastOneExecuted = true;
        bool transactionExecutedOrSkipped = tr.executed == true || tr.skipped == true;
        atLeastOneWithoutExpectSection =
            !transactionExecutedOrSkipped || atLeastOneWithoutExpectSection;
        if (!transactionExecutedOrSkipped)
        {
            string actualTrInfo = " (Errored TR: d: " + to_string(tr.dataInd) +
                                  ", g: " + to_string(tr.gasInd) + ", v: " + to_string(tr.valueInd);
            ETH_MARK_ERROR(
                "A transaction was specified, but there is no execution results in a test! " +
                TestOutputHelper::get().testInfo() + actualTrInfo);
        }
    }
    ETH_ERROR_REQUIRE_MESSAGE(atLeastOneExecuted, "Specified filter did not run a single transaction! " + TestOutputHelper::get().testInfo());
    ETH_ERROR_REQUIRE_MESSAGE(
        !atLeastOneWithoutExpectSection, "Test has transaction uncovered with expect sections! ");
}

scheme_stateTest::scheme_stateTest(DataObject const& _test)
  : scheme_stateTestBase(_test), m_checker(_test), m_post(_test.atKey("post"))
{
}

scheme_stateTest::fieldChecker::fieldChecker(DataObject const& _test)
{
    requireJsonFields(_test, "stateTest " + _test.getKey(), {
        {"_info", {DataType::Object} },
        {"env", {DataType::Object} },
        {"pre", {DataType::Object} },
        {"transaction", {DataType::Object} },
        {"post", {DataType::Object} }
    });

    requireJsonFields(_test.atKey("_info"), "stateTest " + _test.getKey() + " _info ",
        {
            {"comment", {{DataType::String}, jsonField::Required}},
            {"source", {{DataType::String}, jsonField::Required}},
            {"sourceHash", {{DataType::String}, jsonField::Required}},
            {"lllcversion", {{DataType::String}, jsonField::Required}},
            {"filledwith", {{DataType::String}, jsonField::Optional}},
            {"filling-rpc-server", {{DataType::String}, jsonField::Optional}},
            {"filling-tool-version", {{DataType::String}, jsonField::Optional}},
        });

    // Check that `data` in compiled test is not just a string but a binary string
    ETH_ERROR_REQUIRE_MESSAGE(_test.atKey("transaction").count("data"),
        "Field `data` not found in `transaction` section (" + TestOutputHelper::get().caseName() +
            ")");
    ETH_ERROR_REQUIRE_MESSAGE(_test.atKey("transaction").atKey("data").type() == DataType::Array,
        "Field `data` in `transaction` section is expected to be Array! (" +
            TestOutputHelper::get().testName() + ")");
    for (auto const& element : _test.atKey("transaction").atKey("data").getSubObjects())
        ETH_ERROR_REQUIRE_MESSAGE(stringIntegerType(element.asString()) == DigitsType::HexPrefixed,
            "Field `data` in `transaction` section is expected to be binary prefixed with `0x` in " +
                TestOutputHelper::get().testName() + ", but got: `" + element.asString() + "`");
}

scheme_stateTestFiller::fieldChecker::fieldChecker(DataObject const& _test)
{
	if (_test.count("_info"))
		requireJsonFields(_test, "stateTestFiller " + _test.getKey(), {
			{"_info", {DataType::Object} },
			{"env", {DataType::Object} },
			{"pre", {DataType::Object} },
			{"transaction", {DataType::Object} },
			{"expect", {DataType::Array} }
		});
	else
		requireJsonFields(_test, "stateTestFiller " + _test.getKey(), {
			{"env", {DataType::Object} },
			{"pre", {DataType::Object} },
			{"transaction", {DataType::Object} },
			{"expect", {DataType::Array} }
		});
}

scheme_stateTestFiller::scheme_stateTestFiller(DataObject const& _test)
  : scheme_stateTestBase(_test), m_checker(_test), m_expectSection(_test.atKey("expect"))
{
}
