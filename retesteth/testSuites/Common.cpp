#include "Common.h"
#include <dataObject/ConvertJsoncpp.h>
#include <dataObject/DataObject.h>
#include <retesteth/Options.h>
#include <retesteth/RPCSession.h>
using namespace std;
namespace test
{
void checkDataObject(DataObject const& _input)
{
    ETH_REQUIRE_MESSAGE(_input.type() == DataType::Object,
        TestOutputHelper::get().get().testFile().string() + " A test file must contain an object.");
}

void checkOnlyOneTest(DataObject const& _input)
{
    ETH_REQUIRE_MESSAGE(_input.getSubObjects().size() == 1,
        TestOutputHelper::get().testFile().string() + " A test file must contain only one test.");

    ETH_REQUIRE_MESSAGE(_input.getSubObjects().at(0).type() == DataType::Object,
        TestOutputHelper::get().testFile().string() +
            " should contain an object under a test name.");
}

void checkTestNameIsEqualToFileName(DataObject const& _input)
{
    if (!TestOutputHelper::get().testFile().empty())
        ETH_REQUIRE_MESSAGE(_input.getSubObjects().at(0).getKey() + "Filler" ==
                                TestOutputHelper::get().testFile().stem().string(),
            TestOutputHelper::get().testFile().string() +
                " contains a test with a different name '" + _input.getSubObjects().at(0).getKey() +
                "'");
}

DataObject getRemoteState(RPCSession& _session, string const& _trHash, bool _fullPost)
{
    DataObject remoteState;
    const int cmaxRows = 1000;
    string latestBlockNumber = toString(u256(_session.eth_blockNumber()));

    test::scheme_block latestBlock = _session.eth_getBlockByNumber(latestBlockNumber, true);
    remoteState["postHash"] = latestBlock.getData().at("stateRoot");
    if (!_trHash.empty())
        remoteState["logHash"] = _session.test_getLogHash(_trHash);
    remoteState["postState"] = "";
    remoteState["rawBlockData"] = latestBlock.getData();

    if (_fullPost)
    {
        int trIndex = latestBlock.getTransactionCount(); //1000;
        DataObject accountObj;
        Json::Value res = _session.debug_accountRangeAt(latestBlockNumber, trIndex, "0", cmaxRows);
        for (auto acc : res["addressMap"])
        {
            // Balance
            Json::Value ret = _session.eth_getBalance(acc.asString(), latestBlockNumber);
            accountObj[acc.asString()]["balance"] =
                dev::toCompactHexPrefixed(u256(ret.asString()), 1);  // fix odd strings

            // Code
            ret = _session.eth_getCode(acc.asString(), latestBlockNumber);
            accountObj[acc.asString()]["code"] = ret.asString();

            // Nonce
            ret = _session.eth_getTransactionCount(acc.asString(), latestBlockNumber);
            accountObj[acc.asString()]["nonce"] =
                dev::toCompactHexPrefixed(u256(ret.asString()), 1);

            // Storage
            DataObject storage(DataType::Object);
            DataObject debugStorageAt =
                dataobject::ConvertJsoncppToData(_session.debug_storageRangeAt(
                    latestBlockNumber, trIndex, acc.asString(), "0", cmaxRows));
            for (auto const& element : debugStorageAt["storage"].getSubObjects())
                storage[element.at("key").asString()] = element.at("value").asString();
            accountObj[acc.asString()]["storage"] = storage;
        }

        remoteState["postState"].clear();
        remoteState["postState"] = accountObj;
        if (Options::get().poststate)
            std::cout << accountObj.asJson() << std::endl;
    }
    return remoteState;
}
}
