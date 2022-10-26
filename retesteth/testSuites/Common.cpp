#include "Common.h"
#include <retesteth/Options.h>
#include <retesteth/TestHelper.h>
#include <retesteth/TestOutputHelper.h>
using namespace std;
using namespace test::debug;
namespace test
{

void checkDataObject(DataObject const& _input)
{
    ETH_ERROR_REQUIRE_MESSAGE(_input.type() == DataType::Object,
        TestOutputHelper::get().get().testFile().string() + " A test file must contain an object value (json/yaml).");
}

void checkAtLeastOneTest(DataObject const& _input)
{
    ETH_ERROR_REQUIRE_MESSAGE(
        _input.getSubObjects().size() >= 1, " A test file must contain at least one test: " +
                                                TestOutputHelper::get().testFile().string());

    for (auto const& test : _input.getSubObjects())
    {
        ETH_ERROR_REQUIRE_MESSAGE(
            test->type() == DataType::Object, TestOutputHelper::get().testFile().string() +
                                                 " should contain an object under a test name.");
    }
}

void checkOnlyOneTest(DataObject const& _input)
{
    ETH_ERROR_REQUIRE_MESSAGE(_input.getSubObjects().size() == 1,
        " A test file must contain only one test: " + TestOutputHelper::get().testFile().string());

    ETH_ERROR_REQUIRE_MESSAGE(_input.getSubObjects().at(0)->type() == DataType::Object,
        TestOutputHelper::get().testFile().string() +
            " should contain an object under a test name.");
}

void checkTestNameIsEqualToFileName(string const& _testName)
{
    if (!TestOutputHelper::get().testFile().empty())
    {
        string const tFileName = TestOutputHelper::get().testFile().stem().string();
        ETH_ERROR_REQUIRE_MESSAGE(_testName + "Filler" == tFileName,
            TestOutputHelper::get().testFile().string() +
                " contains a test with a different name '" + _testName + "'");
    }
}

void checkTestNameIsEqualToFileName(DataObject const& _input)
{
    if (!TestOutputHelper::get().testFile().empty())
        ETH_ERROR_REQUIRE_MESSAGE(_input.getSubObjects().at(0)->getKey() + "Filler" ==
                                      TestOutputHelper::get().testFile().stem().string(),
            TestOutputHelper::get().testFile().string() +
                " contains a test with a different name '" + _input.getSubObjects().at(0)->getKey() +
                "'");
}

void printVmTrace(VMtraceinfo const& _info)
{
    DebugVMTrace ret(_info.session.debug_traceTransaction(_info.trHash));

    ETH_DC_MESSAGE(DC::TESTLOG, "------------------------");
    if (Options::get().vmtraceraw)
    {
        if (!Options::get().vmtraceraw.outpath.empty())
        {
            auto outpath = fs::path(Options::get().vmtraceraw.outpath);
            ETH_DC_MESSAGE(DC::TESTLOG, "Export vmtraceraw to " + (outpath / _info.trName).string());
            ret.exportLogs(outpath / _info.trName);
        }
        else
            ret.print();
    }
    else
        ret.printNice();

    DataObject state;
    state["stateRoot"] = _info.stateRoot.asString();
    ETH_DC_MESSAGE(DC::TESTLOG, state.asJson(0, false));
    ETH_DC_MESSAGE(DC::TESTLOG, "\n------------------------");
}

void compareTransactionException(spTransaction const& _tr, MineBlocksResult const& _mRes, string const& _testException)
{
    if (!_mRes.isRejectData() && !_testException.empty())
    {
        ETH_WARNING("Looks like client does not support rejected transaction information!");
        return;
    }
    // Mine a block, execute transaction
    FH32 const& trHash = _tr->hash();
    string const remoteException = _mRes.getTrException(trHash);
    if (!_testException.empty() && remoteException.empty())
        ETH_ERROR_MESSAGE("Client didn't reject transaction: (" + trHash.asString() + ") \n" + _tr->getRawBytes().asString() +
            "\nTest Expected: " + _testException);
    if (_testException.empty() && !remoteException.empty())
        ETH_ERROR_MESSAGE("Client reject transaction expected to be valid: (" + trHash.asString() + ") \n" + _tr->getRawBytes().asString() +
                          "\nReason: " + remoteException);

    if (!_testException.empty() && !remoteException.empty())
    {
        string const& expectedReason = Options::getCurrentConfig().translateException(_testException);
        if (remoteException.find(expectedReason) == string::npos)
        {
            ETH_WARNING(_tr->asDataObject()->asJson());
            ETH_ERROR_MESSAGE(string("Transaction rejected but due to a different reason: \n") +
               "Expected reason: `" + expectedReason + "` (" + _testException + ")\n" +
               "Client reason: `" + remoteException
              );
        }
    }
}


void verifyFilledTest(DataObject const& _want, DataObject const& _have)
{
    string root;
    if (_want.getSubObjects().size() != 0)
        verifyFilledTestRecursive(_want, _have, root);
}

void verifyFilledTest(DataObject const& _want, DataObject const& _have, FORK const& _net)
{
    string root = _net.asString() + " -> ";
    if (_want.getSubObjects().size() != 0)
    {
        if (_want.count(_net.asString()))
            verifyFilledTestRecursive(_want.atKey(_net.asString()), _have, root);
        else
            ETH_WARNING("retesteth::verify does not have fork: " + _net.asString());
    }
}

void verifyFilledTestRecursive(DataObject const& _want, DataObject const& _have, string& _debug)
{
    size_t k = 0;
    for (auto const& el : _want.getSubObjects())
    {
        if (_want.type() == DataType::Object)
        {
            bool mustNotExist = false;
            if (el->type() == DataType::String && el->asString() == "shouldnotexist")
                mustNotExist = true;

            if (!mustNotExist && !_have.count(el->getKey()) )
                ETH_ERROR_MESSAGE("verify: filled test missing expected key: " +
                                  _debug + "\n Key: `" + el->getKey() + "`");
            if (mustNotExist && _have.count(el->getKey()))
                ETH_ERROR_MESSAGE("verify: filled test has unexpected key: " +
                                  _debug + "\n Key: `" + el->getKey() + "`");
            if (mustNotExist && !_have.count(el->getKey()))
                continue;

            size_t pos = _debug.size();
            _debug += el->getKey() + " -> ";
            if (el->getSubObjects().size() > 0)
                verifyFilledTestRecursive(el, _have.atKey(el->getKey()), _debug);

            if (el->getSubObjects().size() == 0 && _have.getSubObjects().size() != 0)
                verifyFilledTestRecursive(el, _have.atKey(el->getKey()), _debug);
            _debug = _debug.substr(0, pos - 1);
        }
        if (_want.type() == DataType::Array)
        {
            if (_have.getSubObjects().size() <= k)
                ETH_ERROR_MESSAGE("verify: filled test missing expected array element: " +
                                  _debug + "`" + el->asJson() + "`");
            _debug += "[" + test::fto_string(k) + "] -> ";
            verifyFilledTestRecursive(el, _have.at(k), _debug);
        }
        k++;
    }

    if (_want.getSubObjects().size() == 0 && _have.getSubObjects().size() == 0)
    {
        auto const& elWantJson = _want.asJson();
        auto const& haveJson = _have.asJson();
        if (elWantJson != haveJson)
            ETH_ERROR_MESSAGE("verify: filled test generated field mismatch,\n " +
                              _debug + "\n" +
                              " want: " + elWantJson + "\n  vs \n have: " + haveJson + "\n"
                );
    }
}

void modifyTransactionChainIDByNetwork(test::Transaction const& _tr, FORK const& _fork)
{
    auto const& genesisChainID = Options::getDynamicOptions().getCurrentConfig().getGenesisTemplateChainID();
    if (genesisChainID.count(_fork))
    {
        VALUE const& chainID = genesisChainID.at(_fork);
        if (_tr.getChainID() != chainID)
        {
            // This is gona be very cpu heavy because we need to recalculate the signature
            auto& tr = const_cast<test::Transaction&>(_tr);
            tr.setChainID(chainID);
        }
    }
}

}  // namespace
