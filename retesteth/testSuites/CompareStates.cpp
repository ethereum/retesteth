#include "Common.h"
#include <dataObject/DataObject.h>
#include <retesteth/Options.h>
#include <retesteth/RPCSession.h>
using namespace std;
namespace test
{
// inline function, because _actualExistance could be asked via remote RPC request
CompareResult checkExistance(scheme_expectAccount const& _expectAccount, bool _actualExistance)
{
    // if should not exist but actually exists
    if (_expectAccount.shouldNotExist() && _actualExistance)
    {
        ETH_MARK_ERROR(TestOutputHelper::get().testName() + "' Compare States: " +
                       _expectAccount.address() + "' address not expected to exist!");
        return CompareResult::AccountShouldNotExist;
    }
    // if expected to exist but actually not exists
    if (!_expectAccount.shouldNotExist() && !_actualExistance)
    {
        ETH_MARK_ERROR(TestOutputHelper::get().testName() +
                       " Compare States: Missing expected address: '" + _expectAccount.address() +
                       "'");
        return CompareResult::MissingExpectedAccount;
    }
    return CompareResult::Success;
}

// inline function
CompareResult compareAccounts(
    scheme_account const& _inState, scheme_expectAccount const& _expectAccount)
{
    // report all errors, but return the last error as a compare result
    CompareResult result = CompareResult::Success;
    auto checkMessage = [&result](bool _flag, CompareResult _type, string const& _error) -> void {
        ETH_MARK_ERROR_FLAG(_flag, _error);
        if (!_flag)
            result = _type;
    };

    if (_expectAccount.hasBalance())
    {
        u256 inStateB = u256(_inState.getData().atKey("balance").asString());
        checkMessage(_expectAccount.getData().atKey("balance").asString() ==
                         _inState.getData().atKey("balance").asString(),
            CompareResult::IncorrectBalance,
            "Check State: Remote account '" +
                _expectAccount.address() + "': has incorrect balance " + toString(inStateB) +
                ", test expected " +
                toString(u256(_expectAccount.getData().atKey("balance").asString())) + " (" +
                _expectAccount.getData().atKey("balance").asString() +
                " != " + _inState.getData().atKey("balance").asString() + ")");
    }

    if (_expectAccount.hasNonce())
        checkMessage(_expectAccount.getData().atKey("nonce").asString() ==
                         _inState.getData().atKey("nonce").asString(),
            CompareResult::IncorrectNonce,
            "Check State: Remote account '" +
                _expectAccount.address() + "': has incorrect nonce " +
                _inState.getData().atKey("nonce").asString() + ", test expected " +
                _expectAccount.getData().atKey("nonce").asString());

    // Check that state post has values from expected storage
    if (_expectAccount.hasStorage())
    {
        CompareResult res = _expectAccount.compareStorage(_inState.getData().atKey("storage"));
        if (result == CompareResult::Success)
            result = res;  // Only override success result with potential error
    }

    if (_expectAccount.hasCode())
        checkMessage(_expectAccount.getData().atKey("code").asString() ==
                         _inState.getData().atKey("code").asString(),
            CompareResult::IncorrectCode,
            "Check State: Remote account '" +
                _expectAccount.address() + "': has incorrect code '" +
                _inState.getData().atKey("code").asString() + "', test expected '" +
                _expectAccount.getData().atKey("code").asString() + "'");

    return result;
}

DataObject getRemoteAccountList(RPCSession& _session, scheme_block const& _latestInfo)
{
    DataObject accountList;
    string startHash = "0";
    const int cmaxRows = 100;
    const size_t cycles_max = 100;
    size_t cycles = 0;
    while (cycles++ <= cycles_max)
    {
        DataObject res = _session.debug_accountRange(
            _latestInfo.getNumber(), _latestInfo.getTransactionCount(), startHash, cmaxRows);
        auto const& subObjects = res.atKey("addressMap").getSubObjects();
        for (auto const& element : subObjects)
            accountList.addSubObject(element.asString(), DataObject(DataType::Null));
        if (res.atKey("nextKey").asString() ==
            "0x0000000000000000000000000000000000000000000000000000000000000000")
            break;
        if (subObjects.size() > 0)
            startHash = subObjects.at(subObjects.size() - 1).getKey();
    }
    ETH_ERROR_REQUIRE_MESSAGE(cycles <= cycles_max,
        "Remote state has too many accounts! (>" + to_string(cycles_max * cmaxRows) + ")");
    return accountList;
}

// compare states with full post information
// CompareResult compareStates(scheme_expectState const& _stateExpect, scheme_state const&
// _statePost);  CompareResult compareStates(scheme_state const& _stateExpect, scheme_state const&
// _statePost)
//{
//    return compareStates(scheme_expectState(_stateExpect.getData()), _statePost);
//}

// compare states with session asking post state data on the fly
void compareStates(
    scheme_expectState const& _stateExpect, RPCSession& _session, scheme_block const& _latestInfo)
{
    CompareResult result = CompareResult::Success;
    DataObject accountList = getRemoteAccountList(_session, _latestInfo);
    for (auto const& a : _stateExpect.getAccounts())
    {
        bool hasAccount = accountList.count(a.address());
        CompareResult existanceResult = checkExistance(a, hasAccount);
        if (existanceResult != CompareResult::Success)
        {
            result = existanceResult;
            continue;
        }
        if (!hasAccount)
            continue;

        // Compare account in postState with expect section account
        size_t totalSize = 0;
        CompareResult accountCompareResult =
            compareAccounts(remoteGetAccount(_session, a.address(), _latestInfo, totalSize), a);
        if (accountCompareResult != CompareResult::Success)
            result = accountCompareResult;
    }
    if (result != CompareResult::Success)
        ETH_ERROR_MESSAGE("CompareStates failed with errors: " + CompareResultToString(result));
}

void compareStates(scheme_expectState const& _stateExpect, scheme_state const& _statePost)
{
    CompareResult result = CompareResult::Success;
    for (auto const& a : _stateExpect.getAccounts())
    {
        bool hasAccount = _statePost.hasAccount(a.address());
        CompareResult existanceResult = checkExistance(a, hasAccount);
        if (existanceResult != CompareResult::Success)
        {
            result = existanceResult;
            continue;
        }
        if (!hasAccount)
            continue;

        // Compare account in postState with expect section account
        CompareResult accountCompareResult = compareAccounts(_statePost.getAccount(a.address()), a);
        if (accountCompareResult != CompareResult::Success)
            result = accountCompareResult;
    }
    if (result != CompareResult::Success)
        ETH_ERROR_MESSAGE("CompareStates failed with errors: " + CompareResultToString(result));
}

string CompareResultToString(CompareResult res)
{
    switch (res)
    {
    case CompareResult::Success:
        return "Success";
    case CompareResult::AccountShouldNotExist:
        return "AccountShouldNotExist";
    case CompareResult::MissingExpectedAccount:
        return "MissingExpectedAccount";
    case CompareResult::IncorrectBalance:
        return "IncorrectBalance";
    case CompareResult::IncorrectNonce:
        return "IncorrectNonce";
    case CompareResult::IncorrectCode:
        return "IncorrectCode";
    case CompareResult::IncorrectStorage:
        return "IncorrectStorage";
    default:
        return "Unparsed Compare Result";
    }
}
}
