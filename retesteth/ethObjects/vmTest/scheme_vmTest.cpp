#include "scheme_vmTest.h"
using namespace test;
using namespace std;

scheme_vmTestBase::scheme_vmTestBase(DataObject const& _test)
  : object(_test), m_checker(_test), m_env(_test.atKey("env")), m_pre(_test.atKey("pre"))
{}

scheme_vmTestBase::fieldChecker::fieldChecker(DataObject const& _test)
{
    ETH_ERROR_REQUIRE_MESSAGE(_test.count("env"), "VM test must have 'env' section");
    ETH_ERROR_REQUIRE_MESSAGE(_test.count("pre"), "VM test must have 'pre' section");
}

/*
scheme_vmTest::scheme_vmTest(DataObject const& _test)
  : scheme_vmTestBase(_test), m_checker(_test), m_post(_test.atKey("post"))
{
}

scheme_vmTest::fieldChecker::fieldChecker(DataObject const& _test)
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
        "Field `data` not found in `transaction` section (" + TestInfo::caseName() + ")");
    ETH_ERROR_REQUIRE_MESSAGE(_test.atKey("transaction").atKey("data").type() == DataType::Array,
        "Field `data` in `transaction` section is expected to be Array! (" +
            TestOutputHelper::get().testName() + ")");
    for (auto const& element : _test.atKey("transaction").atKey("data").getSubObjects())
        ETH_ERROR_REQUIRE_MESSAGE(stringIntegerType(element.asString()) == DigitsType::HexPrefixed,
            "Field `data` in `transaction` section is expected to be binary prefixed with `0x` in "
+ TestOutputHelper::get().testName() + ", but got: `" + element.asString() + "`");
}
*/

scheme_vmTestFiller::fieldChecker::fieldChecker(DataObject const& _test)
{
    requireJsonFields(_test, "vmTestFiller " + _test.getKey(),
        {{"_info", {{DataType::Object}, jsonField::Optional}},
            {"env", {{DataType::Object}, jsonField::Required}},
            {"pre", {{DataType::Object}, jsonField::Required}},
            {"exec", {{DataType::Object}, jsonField::Required}},
            {"expect", {{DataType::Object}, jsonField::Optional}},
            {"expectOut", {{DataType::String}, jsonField::Optional}}});
    if (_test.count("expectOut"))
        ETH_WARNING("Unable to verify `expectOut` when creating a stateTest from VMTest " +
                    TestOutputHelper::get().testInfo().errorDebug());
}

DataObject translateExecToTransaction(DataObject const& _exec)
{
    DataObject gtransaction;
    requireJsonFields(_exec, "vmTestFiller exec",
        {{"address", {{DataType::String}, jsonField::Required}},
            {"caller", {{DataType::String}, jsonField::Required}},
            {"data", {{DataType::String}, jsonField::Required}},
            {"code", {{DataType::String}, jsonField::Optional}},
            {"gas", {{DataType::String}, jsonField::Required}},
            {"gasPrice", {{DataType::String}, jsonField::Required}},
            {"origin", {{DataType::String}, jsonField::Required}},
            {"value", {{DataType::String}, jsonField::Required}}});
    gtransaction["data"].addArrayObject(DataObject(_exec.atKey("data").asString()));
    gtransaction["gasLimit"].addArrayObject(DataObject(_exec.atKey("gas").asString()));
    gtransaction["gasPrice"] = _exec.atKey("gasPrice");
    gtransaction["nonce"] = "0";
    gtransaction["secretKey"] = "45a915e4d060149eb4365960e6a7a45f334393093061116b197e3240065ff2d8";
    gtransaction["to"] = _exec.atKey("address");
    gtransaction["value"].addArrayObject(DataObject(_exec.atKey("value").asString()));
    return gtransaction;
}

scheme_vmTestFiller::scheme_vmTestFiller(DataObject const& _test)
  : scheme_vmTestBase(_test),
    m_checker(_test),
    m_gtransaction(translateExecToTransaction(_test.atKey("exec")))
{}
