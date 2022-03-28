#include "TestSuiteHelperFunctions.h"
#include "Options.h"

namespace test
{
namespace testsuite
{
TestFileData readTestFile(fs::path const& _testFileName)
{
    // Legacy hash validation require to sort json data upon load, thats the old algo used to calculate hash
    // Avoid time consuming legacy tests hash validation if there is no --checkhash option
    bool isLegacy = Options::isLegacy();
    bool bSortOnLoad =
        isLegacy;  //(Options::get().checkhash && isLegacy); uncomment here if there would be too many legacy tests

    // Binary file hash calculation is impossible as git messes up with the files
    // So we read json structure and print it here to calculate the hash from string
    // Adds around 1% to execution time
    ETH_LOG("Read json structure " + string(_testFileName.filename().c_str()), 5);
    TestFileData testData;
    if (_testFileName.extension() == ".json")
        testData.data = test::readJsonData(_testFileName, string(), bSortOnLoad);
    else if (_testFileName.extension() == ".yml")
        testData.data = test::readYamlData(_testFileName, bSortOnLoad);
    else
        ETH_ERROR_MESSAGE("Unknown test format!" + test::TestOutputHelper::get().testFile().string());
    ETH_LOG("Read json structure finish", 5);

    // Do not calculate the hash on Legacy tests unless --checkhash option provided
    if (isLegacy && !bSortOnLoad)
    {
        static bool wasWarning = false;
        if (!wasWarning)
        {
            wasWarning = true;
            ETH_WARNING("Skipping LegacyTests hash verification to save time!");
        }
        testData.hashCalculated = false;
        return testData;
    }

    string srcString = testData.data->asJson(0, false);  // json_spirit::write_string(testData.data, false);
    if (test::Options::get().showhash)
    {
        std::string output = "Not a Json object!";
#ifdef JSONCPP
        if (_testFileName.extension() == ".json")
        {
            Json::FastWriter fastWriter;
            Json::Value v = readJson(_testFileName);
            output = fastWriter.write(v);
            output = output.substr(0, output.size() - 1);
        }
#else
        output = "compile with -DJSONCPP flag to get the json print";
#endif

        std::cerr << "JSON: '" << std::endl << output << "'" << std::endl;
        std::cerr << "DATA: '" << std::endl << srcString << "'" << std::endl;
    }
    testData.hash = sha3(srcString);
    testData.hashCalculated = true;
    return testData;
}

void removeComments(spDataObject& _obj)
{
    if (_obj->type() == DataType::Object)
    {
        list<string> removeList;
        for (auto& i : (*_obj).getSubObjectsUnsafe())
        {
            if (i->getKey().substr(0, 2) == "//")
            {
                removeList.push_back(i->getKey());
                continue;
            }
            removeComments(i);
        }
        for (auto const& i : removeList)
            (*_obj).removeKey(i);
    }
    else if (_obj->type() == DataType::Array)
    {
        for (auto& i : (*_obj).getSubObjectsUnsafe())
            removeComments(i);
    }
}

void checkFillerHash(fs::path const& _compiledTest, fs::path const& _sourceTest)
{
    ETH_LOG(string("Check `") + _compiledTest.c_str() + "` hash", 7);
    TestFileData fillerData = readTestFile(_sourceTest);

    // If no hash calculated, skip the hash check
    if (!fillerData.hashCalculated)
        return;

    spDataObject v = test::readJsonData(_compiledTest, "_info");
    for (auto const& i2 : v->getSubObjects())
    {
        DataObject const& i = i2.getCContent();
        try
        {
            // use eth object _info section class here !!!!!
            ETH_ERROR_REQUIRE_MESSAGE(
                i.type() == DataType::Object, i.getKey() + " should contain an object under a test name.");
            ETH_ERROR_REQUIRE_MESSAGE(i.count("_info") > 0, "_info section not set! " + _compiledTest.string());
            DataObject const& info = i.atKey("_info");
            ETH_ERROR_REQUIRE_MESSAGE(
                info.count("sourceHash") > 0, "sourceHash not found in " + _compiledTest.string() + " in " + i.getKey());

            // Check test version  vs retesteth version
            if (!Options::isLegacy())
            {
                ETH_ERROR_REQUIRE_MESSAGE(info.count("filling-tool-version"),
                    string() + "Field `filling-tool-version` not found in generated test!" + _compiledTest.c_str());
                string strippedVersion;
                string const version = info.atKey("filling-tool-version").asString().substr(9, 10);
                for (auto const& ch : version)
                {
                    if (isdigit(ch))
                        strippedVersion += ch;
                }
                if (strippedVersion.at(0) == '0')
                    strippedVersion.erase(0, 1);
                int testVersion = atoi(strippedVersion.c_str());
                if (testVersion > retestethVersion())
                    ETH_WARNING("Test was filled with newer version of retesteth! (test: `" + strippedVersion +
                                "` vs retesteth: `" + test::fto_string(retestethVersion()) +
                                "`) This might cause failures, please update retesteth!");
                else if (testVersion < retestethVersion() - 1)
                    ETH_WARNING("Test filled with older version of retesteth! (test: `" + strippedVersion +
                                "` vs retesteth: `" + test::fto_string(retestethVersion()) +
                                "`) This might cause failures, please regenerate the test!");
            }

            // Check source hash vs filled test hash
            h256 const sourceHash = h256(info.atKey("sourceHash").asString());
            if (sourceHash != fillerData.hash)
            {
                string sourceHashStr;
                string fillerHashStr;
                if (Options::get().showhash)
                {
                    sourceHashStr = sourceHash.hex();
                    fillerHashStr = fillerData.hash.hex();
                }
                else
                {
                    sourceHashStr = sourceHash.hex().substr(0, 4);
                    fillerHashStr = fillerData.hash.hex().substr(0, 4);
                }
                ETH_ERROR_MESSAGE("Test " + _compiledTest.string() + " is outdated. Filler hash is different! " + "('" +
                                  sourceHashStr + "' != '" + fillerHashStr + "') ");
            }
        }
        catch (test::UpwardsException const&)
        {
            continue;
        }
    }
}

}  // namespace testsuite
}  // namespace test
