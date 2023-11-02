#include "EthChecks.h"
#include "Options.h"
#include <retesteth/helpers/TestHelper.h>
#include "TestSuiteHelperFunctions.h"
//#include <libdevcore/CommonIO.h>

using namespace std;
using namespace dev;
using namespace test;
using namespace test::debug;
namespace fs = boost::filesystem;

namespace
{
void checkTestVersion(DataObject const& _info, fs::path const& _compiledTest)
{
    ETH_ERROR_REQUIRE_MESSAGE(_info.count("filling-tool-version"),
        string() + "Field `filling-tool-version` not found in generated test!" + _compiledTest.c_str());
    string strippedVersion;
    string const version = _info.atKey("filling-tool-version").asString().substr(9, 10);
    for (auto const& ch : version)
    {
        if (isdigit(ch))
            strippedVersion += ch;
    }
    if (strippedVersion.at(0) == '0')
        strippedVersion.erase(0, 1);
    int testVersion = atoi(strippedVersion.c_str());
    if (testVersion > retestethVersion())
        ETH_WARNING("Test was filled with newer version of retesteth! "
            "(test: `" + strippedVersion + "` vs retesteth: `" + test::fto_string(retestethVersion()) + "`)"
            " This might cause failures, please update retesteth!\n" + _compiledTest.string());
    else if (testVersion < retestethVersion() - 1)
        ETH_WARNING("Test filled with older version of retesteth! "
            "(test: `" + strippedVersion + "` vs retesteth: `" + test::fto_string(retestethVersion()) + "`) "
            "This might cause failures, please regenerate the test!\n" + _compiledTest.string());
}
}  // namespace

namespace test::testsuite
{
bool checkFillerHash(fs::path const& _compiledTest, fs::path const& _sourceTest)
{
    bool isTestOutdated = false;
    ETH_DC_MESSAGE(DC::TESTLOG, string("Check `") + _compiledTest.c_str() + "` hash");
    ETH_DC_MESSAGE(DC::TESTLOG, string("SrcFile `") + _sourceTest.c_str() + "`");
    TestFileData fillerData = readFillerTestFile(_sourceTest);

    // If no hash calculated, skip the hash check
    if (!fillerData.hashCalculated)
        return isTestOutdated;

    CJOptions opt { .stopper = "_info" };
    spDataObject compiledTestFileData = test::readJsonData(_compiledTest, opt);
    for (auto const& test : compiledTestFileData->getSubObjects())
    {
        DataObject const& testRef = test.getCContent();
        try
        {
            ETH_ERROR_REQUIRE_MESSAGE(
                testRef.type() == DataType::Object, testRef.getKey() + " should contain an object under a test name.");
            ETH_ERROR_REQUIRE_MESSAGE(testRef.count("_info") > 0, "_info section not set! " + _compiledTest.string());
            DataObject const& info = testRef.atKey("_info");
            ETH_ERROR_REQUIRE_MESSAGE(
                info.count("sourceHash") > 0, "sourceHash not found in " + _compiledTest.string() + " in " + testRef.getKey());

            // Check test version  vs retesteth version
            if (!Options::isLegacy())
                checkTestVersion(info, _compiledTest);

            // Check source hash vs filled test hash
            h256 const sourceHash = h256(info.atKey("sourceHash").asString());
            if (sourceHash != fillerData.hash)
            {
                isTestOutdated = true;
                if (Options::get().filloutdated)
                    continue;
                string sourceHashStr;
                string fillerHashStr;
                if (Options::get().showhash)
                {
                    sourceHashStr = sourceHash.hex();
                    fillerHashStr = fillerData.hash.hex();

                    // Correct the generated hash
                    //auto file = dev::contentsString(_compiledTest.string());
                    //size_t pos = file.find(sourceHashStr);
                    //if (pos != string::npos)
                    //{
                    //    file = file.replace(pos, sourceHashStr.size(), fillerHashStr);
                    //    dev::writeFile(_compiledTest.string(), file);
                    //}
                }
                else
                {
                    sourceHashStr = sourceHash.hex().substr(0, 4);
                    fillerHashStr = fillerData.hash.hex().substr(0, 4);
                }
                ETH_MARK_ERROR("Test " + _compiledTest.string() + " is outdated. Filler hash is different! " + "('" +
                                  sourceHashStr + "' != '" + fillerHashStr + "') ");
            }
        }
        catch (test::UpwardsException const&)
        {
            continue;
        }
    }
    return isTestOutdated;
}

}  // namespace testsuite
