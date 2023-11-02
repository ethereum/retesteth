#include <libdataobj/DataObject.h>
#include <retesteth/helpers/TestHelper.h>
#include <boost/filesystem.hpp>
#include <retesteth/mainHelper.h>
using namespace std;
using namespace test;
using namespace test::main;
using namespace dataobject;
namespace fs = boost::filesystem;

namespace
{

string getTestType(string const& _filename)
{
    string type = "GeneralStateTests";
    if (fs::exists(_filename))
    {
        spDataObject res = readAutoDataWithoutOptions(_filename);
        if (res.isEmpty())
            return type;

        auto isBlockChainTest = [](DataObject const& _el)
        {
            return (_el.getKey() == "blocks") ? true : false;
        };
        auto isStateTest = [](DataObject const& _el)
        {
            return (_el.getKey() == "env") ? true : false;
        };
        if (res->performSearch(isBlockChainTest))
            return "BlockchainTests";
        if (res->performSearch(isStateTest))
            return "GeneralStateTests";
    }
    return type;
}

string getTestTArg(fs::path const& _cwd, string const& arg)
{
    const vector<string> supportedSuites = {
        "GeneralStateTests", "BlockchainTests",
        "GeneralStateTestsFiller", "BlockchainTestsFiller",
        "EOFTests", "EOFTestsFiller",
        "EIPTests", "EIPTestsFiller",
        "TransactionTests", "TransactionTestsFiller",
        "LegacyTests"
    };
    string cArg = arg;
    if (cArg.size() > 1 && cArg.at(cArg.size() - 1) == '/')
        cArg = cArg.erase(cArg.size() - 1);

    string tArg;
    fs::path cwd = _cwd;
    bool stepinfolder = false;
    if (test::inArray(supportedSuites, cArg))
    {
        stepinfolder = true;
        if (fs::exists(cwd / cArg))
            cwd = cwd / cArg;
    }

    while(!test::inArray(supportedSuites, cwd.stem().string()) && !cwd.empty())
    {
        tArg.insert(0, cwd.stem().string() + "/");
        cwd = cwd.parent_path();
    }
    if (!cwd.empty())
    {
        string headTestSuite = cwd.stem().string();
        size_t const pos = headTestSuite.find("Filler");
        if (pos != string::npos)
            headTestSuite = headTestSuite.erase(pos, 6);
        else
        {
            if (cwd.parent_path().stem() == "BlockchainTests" && headTestSuite == "GeneralStateTests")
            {
                headTestSuite.insert(0, "BC");
                if (cwd.parent_path().parent_path().stem() == "Constantinople")
                    headTestSuite.insert(0, "LegacyTests/Constantinople/");
            }
            else if ((cwd.parent_path().stem() == "EIPTests" || cwd.parent_path().stem() == "EIPTestsFiller")
                     && headTestSuite == "BlockchainTests")
                headTestSuite.insert(0, "EIPTests/");
            else if (cwd.parent_path().stem() == "Constantinople")
                headTestSuite.insert(0, "LegacyTests/Constantinople/");
        }
        if (stepinfolder)
            tArg.insert(0, headTestSuite);
        else
            tArg.insert(0, headTestSuite  + "/");
    }

    if (!stepinfolder)
        tArg.insert(tArg.size(), cArg);

    if (tArg == "BlockchainTests/InvalidBlocks/bcExpectSection")
        tArg = "BlockchainTests/Retesteth/bcExpectSection";
    if (tArg == "GeneralStateTests/stExpectSection")
        tArg = "GeneralStateTests/Retesteth";
    return tArg;
}

fs::path findTestPathArg(int& _argc, const char* _argv[])
{
    // Get Test Path before initializing options
    fs::path testPath;
    const char* ptestPath = std::getenv("ETHEREUM_TEST_PATH");
    if (ptestPath != nullptr)
        testPath = fs::path(ptestPath);

    for (short i = 1; i < _argc; i++)
    {
        string const arg = string{_argv[i]};
        if (arg == "--testpath" && i + 1 < _argc)
            testPath = fs::path(std::string{_argv[i + 1]});
    }
    return testPath;
}

bool abortOnHelpOrVersion(int& _argc, const char* _argv[])
{
    for (short i = 1; i < _argc; i++)
    {
        string const arg = string{_argv[i]};
        if (arg == "--help" || arg == "--version")
            return true;
    }
    return false;
}

const char** makeNewOptions(int& _argc, vector<string> const& _options)
{
    size_t i = 0;
    const char** argv2 = new const char*[_options.size()];
    for (auto const& el : _options)
    {
        char *buffer = new char[el.size() + 1];   //we need extra char for NUL
        memcpy(buffer, el.c_str(), el.size() + 1);
        registerBuffer(buffer);
        argv2[i++] = buffer;
    }
    _argc = _options.size();
    registerFakeArgs(argv2);
    return argv2;
}

struct ParsedOptions
{
    fs::path cwd;
    string filenameArg;
    string subtestArg;
    string directoryArg;
    string directoryArgFull;
    bool fileInsideTheTestRepo = false;
    bool hasTArg = false;
};

typedef vector<string> OptionsVector;
std::tuple<OptionsVector, ParsedOptions> preparseOptions(int& _argc, const char* _argv[])
{
    fs::path testPath = findTestPathArg(_argc, _argv);
    ParsedOptions optInfo;
    vector<string> options;

    bool fileOptionParsed = false;
    optInfo.cwd = fs::path(fs::current_path());
    options.emplace_back(_argv[0]);
    for (short i = 1; i < _argc; i++)
    {
        string const arg = string{_argv[i]};

        if (arg == "-t")
            optInfo.hasTArg = true;

        if (!fileOptionParsed)
        {
            bool isFile = (arg.find(".json") != string::npos || arg.find(".yml") != string::npos
                           || arg.find(".py") != string::npos);

            if (isFile && string{_argv[i - 1]} != "--testfile")
            {
                fileOptionParsed = true;
                optInfo.filenameArg = arg;

                size_t subtestPos = arg.find("::");
                if (subtestPos != string::npos)
                    optInfo.subtestArg = arg.substr(subtestPos + 2);

                if (!testPath.empty() && fs::relative(optInfo.cwd, testPath).string().find("..") == string::npos)
                    optInfo.fileInsideTheTestRepo = true;
            }
            else if (fs::exists(optInfo.cwd / arg) && fs::is_directory(optInfo.cwd / arg))
            {
                optInfo.directoryArg = arg;
                fileOptionParsed = true;
            }
            else if (fs::exists(arg) && fs::is_directory(arg))
            {
                fileOptionParsed = true;
                auto const& argPath = fs::path(arg);
                optInfo.cwd = argPath.parent_path();
                optInfo.directoryArg = argPath.stem().string();
                optInfo.directoryArgFull = arg;
            }
        }

        options.emplace_back(arg);
    }
    return {options, optInfo};
}

void changeOptionsString(vector<string>& options, ParsedOptions const& optInfo)
{
    options.insert(options.begin() + 1, "-t");
    string suite;
    if (!optInfo.filenameArg.empty())
    {
        if (optInfo.fileInsideTheTestRepo)
        {
            suite = getTestTArg(optInfo.cwd.parent_path(), optInfo.cwd.stem().string());
            for (vector<string>::iterator it = options.begin(); it != options.end(); it++)
            {
                if (*it == optInfo.filenameArg)
                {
                    (*it) = (fs::path(*it)).stem().string();
                    if (!optInfo.subtestArg.empty())
                        (*it) += "/" + optInfo.subtestArg;
                    options.insert(it, "--singletest");
                    break;
                }
            }
        }
        else
        {
            suite = getTestType(optInfo.filenameArg);
            for (vector<string>::iterator it = options.begin(); it != options.end(); it++)
            {
                if (*it == optInfo.filenameArg)
                {
                    options.insert(it, "--testfile");
                    break;
                }
            }
        }
    }
    else if (!optInfo.directoryArg.empty())
    {
        suite = getTestTArg(optInfo.cwd, optInfo.directoryArg);
        for (vector<string>::iterator it = options.begin(); it != options.end(); it++)
        {
            if (*it == optInfo.directoryArg || *it == optInfo.directoryArgFull)
            {
                options.erase(it);
                break;
            }
        }
    }

    if (suite.empty())
        std::cerr << "preprocessOptions:: Error autodetecting -t argument!" << std::endl;
    options.insert(options.begin() + 2, suite);
    options.insert(options.begin() + 3, "--");
}

}

namespace test::main
{
const char** preprocessOptions(int& _argc, const char* _argv[])
{
    if (abortOnHelpOrVersion(_argc, _argv))
        return _argv;

    // if file.json is outside of the testpath
    //    parse "retesteth file.json" ==> "retesteth -t TestSuite -- --testfile file.json"
    // else
    //    "retesteth file.json" ==> "retesteth -t TestSuite/Subsuite -- --singletest file.json"
    // parse "retesteth Folder" ==> "retesteth -t TestSuite/Folder

    auto res = preparseOptions(_argc, _argv);
    ParsedOptions optInfo = std::get<1>(res);
    vector<string> options = std::get<0>(res);

    if (!optInfo.hasTArg)
        changeOptionsString(options, optInfo);

    // Print debug line
    for (auto const& el : options)
        std::cout << el << " ";
    std::cout << std::endl;

    if (options.size() == (size_t)_argc)
        return _argv;

    return makeNewOptions(_argc, options);
}}
