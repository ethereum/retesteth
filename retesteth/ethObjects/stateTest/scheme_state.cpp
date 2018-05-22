#include "scheme_state.h"
#include "scheme_postState.h"
#include "scheme_expectState.h"
#include <retesteth/TestOutputHelper.h>
using namespace  std;

namespace test {

CompareResult compareStates(scheme_expectState const& _stateExpect, scheme_state const& _statePost)
{
    CompareResult result = CompareResult::Success;
	auto checkMessage = [&result](bool _flag, CompareResult _type, string const& _error) -> void
    {
        ETH_CHECK_MESSAGE(_flag, _error);
        if (!_flag)
			result = _type;
    };

    for (auto const& a: _stateExpect.getAccounts())
    {
        if (a.shouldNotExist())
		{
            checkMessage(!_statePost.hasAccount(a.address()),
                CompareResult::AccountShouldNotExist,
                TestOutputHelper::get().testName() +  "' Compare States: " + a.address()
                + "' address not expected to exist!");
			if (result != CompareResult::Success)
				break;
			continue;
		}
        else
            checkMessage(_statePost.hasAccount(a.address()),
                CompareResult::MissingExpectedAccount,
                TestOutputHelper::get().testName() +  " Compare States: Missing expected address: '" + a.address() + "'");

		if (result != CompareResult::Success)
            break;

		// Check account in state post with expect section account
		scheme_account const& inState = _statePost.getAccount(a.address());

        if (a.hasBalance())
		{
          u256 inStateB = u256(inState.getData().at("balance").asString());
          checkMessage(
              a.getData().at("balance").asString() ==
                  inState.getData().at("balance").asString(),
              CompareResult::IncorrectBalance,
              TestOutputHelper::get().testName() + " Check State: '" +
                  a.address() + "': incorrect balance " + toString(inStateB) +
                  ", expected " +
                  toString(u256(a.getData().at("balance").asString())));

		}

        if (a.hasNonce())
            checkMessage(a.getData().at("nonce").asString() == inState.getData().at("nonce").asString(),
                CompareResult::IncorrectNonce,
                TestOutputHelper::get().testName() + " Check State: '" + a.address()
                + "': incorrect nonce " + inState.getData().at("nonce").asString() + ", expected "
                + a.getData().at("nonce").asString());

        // Check that state post has values from expected storage
        if (a.hasStorage()) {
          CompareResult res = a.compareStorage(inState.getData().at("storage"));
          if (result == CompareResult::Success)
            result = res; // Only override success result with potential error
        }

        if (a.hasCode())
            checkMessage(a.getData().at("code").asString() == inState.getData().at("code").asString(),
                CompareResult::IncorrectCode,
                TestOutputHelper::get().testName() + " Check State: '" + a.address()
                + "': incorrect code '" + inState.getData().at("code").asString() + "', expected '"
                + a.getData().at("code").asString() + "'");
    }
    return result;
}
} // namespace
