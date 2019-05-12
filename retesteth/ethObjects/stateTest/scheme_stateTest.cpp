#include "scheme_stateTest.h"
using namespace test;
using namespace std;

scheme_stateTestBase::scheme_stateTestBase(DataObject const& _test):
    object(_test),
    m_checker(_test),
    m_env(_test.at("env")),
    m_pre(_test.at("pre")),
    m_transaction(_test.at("transaction"))
{}

DataObject scheme_stateTestBase::getGenesisForRPC(
    const string& _network, const string& _sealEngine) const
{
    DataObject genesis = prepareGenesisParams(_network, _sealEngine);
    genesis["genesis"] = getEnv().getDataForRPC();
    genesis["accounts"] = getPre().getDataForRPC(_network);
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
    Options const& opt = Options::get();
    if (opt.trDataIndex == -1 && opt.trGasIndex != -1 && opt.trValueIndex != -1)
    {
        for (auto const& tr: m_transaction.getTransactions())
        {
            ETH_FAIL_REQUIRE_MESSAGE(tr.executed == true, "A transaction was specified, but there is no execution results in a test! Transaction: dInd="
            + toString(tr.dataInd) + " gInd=" + toString(tr.gasInd) + " vInd=" + toString(tr.valueInd));
        }
    }
}

scheme_stateTest::scheme_stateTest(DataObject const& _test):
            scheme_stateTestBase(_test),
            m_checker(_test),
            m_post(_test.at("post"))
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

    requireJsonFields(_test.at("_info"), "stateTest " + _test.getKey() + " _info ", {
        {"comment", {{DataType::String}, jsonField::Required} },
        {"source", {{DataType::String}, jsonField::Required} },
        {"sourceHash", {{DataType::String}, jsonField::Required} },
        {"lllcversion", {{DataType::String}, jsonField::Required} },
        {"filledwith", {{DataType::String}, jsonField::Optional} },
        {"filling-rpc-server", {{DataType::String}, jsonField::Optional} },
        {"filling-tool-version", {{DataType::String}, jsonField::Optional} },
    });

    // Check that `data` in compiled test is not just a string but a binary string
    ETH_ERROR_REQUIRE_MESSAGE(
        _test.at("transaction").count("data"), "Field `data` not found in `transaction` section (" +
                                                   TestOutputHelper::get().caseName() + ")");
    ETH_ERROR_REQUIRE_MESSAGE(_test.at("transaction").at("data").type() == DataType::Array,
        "Field `data` in `transaction` section is expected to be Array! (" +
            TestOutputHelper::get().testName() + ")");
    for (auto const& element : _test.at("transaction").at("data").getSubObjects())
        ETH_ERROR_REQUIRE_MESSAGE(stringIntegerType(element.asString()) == DigitsType::HexPrefixed,
            "Field `data` is expected to be binary prefixed with 0x in " +
                TestOutputHelper::get().testName() + ", but got: " + element.asString());
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

scheme_stateTestFiller::scheme_stateTestFiller(DataObject const& _test):
    scheme_stateTestBase(_test),
    m_checker(_test)
{
    // parse expect section
    for (auto const& expect: _test.at("expect").getSubObjects())
    {
        scheme_expectSectionElement expElement(expect);
        m_expect.push_back(expElement);
        for (auto const& net : expElement.getNetworks())
            m_allNetworksDeclaredInExpectSection.emplace(net);
    }

    if (!Options::get().singleTestNet.empty())
    {
        if (m_allNetworksDeclaredInExpectSection.count(Options::get().singleTestNet))
        {
            m_allNetworksDeclaredInExpectSection.clear();
            m_allNetworksDeclaredInExpectSection.emplace(Options::get().singleTestNet);
        }
    }
}
