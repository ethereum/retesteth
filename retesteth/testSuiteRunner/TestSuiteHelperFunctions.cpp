#include "TestSuiteHelperFunctions.h"
#include "Options.h"

using namespace test::debug;
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
    ETH_DC_MESSAGE(DC::TESTLOG, "Read json structure " + string(_testFileName.filename().c_str()));
    TestFileData testData;
    if (_testFileName.extension() == ".json")
        testData.data = test::readJsonData(_testFileName, string(), bSortOnLoad);
    else if (_testFileName.extension() == ".yml")
        testData.data = test::readYamlData(_testFileName, bSortOnLoad);
    else
        ETH_ERROR_MESSAGE("Unknown test format!" + test::TestOutputHelper::get().testFile().string());
    ETH_DC_MESSAGE(DC::TESTLOG, "Read json structure finish");

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

    string const srcString = testData.data->asJson(0, false);
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
        list<string> removeKeysList;
        for (auto& element : (*_obj).getSubObjectsUnsafe())
        {
            if (element->getKey().substr(0, 2) == "//")
            {
                removeKeysList.push_back(element->getKey());
                continue;
            }
            removeComments(element);
        }
        for (auto const& key : removeKeysList)
            (*_obj).removeKey(key);
    }
    else if (_obj->type() == DataType::Array)
    {
        for (auto& element : (*_obj).getSubObjectsUnsafe())
            removeComments(element);
    }
}


}  // namespace testsuite
}  // namespace test
