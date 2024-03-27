#include "Common.h"
#include <retesteth/Options.h>
#include <retesteth/helpers/TestHelper.h>
#include <retesteth/helpers/TestOutputHelper.h>
#include <retesteth/Constants.h>
using namespace std;
using namespace test::debug;
using namespace test::teststruct::constnames;
namespace fs = boost::filesystem;

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
        const string tFileName = TestOutputHelper::get().testFile().stem().string();
        ETH_ERROR_REQUIRE_MESSAGE(_testName + "Filler" == tFileName,
            TestOutputHelper::get().testFile().string() +
                " contains a test with a different name '" + _testName + "'");
    }
}

void checkTestNameIsEqualToFileName(DataObject const& _input)
{
    if (!TestOutputHelper::get().testFile().empty())
    {
        auto const& testfile = TestOutputHelper::get().testFile();
        auto const& key = _input.getSubObjects().at(0)->getKey();
        ETH_ERROR_REQUIRE_MESSAGE(
             key + "Filler" == testfile.stem().string(),
            testfile.string() + " contains a test with a different name '" + key + "'");
    }
}

void printVmTrace(VMtraceinfo const& _info)
{
    const DebugVMTrace ret(_info.session.debug_traceTransaction(_info.trHash));

    ETH_DC_MESSAGE(DC::TESTLOG, "------------------------");
    auto const& vmtraceraw = Options::get().vmtraceraw;
    if (vmtraceraw)
    {
        if (!vmtraceraw.outpath.empty())
        {
            auto const outpath = fs::path(vmtraceraw.outpath);
            ETH_DC_MESSAGEC(DC::TESTLOG, "Export vmtraceraw to " + (outpath / _info.trName).string(), LogColor::LIME);
            ret.exportLogs(outpath / _info.trName);
        }
        else
            ret.print();
    }
    else
        ret.print();

    DataObject state;
    state["stateRoot"] = _info.stateRoot.asString();
    ETH_DC_MESSAGE(DC::TESTLOG, state.asJson(0, false));
    ETH_DC_MESSAGE(DC::TESTLOG, "\n------------------------");
}

void compareTransactionException(spTransaction const& _tr, MineBlocksResult const& _mRes, string const& _testException)
{
    if (!_mRes.isRejectData() && !_testException.empty())
    {
        if (Options::get().filltests)
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
        if (remoteException.find(expectedReason) == string::npos || expectedReason.empty())
        {
            string const error = "Transaction rejected but due to a different reason: \n";
            ETH_WARNING(error + _tr->asDataObject()->asJson());
            ETH_ERROR_MESSAGE(error +
               "Expected reason: `" + expectedReason + "` (" + _testException + ")\n" +
               "Client reason: `" + remoteException
              );
        }
    }
}

