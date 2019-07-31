#include "Common.h"
#include <dataObject/ConvertJsoncpp.h>
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
            TestOutputHelper::get().testName() + " Check State: '" + _expectAccount.address() +
                "': incorrect balance " + toString(inStateB) + ", expected " +
                toString(u256(_expectAccount.getData().atKey("balance").asString())) + " (" +
                _expectAccount.getData().atKey("balance").asString() +
                " != " + _inState.getData().atKey("balance").asString() + ")");
    }

    if (_expectAccount.hasNonce())
        checkMessage(_expectAccount.getData().atKey("nonce").asString() ==
                         _inState.getData().atKey("nonce").asString(),
            CompareResult::IncorrectNonce,
            TestOutputHelper::get().testName() + " Check State: '" + _expectAccount.address() +
                "': incorrect nonce " + _inState.getData().atKey("nonce").asString() +
                ", expected " + _expectAccount.getData().atKey("nonce").asString());

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
            TestOutputHelper::get().testName() + " Check State: '" + _expectAccount.address() +
                "': incorrect code '" + _inState.getData().atKey("code").asString() +
                "', expected '" + _expectAccount.getData().atKey("code").asString() + "'");

    return result;
}

// inline function
bool remoteHasAccount(
    RPCSession& _session, string const& _account, string const& _latestBlockNumber)
{
    test::scheme_block latestBlock = _session.eth_getBlockByNumber(_latestBlockNumber, false);
    int trIndex = latestBlock.getTransactionCount();

    string startHash = "0";
    const int cmaxRows = 100;
    const size_t cycles_max = 100;
    size_t cycles = cycles_max;
    while (--cycles)
    {
        DataObject res =
            _session.debug_accountRange(_latestBlockNumber, trIndex, startHash, cmaxRows);
        auto const& subObjects = res.atKey("addressMap").getSubObjects();
        for (auto const& element : subObjects)
        {
            if (element.asString() == _account)
                return true;
        }
        if (res.atKey("nextKey").asString() ==
            "0x0000000000000000000000000000000000000000000000000000000000000000")
            break;
        if (subObjects.size() > 0)
            startHash = subObjects.at(subObjects.size() - 1).getKey();
    }
    ETH_ERROR_REQUIRE_MESSAGE(cycles > 0,
        "Remote state has too many accounts! (" + to_string(cycles_max * cmaxRows) + ")");
    return false;
}

// compare states with full post information
CompareResult compareStates(scheme_expectState const& _stateExpect, scheme_state const& _statePost);
CompareResult compareStates(scheme_state const& _stateExpect, scheme_state const& _statePost)
{
    return compareStates(scheme_expectState(_stateExpect.getData()), _statePost);
}

// compare states with session asking post state data on the fly
CompareResult compareStates(scheme_expectState const& _stateExpect, RPCSession& _session)
{
    CompareResult result = CompareResult::Success;
    string latestBlockNumber = _session.eth_blockNumber();
    for (auto const& a : _stateExpect.getAccounts())
    {
        bool hasAccount = remoteHasAccount(_session, a.address(), latestBlockNumber);
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
        CompareResult accountCompareResult = compareAccounts(
            remoteGetAccount(_session, a.address(), latestBlockNumber, totalSize), a);
        if (accountCompareResult != CompareResult::Success)
            result = accountCompareResult;
    }
    return result;
}

CompareResult compareStates(scheme_expectState const& _stateExpect, scheme_state const& _statePost)
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
    return result;
}
}
