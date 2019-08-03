#include "Common.h"
#include <dataObject/DataObject.h>
#include <retesteth/Options.h>
#include <retesteth/RPCSession.h>
using namespace std;
namespace test
{
void validatePostHash(
    RPCSession& _session, string const& _postHash, scheme_block const& _latestInfo)
{
    string actualHash = _latestInfo.getStateHash();
    if (actualHash != _postHash)
    {
        if (Options::get().logVerbosity > 5)
            ETH_LOG("\nState Dump: \n" + getRemoteState(_session, _latestInfo).getData().asJson(), 5);
        ETH_ERROR_MESSAGE("Error at " + TestOutputHelper::get().testInfo() +
                          ", post hash mismatch remote: " + actualHash + ", expected: " + _postHash);
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

scheme_account remoteGetAccount(RPCSession& _session, string const& _account,
    scheme_block const& _latestInfo, size_t& _totalSize)
{
    DataObject accountObj;
    accountObj.setKey(_account);
    accountObj["code"] = _session.eth_getCode(_account, _latestInfo.getNumber());
    _totalSize += accountObj["code"].asString().size();
    accountObj["nonce"] =
        to_string(_session.eth_getTransactionCount(_account, _latestInfo.getNumber()));
    accountObj["balance"] = _session.eth_getBalance(_account, _latestInfo.getNumber());

    // Storage
    const size_t cycles_max = 100;
    const int cmaxRows = 100;
    DataObject storage(DataType::Object);
    string beginHash = "0";
    size_t cycles = cycles_max;
    while (--cycles)
    {
        DataObject debugStorageAt = _session.debug_storageRangeAt(_latestInfo.getNumber(),
            _latestInfo.getTransactionCount(), _account, beginHash, cmaxRows);
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

scheme_state getRemoteState(RPCSession& _session, scheme_block const& _latestInfo)
{
    const int c_accountLimitBeforeHash = 20;
    DataObject accountsObj;

    // Probe for a huge state so not to call entire list of accounts if there are more then Limited
    bool isHugeState = false;
    DataObject accountList;
    if (!Options::get().fullstate)
    {
        DataObject res = _session.debug_accountRange(_latestInfo.getNumber(),
            _latestInfo.getTransactionCount(), "", c_accountLimitBeforeHash);
        if (res.atKey("nextKey").asString() !=
            "0x0000000000000000000000000000000000000000000000000000000000000000")
            isHugeState = true;
        else
        {
            // looks like the state is small
            for (auto const& element : res.atKey("addressMap").getSubObjects())
                accountList.addSubObject(element.asString(), DataObject(DataType::Null));
        }
    }
    else
        accountList =
            getRemoteAccountList(_session, _latestInfo);  // get the whole list of accounts

    if (!isHugeState || Options::get().fullstate)
    {
        size_t stateTotalSize = 0;
        for (auto acc : accountList.getSubObjects())
        {
            scheme_account accountScheme =
                remoteGetAccount(_session, acc.getKey(), _latestInfo, stateTotalSize);
            if (stateTotalSize > 1024000 && !Options::get().fullstate)  // > 1MB
            {
                isHugeState = true;
                break;
            }
            accountsObj.addSubObject(accountScheme.getData());
        }
    }

    if (isHugeState)
    {
        accountsObj.clear();
        accountsObj = _latestInfo.getStateHash();
    }

    if (Options::get().poststate)
        std::cout << accountsObj.asJson() << std::endl;

    return scheme_state(accountsObj);
}
}
