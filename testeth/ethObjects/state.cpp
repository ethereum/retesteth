#include <testeth/ethObjects/state.h>
#include <testeth/TestOutputHelper.h>
using namespace  std;

namespace test {
bool compareStates(expectState const& _stateExpect, state const& _statePost)
{
    bool wasError = false;
    for (auto const& a: _stateExpect.getAccounts())
    {
        if (a.shouldNotExist())
            BOOST_CHECK_MESSAGE(!_statePost.hasAccount(a.address()),
                 TestOutputHelper::get().testName() +  "' Compare States: " << a.address()
                 << "' address not expected to exist!");
        else
            BOOST_CHECK_MESSAGE(_statePost.hasAccount(a.address()),
                 TestOutputHelper::get().testName() +  " Compare States: '" << a.address()
                 << "' missing expected address!");

        // Check account in state post with expect sectino account
        account const& inState = _statePost.getAccount(a.address());

        if (a.hasBalance())
            BOOST_CHECK_MESSAGE(a.getData().at("balance").asString() == inState.getData().at("balance").asString(),
                 TestOutputHelper::get().testName() + " Check State: '" << a.address()
                 <<  "': incorrect balance " << inState.getData().at("balance").asString() << ", expected "
                 << a.getData().at("balance").asString());

        if (a.hasNonce())
            BOOST_CHECK_MESSAGE(a.getData().at("nonce").asString() == inState.getData().at("nonce").asString(),
                TestOutputHelper::get().testName() + " Check State: '" << a.address()
                << "': incorrect nonce " << inState.getData().at("nonce").asString() << ", expected "
                << a.getData().at("nonce").asString());

        // Check that state post has values from expected storage
        if (a.hasStorage())
            a.compareStorage(inState.getData().at("storage"));

        if (a.hasCode())
            BOOST_CHECK_MESSAGE(a.getData().at("code").asString() == inState.getData().at("code").asString(),
                TestOutputHelper::get().testName() + " Check State: '" << a.address()
                << "': incorrect code '" << inState.getData().at("code").asString() << "', expected '"
                << a.getData().at("code").asString() << "'");
    }
    return wasError;
}
} // namespace
