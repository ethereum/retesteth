#include "Common.h"
#include <dataObject/ConvertJsoncpp.h>
#include <dataObject/DataObject.h>
#include <retesteth/Options.h>
#include <retesteth/RPCSession.h>
using namespace std;
namespace test
{
// Validate post condition on remote client
void checkExpectSection(
    RPCSession& _session, ExpectInfo const& _expectedInfo, scheme_remoteState& _remoteState)
{
    // expect section provided, perform validation check on remote client
    if (!_expectedInfo.expectState.isNull())
    {
        // check that the post state qualifies to the expect section
        _remoteState = getRemoteState(_session, StateRequest::AttemptFullPost);
        if (Options::get().logVerbosity >= 6)
        {
            test::scheme_block blockData(_remoteState.getData().atKey("rawBlockData"));
            if (blockData.getTransactionCount() != 1)
                ETH_TEST_MESSAGE("Warning: Looks like remote block do not contain a transaction! " +
                                 TestOutputHelper::get().testInfo());
        }

        CompareResult res = CompareResult::Success;
        if (_remoteState.hasPostState())
        {
            // a small state loaded into postState object. compare expect section to this object
            scheme_state postState(_remoteState.getPostState());
            res = test::compareStates(_expectedInfo.expectState, postState);
        }
        else
        {
            // perform check on a huge state,
            // do requests to the client and compare to expect section
            res = test::compareStates(_expectedInfo.expectState, _session);
        }

        if (res != CompareResult::Success)
            ETH_ERROR_MESSAGE("checkExpectSection on remote client mismatch!");
    }

    _remoteState = getRemoteState(_session, StateRequest::NoPost);

    // trHash provided. ask remote client for logHash for the provided trHash
    if (!_expectedInfo.trHash.empty() && !_expectedInfo.logHash.empty())
    {
        string remoteLogHash = _session.test_getLogHash(_expectedInfo.trHash);
        if (!remoteLogHash.empty() && remoteLogHash != _expectedInfo.logHash)
        {
            ETH_ERROR_MESSAGE("Error at " + TestOutputHelper::get().testInfo() +
                              ", logs hash mismatch: '" + remoteLogHash + "'" + ", expected: '" +
                              _expectedInfo.logHash + "'");
        }
    }

    // postHash provided, compare to remoteState postHash
    if (!_expectedInfo.postHash.empty() && _remoteState.getPostHash() != _expectedInfo.postHash)
    {
        ETH_ERROR_MESSAGE("Error at " + TestOutputHelper::get().testInfo() +
                          ", post hash mismatch: " + _remoteState.getPostHash() +
                          ", expected: " + _expectedInfo.postHash);
        if (Options::get().logVerbosity > 5)
        {
            _remoteState = getRemoteState(_session, StateRequest::AttemptFullPost);
            ETH_TEST_MESSAGE("\nState Dump: \n" + _remoteState.getPostState().asJson());
        }
    }
}

void checkDataObject(DataObject const& _input)
{
    ETH_ERROR_REQUIRE_MESSAGE(_input.type() == DataType::Object,
        TestOutputHelper::get().get().testFile().string() + " A test file must contain an object.");
}

void checkAtLeastOneTest(DataObject const& _input)
{
    ETH_ERROR_REQUIRE_MESSAGE(
        _input.getSubObjects().size() >= 1, " A test file must contain at least one test: " +
                                                TestOutputHelper::get().testFile().string());

    for (auto const& test : _input.getSubObjects())
    {
        ETH_ERROR_REQUIRE_MESSAGE(
            test.type() == DataType::Object, TestOutputHelper::get().testFile().string() +
                                                 " should contain an object under a test name.");
    }
}

void checkOnlyOneTest(DataObject const& _input)
{
    ETH_ERROR_REQUIRE_MESSAGE(_input.getSubObjects().size() == 1,
        " A test file must contain only one test: " + TestOutputHelper::get().testFile().string());

    ETH_ERROR_REQUIRE_MESSAGE(_input.getSubObjects().at(0).type() == DataType::Object,
        TestOutputHelper::get().testFile().string() +
            " should contain an object under a test name.");
}

void checkTestNameIsEqualToFileName(DataObject const& _input)
{
    if (!TestOutputHelper::get().testFile().empty())
        ETH_ERROR_REQUIRE_MESSAGE(_input.getSubObjects().at(0).getKey() + "Filler" ==
                                      TestOutputHelper::get().testFile().stem().string(),
            TestOutputHelper::get().testFile().string() +
                " contains a test with a different name '" + _input.getSubObjects().at(0).getKey() +
                "'");
}

scheme_account remoteGetAccount(
    RPCSession& _session, string const& _account, LatestInfo const& _latestInfo, size_t& _totalSize)
{
    DataObject accountObj;
    accountObj.setKey(_account);
    accountObj["code"] = _session.eth_getCode(_account, _latestInfo.latestBlockNumber);
    _totalSize += accountObj["code"].asString().size();
    accountObj["nonce"] =
        to_string(_session.eth_getTransactionCount(_account, _latestInfo.latestBlockNumber));
    accountObj["balance"] = _session.eth_getBalance(_account, _latestInfo.latestBlockNumber);

    // Storage
    const size_t cycles_max = 100;
    const int cmaxRows = 100;
    DataObject storage(DataType::Object);
    string beginHash = "0";
    size_t cycles = cycles_max;
    while (--cycles)
    {
        DataObject debugStorageAt = _session.debug_storageRangeAt(_latestInfo.latestBlockNumber,
            _latestInfo.latestTrIndex, _account, beginHash, cmaxRows);
        auto const& subObjects = debugStorageAt["storage"].getSubObjects();
        _totalSize += subObjects.size() * 64;
        for (auto const& element : subObjects)
            storage[element.atKey("key").asString()] = element.atKey("value").asString();
        if (debugStorageAt.atKey("complete").asBool())
            break;
        if (subObjects.size() > 0)
            beginHash = subObjects.at(subObjects.size() - 1).getKey();
    }
    accountObj["storage"] = storage;
    ETH_ERROR_REQUIRE_MESSAGE(cycles > 0,
        "Remote state has too many storage records! (" + to_string(cycles_max * cmaxRows) + ")");
    return scheme_account(accountObj);
}

scheme_remoteState getRemoteState(RPCSession& _session, StateRequest _stateRequest)
{
    DataObject remoteState;
    test::scheme_block latestBlock = _session.eth_getBlockByNumber(
        _session.eth_blockNumber(), _stateRequest == StateRequest::AttemptFullPost ? true : false);
    remoteState["postHash"] = latestBlock.getData().atKey("stateRoot");
    remoteState["rawBlockData"] = latestBlock.getData();

    LatestInfo latestInfo;
    latestInfo.latestBlockNumber = latestBlock.getNumber();
    latestInfo.latestTrIndex = latestBlock.getTransactionCount();

    if (_stateRequest == StateRequest::AttemptFullPost)
    {
        DataObject accountsObj;
        DataObject accountList = getRemoteAccountList(_session, latestInfo);

        bool isHugeState = false;
        if (accountList.getSubObjects().size() < 20 || Options::get().fullstate)
        {
            size_t stateTotalSize = 0;
            for (auto acc : accountList.getSubObjects())
            {
                scheme_account accountScheme =
                    remoteGetAccount(_session, acc.getKey(), latestInfo, stateTotalSize);
                if (stateTotalSize > 1024000 && !Options::get().fullstate)  // > 1MB
                {
                    isHugeState = true;
                    break;
                }
                accountsObj.addSubObject(accountScheme.getData());
            }
        }
        else
            isHugeState = true;

        if (isHugeState)
        {
            accountsObj.clear();
            accountsObj = remoteState["postHash"];
        }
        remoteState["postState"] = accountsObj;

        if (Options::get().poststate)
            std::cout << accountsObj.asJson() << std::endl;
    }
    return scheme_remoteState(remoteState);
}
}
