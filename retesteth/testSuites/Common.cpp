#include "Common.h"
#include <dataObject/DataObject.h>
#include <retesteth/Options.h>
#include <retesteth/session/Session.h>
using namespace std;
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

void printVmTrace(SessionInterface& _session, FH32 const& _trHash, FH32 const& _stateRoot)
{
    DebugVMTrace ret(_session.debug_traceTransaction(_trHash));

    ETH_STDOUT_MESSAGE("------------------------");
    if (Options::get().vmtraceraw)
        ret.print();
    else
        ret.printNice();

    DataObject state;
    state["stateRoot"] = _stateRoot.asString();
    ETH_LOG(state.asJson(0, false), 0);
    ETH_STDOUT_MESSAGE("\n------------------------");
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
        ETH_ERROR_MESSAGE("Client didn't reject transaction: (" + trHash.asString() + ") \n" + _tr->getRawBytes().asString());
    if (_testException.empty() && !remoteException.empty())
        ETH_ERROR_MESSAGE("Client reject transaction expected to be valid: (" + trHash.asString() + ") \n" + _tr->getRawBytes().asString());

    if (!_testException.empty() && !remoteException.empty())
    {
        string const& expectedReason = Options::getCurrentConfig().translateException(_testException);
        if (remoteException.find(expectedReason) == string::npos)
        {
            ETH_WARNING(_tr->asDataObject()->asJson());
            ETH_ERROR_MESSAGE(string("Transaction rejecetd but due to a different reason: \n") +
               "Expected reason: `" + expectedReason + "` (" + _testException + ")\n" +
               "Client reason: `" + remoteException
              );
        }
    }
}

}  // namespace
