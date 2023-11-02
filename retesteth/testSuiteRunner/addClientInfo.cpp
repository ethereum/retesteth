#include "TestSuiteHelperFunctions.h"
#include <libdevcore/CommonIO.h>
#include <retesteth/Options.h>
#include <retesteth/helpers/TestHelper.h>
#include <retesteth/helpers/TestOutputHelper.h>
#include <retesteth/session/Session.h>

using namespace std;
using namespace dev;
using namespace dataobject;
using namespace test::teststruct;
namespace fs = boost::filesystem;

namespace
{
string calculateHashOfTheNewFilledTest(DataObject& _newFilledTestRef)
{
    // Check up the generated test hash without _info section
    _newFilledTestRef.removeKey("_info");
    _newFilledTestRef.performModifier(mod_sortKeys);
    string const newFilledTestHashStr = _newFilledTestRef.asJson(0, false);
    return dev::toString(sha3(newFilledTestHashStr));
}

bool checkIfThereAreUpdatesToTheTest(spDataObject _oldFilledTestFile, spDataObject _newFilledTest,
    string const& _newFilledTestHash, string const& _newFilledTestSrcHash)
{
    // See if we actually changed something in the test after regeneration
    if (_oldFilledTestFile->count(_newFilledTest->getKey()))
    {
        DataObject const& existingTest = _oldFilledTestFile->atKey(_newFilledTest->getKey());
        if (!existingTest.atKey("_info").count("generatedTestHash"))
            return true;
        else
        {
            string const& oldTestHash = existingTest.atKey("_info").atKey("generatedTestHash").asString();
            if (oldTestHash != _newFilledTestHash)
                return true;

            string const& oldSrcHash = existingTest.atKey("_info").atKey("sourceHash").asString();
            if (oldSrcHash != _newFilledTestSrcHash)
                return true;
        }
    }
    else
        return true;
    return false;
}

}  // namespace


namespace test::testsuite
{
bool addClientInfoIfUpdate(DataObject& _newFilledTestData, fs::path const& _testSourcePath, dev::h256 const& _testSourceHash,
    fs::path const& _existingFilledTest)
{
    bool atLeastOneUpdate = false || Options::get().forceupdate;

    spDataObject oldFilledTestFile;
    if (Options::get().filltests && fs::exists(_existingFilledTest) && !Options::get().forceupdate)
        oldFilledTestFile = test::readJsonData(_existingFilledTest);

    session::SessionInterface& session = session::RPCSession::instance(TestOutputHelper::getThreadID());
    for (spDataObject& newFilledTest : _newFilledTestData.getSubObjectsUnsafe())
    {
        spDataObject newTestClientInfo;

        // Since one gtest parsed into many bctests we need a copy
        DataObject& newFilledTestRef = newFilledTest.getContent();
        if (newFilledTestRef.count("_info"))
            newTestClientInfo.getContent().copyFrom(newFilledTestRef.atKey("_info"));

        (*newTestClientInfo)["generatedTestHash"] = calculateHashOfTheNewFilledTest(newFilledTestRef);
        (*newTestClientInfo)["sourceHash"] = toString(_testSourceHash);

        string const& newHash = newTestClientInfo->atKey("generatedTestHash").asString();
        string const& newSrcHash = newTestClientInfo->atKey("sourceHash").asString();
        atLeastOneUpdate =
            atLeastOneUpdate || checkIfThereAreUpdatesToTheTest(oldFilledTestFile, newFilledTest, newHash, newSrcHash);

        if (!newTestClientInfo->count("comment"))
            (*newTestClientInfo)["comment"] = "";

        (*newTestClientInfo)["filling-rpc-server"] = session.web3_clientVersion()->asString();
        (*newTestClientInfo)["filling-tool-version"] = test::prepareVersionString();
        (*newTestClientInfo)["lllcversion"] = test::prepareLLLCVersionString();
        (*newTestClientInfo)["solidity"] = test::prepareSolidityVersionString();

        (*newTestClientInfo)["source"] = _testSourcePath.string();
        if (newTestClientInfo->count("labels"))
            (*newTestClientInfo).setKeyPos("labels", newTestClientInfo->getSubObjects().size() - 1);
        (*newTestClientInfo).performModifier(mod_sortKeys);

        newFilledTestRef.atKeyPointer("_info") = newTestClientInfo;
        newFilledTestRef.setKeyPos("_info", 0);
    }
    return atLeastOneUpdate;
}

}  // namespace testsuite
