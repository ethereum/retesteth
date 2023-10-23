#include "TestSuiteHelperFunctions.h"
#include "Options.h"
#include <retesteth/EthChecks.h>
#include <retesteth/helpers/TestHelper.h>
#include <retesteth/helpers/TestOutputHelper.h>
#include <retesteth/testSuiteRunner/TestSuite.h>
#include <libdevcore/CommonIO.h>

using namespace dev;
using namespace std;
using namespace test::debug;
namespace fs = boost::filesystem;

namespace test::testsuite
{
TestFileData readFillerTestFile(fs::path const& _testFileName)
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
    {
        CJOptions opt { .jsonParse = CJOptions::JsonParse::ALLOW_COMMENTS, .autosort = bSortOnLoad,};
        testData.data = test::readJsonData(_testFileName, opt);
    }
    else if (_testFileName.extension() == ".yml")
        testData.data = test::readYamlData(_testFileName, bSortOnLoad);
    else if (_testFileName.extension() == ".py")
        testData.data = spDataObject(new DataObject(dev::contentsString(_testFileName)));
    else
        ETH_ERROR_MESSAGE("Unknown test format! \n" + _testFileName.string());
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
                removeKeysList.emplace_back(element->getKey());
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


typedef vector<string> VectorString;
std::map<string, VectorString> C_GeneratedTestsMAP;
std::mutex G_GeneratedTestsMap_Mutex;
void clearGeneratedTestNamesMap()
{
    std::lock_guard<std::mutex> lock(G_GeneratedTestsMap_Mutex);
    C_GeneratedTestsMAP.clear();
}

void checkDoubleGeneratedTestNames()
{
    std::lock_guard<std::mutex> lock(G_GeneratedTestsMap_Mutex);

    typedef std::set<string> SetOfString;
    std::map<string, SetOfString> verifiedMap;

    std::set<string> checkedNames;
    for (auto const& test : C_GeneratedTestsMAP)
    {
        SetOfString localNames;
        for (auto const& name : test.second)
        {
            string sPrevious;
            for (auto const& previousTest : verifiedMap)
            {
                if (previousTest.second.count(name))
                    sPrevious = "Previous occurance in test filler: `" + previousTest.first + "`";
            }
            if (checkedNames.count(name))
                ETH_ERROR_MESSAGE("Filler will produce test name collision (change the name): `" +
                                  name + "` in test filler `" + test.first + "`\n" + sPrevious);

            checkedNames.emplace(name);
            localNames.emplace(name);
        }
        verifiedMap.emplace(test.first, localNames);
    }
}

vector<string> const& getGeneratedTestNames(fs::path const& _filler)
{
    std::lock_guard<std::mutex> lock(G_GeneratedTestsMap_Mutex);
    if (C_GeneratedTestsMAP.count(_filler.stem().string()))
        return C_GeneratedTestsMAP.at(_filler.stem().string());

    vector<string> generatedTestNames;
    if (_filler.extension() == ".json" || _filler.extension() == ".yml")
    {
        string fillerName = _filler.stem().string();
        if (fillerName.find(c_fillerPostf) != string::npos)
        {
            fillerName = fillerName.substr(0, fillerName.length() - c_fillerPostf.size());
            generatedTestNames.emplace_back(fillerName);
        }
        else if (fillerName.find(c_copierPostf) != string::npos)
        {
            fillerName = fillerName.substr(0, fillerName.length() - c_copierPostf.size());
            generatedTestNames.emplace_back(fillerName);
        }
        else
            ETH_WARNING("Skipping unsupported test file: " + _filler.string());
    }
    else if (_filler.extension() == ".py")
    {
        // Get filled test names from .py filler
        size_t pos = 0;
        string pythonSrc = dev::contentsString(_filler);
        size_t foundPos = pythonSrc.find("def test_", pos);
        while (foundPos != string::npos)
        {
            size_t endSelectionPos = pythonSrc.find("(", foundPos);
            if (endSelectionPos != string::npos)
            {
                string pythonTestname = pythonSrc.substr(foundPos + 9, endSelectionPos - foundPos - 9);

                size_t foundSkipPos = pythonSrc.rfind("@pytest.mark.skip", foundPos);
                if (foundSkipPos != string::npos)
                {
                    size_t foundPreviuosTest = pythonSrc.rfind("def test_", foundPos - 1);
                    if (foundPreviuosTest != string::npos)
                    {
                        if (foundPreviuosTest <= foundSkipPos)
                        {
                            ETH_WARNING("Pyspec marked test as skipped: " + pythonTestname);
                        }
                        else
                            generatedTestNames.emplace_back(pythonTestname);
                    }
                    else
                        ETH_WARNING("Pyspec marked test as skipped: " + pythonTestname);
                }
                else
                    generatedTestNames.emplace_back(pythonTestname);
            }
            pos = foundPos + 1;
            foundPos = pythonSrc.find("def test_", pos);
        }
    }
    else
    {
        ETH_ERROR_MESSAGE("getGeneratedTestNames:: unknown filler extension: \n" + _filler.string());
    }

    C_GeneratedTestsMAP.emplace(_filler.stem().string(), generatedTestNames);
    return C_GeneratedTestsMAP.at(_filler.stem().string());
}


}  // namespace testsuite