void compareEOFException(BYTES const& _code, std::string const& _mRes, std::string const& _testException)
{
    bool const hasOk = (_mRes.find("ok.") != string::npos || _mRes.find("OK") != string::npos);
    string const remoteException = hasOk ? "" : _mRes;
    if (!_testException.empty() && remoteException.empty())
        ETH_ERROR_MESSAGE("Client didn't reject EOF code: (" + _code.asString() + ")" +
                          "\nTest Expected: " + _testException);
    if (_testException.empty() && !remoteException.empty())
        ETH_ERROR_MESSAGE("Client reject EOF code expected to be valid: (" + _code.asString() + ")" +
                          "\nReason: " + remoteException);

    if (!_testException.empty() && !remoteException.empty())
    {
        string const& expectedReason = Options::getCurrentConfig().translateException(_testException);
        if (remoteException.find(expectedReason) == string::npos || expectedReason.empty())
        {
            ETH_WARNING(_code.asString());
            ETH_ERROR_MESSAGE(string("EOF code rejected but due to a different reason: \n") +
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
            ETH_WARNING("retesteth::verify does not have fork: " + _net.asString() + TestOutputHelper::get().testInfo().errorDebug());
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
            _debug += "[";
            _debug += test::fto_string(k);
            _debug += "] -> ";
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
    if (Options::get().chainid.initialized())
    {
        VALUE const chainID((size_t)Options::get().chainid);
        if (_tr.getChainID() != chainID)
        {
            auto& tr = const_cast<test::Transaction&>(_tr);
            tr.setChainID(chainID);
        }
        return;
    }

    auto const& genesisChainID = Options::getCurrentConfig().getGenesisTemplateChainID();
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

spDataObject storageDiff(Storage const& _pre, Storage const& _post)
{
    spDataObject res;
    for (auto const& _postKey : _post.getKeys())
    {
        auto const& postKey = std::get<0>(_postKey.second);
        auto const& postValue = std::get<1>(_postKey.second);
        if (_pre.hasKey(postKey))
        {
            // old key changed
            auto const& preAtPostKey = _pre.atKey(postKey);
            if (preAtPostKey != postValue)
            {
                auto const msg = preAtPostKey.asString() + " -> " + postValue->asString() + " (" +
                                 preAtPostKey.asDecString() + " -> " + postValue->asDecString() + ")";
                (*res)[postKey->asString()] = msg;
            }
        }
        else
        {
            // new key appeared
            auto const msg = "0x -> " + postValue->asString() + " (" + "0x -> " + postValue->asDecString() + ")";
            (*res)[postKey->asString()] = msg;
        }
    }
    for (auto const& _preKey : _pre.getKeys())
    {
        auto const& preKey = std::get<0>(_preKey.second);
        if (!_post.hasKey(preKey))
        {
            // old key removed
            (*res)["DELETED: " + preKey->asString()] = std::get<1>(_preKey.second)->asString();
        }
    }
    return res;
}

spDataObject stateDiff(State const& _pre, State const& _post)
{
    spDataObject res(new DataObject(DataType::Object));
    for (auto const& postAcc : _post.accounts())
    {
        if (_pre.hasAccount(postAcc.first))
        {
            // check for updates
            auto const& accPre = _pre.getAccount(postAcc.first);
            auto const& accPost = postAcc.second;

            auto const& preBalance = accPre.balance();
            auto const& postBalance = accPost->balance();
            if (preBalance != postBalance)
            {
                auto const msg = preBalance.asString() + " -> " + postBalance.asString() + " (" +
                                 preBalance.asDecString() + " -> " + postBalance.asDecString() + ")";
                (*res)[postAcc.first.asString()][c_balance] = msg;
            }

            auto const& preNonce = accPre.nonce();
            auto const& postNonce = accPost->nonce();
            if (preNonce != postNonce)
            {
                auto const msg = preNonce.asString() + " -> " + postNonce.asString() + " (" +
                                 preNonce.asDecString() + " -> " + postNonce.asDecString() + ")";
                (*res)[postAcc.first.asString()][c_nonce] = msg;
            }

            auto const& preCode = accPre.code();
            auto const& postCode = accPost->code();
            if (preCode != postCode)
                (*res)[postAcc.first.asString()][c_code] = preCode.asString() + " -> " + postCode.asString();

            auto const storageDiffRes = storageDiff(accPre.storage(), accPost->storage());
            if (storageDiffRes->getSubObjects().size())
                (*res)[postAcc.first.asString()].atKeyPointer(c_storage) = storageDiffRes;
        }
        else
        {
            // this is new account
            string const key = "NEW: " + postAcc.first.asString();
            (*res).atKeyPointer(key) = postAcc.second->asDataObject()->copy();

            // Print dec values
            VALUE balance((*res).atKey(key).atKey(c_balance));
            (*res).atKeyUnsafe(key)[c_balance] = balance.asString() + " (" + balance.asDecString() + ")";
            VALUE nonce((*res).atKey(key).atKey(c_nonce));
            (*res).atKeyUnsafe(key)[c_nonce] = nonce.asString() + " (" + nonce.asDecString() + ")";
            for (auto& el : (*res).atKeyUnsafe(key).atKeyUnsafe(c_storage).getSubObjectsUnsafe())
            {
                VALUE val(el->asString());
                el.getContent().setString(val.asString() + " (" + val.asDecString() + ")");
            }
        }
    }
    for (auto const& preAcc : _pre.accounts())
    {
        if (!_post.hasAccount(preAcc.first))
        {
            // this is deleted account
            spDataObject deleted(new DataObject(string("DELETED: ") + preAcc.first.asString()));
            (*res).addSubObject(deleted);
        }
    }
    return res;
}

bool hasSkipFork(std::set<FORK> const& _allforks)
{
    Options const& opt = Options::get();
    auto const& skipforks = opt.getCurrentConfig().cfgFile().fillerSkipForks();
    for (auto const& skipfork : skipforks)
    {
        if (_allforks.count(skipfork))
        {
            ETH_WARNING(string("Test has unsupported fork `") + skipfork.asString() +
                        "` allowed to skip, skipping the test from filling!"
                        + TestOutputHelper::get().testInfo().errorDebug());
            return true;
        }
    }
    return false;
}

bool networkSkip(FORK const& _net, string const& _testName)
{
    auto const& opt = Options::get();
    auto const& conf = opt.getCurrentConfig();
    bool const skipedFork = conf.checkForkSkipOnFiller(_net);
    bool const allowedFork = conf.checkForkAllowed(_net);
    bool singleNetDeny = (!opt.singleTestNet.empty() && opt.singleTestNet != _net.asString());
    if (opt.runOnlyNets.initialized())
        singleNetDeny = !Options::getDynamicOptions().runOnlyNetworks().count(_net);
    if (singleNetDeny || !allowedFork || skipedFork)
    {
        if ((!allowedFork || skipedFork) && !singleNetDeny)
            ETH_WARNING("Skipping unsupported fork: " + _net.asString() + " in " + _testName);
        return true;
    }
    return false;
}

}  // namespace
